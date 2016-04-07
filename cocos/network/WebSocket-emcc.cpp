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


#define WS_WRITE_BUFFER_SIZE 2048

NS_CC_BEGIN

namespace network {

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
void WebSocket::onOpen(int fd, void* userData)
{
    CCLOG("WebSocket::onOpen");
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    webSocket->_delegate->onOpen(webSocket);
}

void WebSocket::onMessage(int fd, void* userData)
{
    CCLOG("WebSocket::onMessage");
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    int receiveTotal = 0;
    const int maxLength = 4096;
    
    while (1)
    {
        if (receiveTotal > webSocket->_websocketData.size())
        {
            webSocket->_websocketData.resize(webSocket->_websocketData.size() + maxLength);
        }
        
        int receiveLength = ::recvfrom(webSocket->_websocket, webSocket->_websocketData.data() + receiveTotal, maxLength, 0, nullptr, 0);
        receiveTotal += receiveLength;
        
        if (receiveLength < maxLength)
            break;
    }
    
    CCLOG("message size: %ld %ld\n", webSocket->_websocketData.size(), receiveTotal);
    
    cocos2d::network::WebSocket::Data data;
    data.len = receiveTotal;
    data.bytes = new char[data.len];
    memcpy(data.bytes, webSocket->_websocketData.data(), data.len);
        
    webSocket->_delegate->onMessage(webSocket, data);
}

void WebSocket::onError(int fd, int err, const char* msg, void* userData)
{
    CCLOG("WebSocket::onError");
    WebSocket* webSocket = static_cast<WebSocket*>(userData);
    webSocket->_delegate->onError(webSocket, cocos2d::network::WebSocket::ErrorCode::CONNECTION_FAILURE);
    
    int error;
    socklen_t len = sizeof(error);
    
    int ret = getsockopt(fd, SOL_SOCKET, SO_ERROR, &error, &len);
    CCLOG("error message: %s\n", msg);
}
    
#endif
    
bool WebSocket::init(const Delegate& delegate,
                     const std::string& url,
                     const std::vector<std::string>* protocols/* = nullptr*/)
{
    bool useSSL = false;
    std::string host = url;
    size_t pos = 0;
    int port = 80;
    
    _delegate = const_cast<Delegate*>(&delegate);
    
    //ws://
    pos = host.find("ws://");
    if (pos == 0) host.erase(0,5);
    
    pos = host.find("wss://");
    if (pos == 0)
    {
        host.erase(0,6);
        useSSL = true;
    }
    
    pos = host.find(":");
    if (pos != std::string::npos) port = atoi(host.substr(pos+1, host.size()).c_str());
    
    pos = host.find("/", 0);
    std::string path = "/";
    if (pos != std::string::npos) path += host.substr(pos + 1, host.size());
    
    pos = host.find(":");
    if(pos != std::string::npos){
        host.erase(pos, host.size());
    }else if((pos = host.find("/")) != std::string::npos) {
    	host.erase(pos, host.size());
    }
    
    _host = host;
    _port = port;
    _path = path;
    _SSLConnection = useSSL ? 1 : 0;
    
    CCLOG("[WebSocket::init] _host: %s, _port: %d, _path: %s", _host.c_str(), _port, _path.c_str());

    struct addrinfo addr;
    struct addrinfo *res;
    memset(&addr, 0, sizeof addr);
    
    addr.ai_family = AF_INET;
#if !TEST_DGRAM
    addr.ai_socktype = SOCK_STREAM;
#else
    addr.ai_socktype = SOCK_DGRAM;
#endif
    
    int result = getaddrinfo((_host + _path).c_str(), std::to_string(_port).c_str(), &addr, &res);
    
    if (result != 0)
    {
        CCLOG("error getaddrinfo");
    }
    
    _websocket = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (_websocket == -1)
    {
        CCLOG("cannot create socket");
    }
    
    fcntl(_websocket, F_SETFL, O_NONBLOCK);

    // connect the socket
    result = connect(_websocket, res->ai_addr, res->ai_addrlen);
    if (result == -1 && errno != EINPROGRESS)
    {
        CCLOG("connect failed");
        ::close(_websocket);
    }
    
    freeaddrinfo(res);
    
#ifdef __EMSCRIPTEN__
    emscripten_set_socket_error_callback(static_cast<void*>(this), &WebSocket::onError);
    emscripten_set_socket_open_callback(static_cast<void*>(this), &WebSocket::onOpen);
    emscripten_set_socket_message_callback(static_cast<void*>(this), &WebSocket::onMessage);
    
    _websocketData.resize(4096);
#endif
    return true;
}

void WebSocket::send(const std::string& message)
{
    int length = static_cast<int>(message.length());
    ::send(_websocket, message.c_str(), length, 0);
}

void WebSocket::send(const unsigned char* binaryMsg, unsigned int len)
{
    CCASSERT(binaryMsg != nullptr && len > 0, "parameter invalid.");
    ::send(_websocket, binaryMsg, len, 0);
}

void WebSocket::close()
{
    // onClose callback needs to be invoked at the end of this method
    // since websocket instance may be deleted in 'onClose'.
    _delegate->onClose(this);
    
#ifdef __EMSCRIPTEN__
    ::close(_websocket);
#endif
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
