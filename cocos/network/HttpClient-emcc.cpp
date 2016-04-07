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
}

HttpClient::~HttpClient()
{
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
    std::vector<char> responseBuffer(newBuffer, newBuffer + bufferSize);
    
    response->setSucceed(true);
    response->setResponseCode(200);
    response->setResponseDataString(newBuffer, bufferSize);
    response->setResponseData(&responseBuffer);
    
    printf("HttpClient::send onLoad %s %ld\n", newBuffer, bufferSize);
    
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
    printf("HttpClient::onError\n");
    
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
};
    
//Add a get task to queue
void HttpClient::send(HttpRequest* request)
{
    sendImmediate(request);
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
    
    emscripten_async_wget2_data(request->getUrl(), requestType.c_str(), request->getRequestData(), static_cast<void*>(request), true, &onLoad, &onError, &onProgress);
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
