/****************************************************************************
 Copyright (c) 2012      greathqy
 Copyright (c) 2012      cocos2d-x.org
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
 ****************************************************************************/

#include "platform/CCPlatformConfig.h"
#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)

#include "HttpClient.h"

#include <queue>
#include <sstream>
#include <stdio.h>
#include <errno.h>

#include "base/CCDirector.h"
#include "platform/CCFileUtils.h"

#include "base/ccUTF8.h"
#include "emscripten.h"
 
NS_CC_BEGIN

namespace network {

static HttpClient* _httpClient = nullptr; // pointer to singleton

// Process Response
void HttpClient::processResponse(HttpResponse* response, char* responseMessage)
{
}

// Worker thread
void HttpClient::networkThread()
{
}

// Worker thread
void HttpClient::networkThreadAlone(HttpRequest* request, HttpResponse* response)
{
}

// HttpClient implementation
HttpClient* HttpClient::getInstance()
{
    if (_httpClient == nullptr) 
    {
        _httpClient = new (std::nothrow) HttpClient();
    }
    
    return _httpClient;
}

void HttpClient::destroyInstance()
{
    if (_httpClient == nullptr)
    {
        CCLOG("HttpClient singleton is nullptr");
        return;
    }

    CCLOG("HttpClient::destroyInstance ...");

    delete _httpClient;
    _httpClient = nullptr;
}

void HttpClient::enableCookies(const char* cookieFile) 
{
}
    
void HttpClient::setSSLVerification(const std::string& caFile)
{
}

HttpClient::HttpClient()
: _timeoutForConnect(30)
, _timeoutForRead(60)
, _isInited(false)
, _threadCount(0)
, _requestSentinel(nullptr)
, _scheduler(nullptr)
, _cookie(nullptr)
{
    cocos2d::Director::getInstance()->getScheduler()->scheduleUpdate(this, 0, false);
}

HttpClient::~HttpClient()
{
    cocos2d::Director::getInstance()->getScheduler()->unscheduleUpdate(this);
}

//Lazy create semaphore & mutex & thread
bool HttpClient::lazyInitThreadSemphore()
{
    return true;
}

static void onLoad(unsigned, void* userData, void *buffer, unsigned bufferSize)
{
    HttpRequest* request = static_cast<HttpRequest*>(userData);
    HttpResponse *response = new (std::nothrow) HttpResponse(request);
    
    char *newBuffer = static_cast<char*>(buffer);
    
    response->setSucceed(true);
    response->setResponseCode(200);
    response->setResponseDataString(newBuffer, bufferSize);
    response->setResponseData(newBuffer, bufferSize);
    request->setSendingCompleted(true);
    
    CCLOG("HttpClient::onLoad bufferSize:%u\n", bufferSize);
    
    const ccHttpRequestCallback& callback = request->getCallback();
    Ref* pTarget = request->getTarget();
    SEL_HttpResponse pSelector = request->getSelector();
    
    if (callback != nullptr)
    {
        callback(nullptr, response);
    }
    else if (pTarget && pSelector)
    {
        (pTarget->*pSelector)(nullptr, response);
    }
    
    response->release();
    // do not release in other thread
    request->release();
};

static void onError(unsigned, void* userData, int errorCode, const char* status)
{
    CCLOG("HttpClient::onError code:%d status:%s\n", errorCode, status);
    
    HttpRequest* request = static_cast<HttpRequest*>(userData);
    HttpResponse *response = new (std::nothrow) HttpResponse(request);
    
    response->setSucceed(false);
    response->setResponseCode(errorCode);
    response->setErrorBuffer(status);
    
    const ccHttpRequestCallback& callback = request->getCallback();
    Ref* pTarget = request->getTarget();
    SEL_HttpResponse pSelector = request->getSelector();
    
    if (callback != nullptr)
    {
        callback(nullptr, response);
    }
    else if (pTarget && pSelector)
    {
        (pTarget->*pSelector)(nullptr, response);
    }
    
    response->release();
    // do not release in other thread
    request->release();
};

static void onProgress(unsigned, void* userData, int, int)
{
    CCLOG("HttpClient::onProgress is connected\n");
    
    HttpRequest* request = static_cast<HttpRequest*>(userData);
    request->setConnected(true);
};
    
//Add a get task to queue
void HttpClient::send(HttpRequest* request)
{
    CCLOG("HttpClient::send\n");
    request->retain();
    request->setHandler(-1);
    _requestQueue.pushBack(request);
}

void HttpClient::sendImmediate(HttpRequest* request)
{
    std::string requestType;
    switch (request->getRequestType())
    {
        case HttpRequest::Type::GET:
            requestType = "GET";
            break;
            
        case HttpRequest::Type::POST:
            requestType = "POST";
            break;
        default:
            break;
    }
    
    request->retain();
    int handler = emscripten_async_wget2_data(request->getUrl(), requestType.c_str(), request->getRequestData(), static_cast<void*>(request), true, &onLoad, &onError, &onProgress);
    request->setHandler(handler);
    _requestQueue.pushBack(request);
    
    CCLOG("HttpClient::sendImmediate %i\n", handler);
}
    
void HttpClient::update(float time)
{
    int index = 0;
    bool sendOneTime = false;
    
    CCLOG("HttpClient::update: _requestQueue %i", _requestQueue.size());
    
    for (auto it = _requestQueue.begin(); it != _requestQueue.end(); )
    {
        HttpRequest* request = (*it);
        
        if (request->getHandler() >= 0)
        {
            double connectTime = request->getConnectTime();
            double sendTime = request->getSendTime();
            
            CCLOG("HttpClient::update: connectTime %f sendTime %f index %i\n", connectTime, sendTime, index);
            
            if (
                (!request->isConnected() && connectTime > _timeoutForConnect) ||
                (!request->isSendingCompleted() && sendTime > _timeoutForRead)
                )
            {
                CCLOG("HttpClient::update: error timeout\n");
                
                onError(0, static_cast<void*>(request), 408, "connect failed");

                emscripten_async_wget2_abort(request->getHandler());
                it = _requestQueue.erase(it);
            }
            else
            {
                if (request->isSendingCompleted())
                {
                    CCLOG("HttpClient::update is sending completed index %i\n", index);
                    
                    it = _requestQueue.erase(it);
                }
                else
                {
                    request->setConnectTime(connectTime + time);
                    request->setSendTime(sendTime + time);
                    ++it;
                }
            }
        }
        else
        {
            if (!sendOneTime)
            {
                std::string requestType;
                switch (request->getRequestType())
                {
                    case HttpRequest::Type::GET:
                        requestType = "GET";
                        break;
                        
                    case HttpRequest::Type::POST:
                        requestType = "POST";
                        break;
                    default:
                        break;
                }
                
                sendOneTime = true;
                int handler = emscripten_async_wget2_data(request->getUrl(), requestType.c_str(), request->getRequestData(), static_cast<void*>(request), true, &onLoad, &onError, &onProgress);
                request->setHandler(handler);
                
                CCLOG("HttpClient::update: sendOneTime %i index %i\n", handler, index);
            }
            
            ++it;
        }
        
        ++index;
    }
    
    CCLOG("-------------------------");
}

// Poll and notify main thread if responses exists in queue
void HttpClient::dispatchResponseCallbacks()
{
}

void HttpClient::increaseThreadCount()
{
}

void HttpClient::decreaseThreadCountAndMayDeleteThis()
{
}

void HttpClient::setTimeoutForConnect(int value)
{
    _timeoutForConnect = value;
}
    
int HttpClient::getTimeoutForConnect()
{
    return _timeoutForConnect;
}
    
void HttpClient::setTimeoutForRead(int value)
{
    _timeoutForRead = value;
}
    
int HttpClient::getTimeoutForRead()
{
    return _timeoutForRead;
}
    
const std::string& HttpClient::getCookieFilename()
{
    return _cookieFilename;
}
    
const std::string& HttpClient::getSSLVerification()
{
    return _sslCaFilename;
}
    
}

NS_CC_END

#endif // #if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
