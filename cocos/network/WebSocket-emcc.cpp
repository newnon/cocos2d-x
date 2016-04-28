/****************************************************************************
 Copyright (c) 2010-2012 cocos2d-x.org
 Copyright (c) 2013-2014 Chukong Technologies Inc.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.

"[WebSocket module] is based in part on the work of the libwebsockets  project
(http://libwebsockets.org)"

 ****************************************************************************/

#include "WebSocket.h"
#include "base/CCDirector.h"
#include "base/CCScheduler.h"

#include <queue>
#include <list>
#include <signal.h>

#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <assert.h>
#include <netdb.h>
#include <limits.h>
#include <getopt.h>
#include <sys/select.h>
#include <sys/stat.h>

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif


NS_CC_BEGIN

namespace network {
    
extern "C"
{
    void WebSocket_init(const char *);
    void WebSocket_send(const char *);
    void WebSocket_send_data(unsigned char *, unsigned int);
    void WebSocket_close();
    
    typedef void (*socket_callback)(void* userData, unsigned char *msg, int length);
    typedef void (*socket_close_callback)(int err, unsigned char *msg, int length, void* userData);
    typedef void (*socket_error_callback)(void* userData);
    
    void WebSocket_set_socket_open_callback(void *userData, socket_callback callback);
    void WebSocket_set_socket_listen_callback(void *userData, socket_callback callback);
    void WebSocket_set_socket_connection_callback(void *userData, socket_callback callback);
    void WebSocket_set_socket_message_callback(void *userData, socket_callback callback);
    
    void WebSocket_set_socket_error_callback(void *userData, socket_error_callback callback);
    void WebSocket_set_socket_close_callback(void *userData, socket_close_callback callback);
};

WebSocket::WebSocket()
: _readyState(State::CONNECTING)
, _port(80)
, _pendingFrameDataLen(0)
, _currentDataLen(0)
, _currentData(nullptr)
, _wsHelper(nullptr)
, _wsInstance(nullptr)
, _wsContext(nullptr)
, _delegate(nullptr)
, _SSLConnection(0)
, _wsProtocols(nullptr)
{
}

WebSocket::~WebSocket()
{
    close();
}

#ifdef __EMSCRIPTEN__
    
void WebSocket::onOpen(void* userData, unsigned char *msg, int length)
{
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    
    if (webSocket && webSocket->_delegate)
        webSocket->_delegate->onOpen(webSocket);
}

void WebSocket::onMessage(void* userData, unsigned char *msg, int length)
{
    CCLOG("WebSocket message: %s size:%d", (char*)msg, length);
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    
    if (userData)
    {
        cocos2d::network::WebSocket::Data data;
        data.len = length;
        data.bytes = (char*)msg;
    
        if (webSocket->_delegate)
            webSocket->_delegate->onMessage(webSocket, data);
    }
}

void WebSocket::onError(void* userData)
{
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    
    if (userData)
    {
        CCLOG("WebSocket::onError");
        
        if (webSocket->_delegate)
            webSocket->_delegate->onError(webSocket, cocos2d::network::WebSocket::ErrorCode::CONNECTION_FAILURE);
    }
}
    
void WebSocket::onClose(int err, unsigned char* msg, int length, void* userData)
{
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    
    if (userData)
    {
        if (webSocket->_readyState == State::CLOSING || webSocket->_readyState == State::CLOSED)
            return;
        
        CCLOG("WebSocket::onClose: errorCode:%i %s", err, (char*)msg);

        webSocket->_readyState = State::CLOSED;
        
        if (webSocket->_delegate)
            webSocket->_delegate->onClose(webSocket);
    }
}
    
#endif
    
bool WebSocket::init(const Delegate& delegate,
                     const std::string& url,
                     const std::vector<std::string>* protocols/* = nullptr*/)
{
    WebSocket_init(url.c_str());
    
    _delegate = const_cast<Delegate*>(&delegate);
    
#ifdef __EMSCRIPTEN__
    WebSocket_set_socket_error_callback(static_cast<void*>(this), &WebSocket::onError);
    WebSocket_set_socket_open_callback(static_cast<void*>(this), &WebSocket::onOpen);
    WebSocket_set_socket_message_callback(static_cast<void*>(this), &WebSocket::onMessage);
    WebSocket_set_socket_close_callback(static_cast<void*>(this), &WebSocket::onClose);
#endif
    _readyState = State::OPEN;
    
    return true;
}

void WebSocket::send(const std::string& message)
{
    if (_readyState == State::OPEN)
    {
        WebSocket_send(message.c_str());
    }
}

void WebSocket::send(const unsigned char* binaryMsg, unsigned int len)
{
    if (_readyState == State::OPEN)
    {
        CCASSERT(binaryMsg != nullptr && len > 0, "parameter invalid.");
        
        WebSocket_send_data((unsigned char*)binaryMsg, len);
    }
}

void WebSocket::close()
{
    if (_readyState == State::CLOSING || _readyState == State::CLOSED)
    {
        return;
    }
    
    _readyState = State::CLOSED;
    
    // onClose callback needs to be invoked at the end of this method
    // since websocket instance may be deleted in 'onClose'.
    _delegate->onClose(this);
    
    WebSocket_close();
}

WebSocket::State WebSocket::getReadyState()
{
    return _readyState;
}

int WebSocket::onSubThreadLoop()
{
    return 0;
}

void WebSocket::onSubThreadStarted()
{
}

void WebSocket::onSubThreadEnded()
{
}

int WebSocket::onSocketCallback(struct libwebsocket_context *ctx,
                     struct libwebsocket *wsi,
                     int reason,
                     void *user, void *in, ssize_t len)
{
	return 0;
}

void WebSocket::onUIThreadReceiveMessage(WsMessage* msg)
{
}

}

NS_CC_END
