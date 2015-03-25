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
****************************************************************************/

#include "platform/CCPlatformConfig.h"
#if CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN

#include "CCApplication.h"
//#include "platform/CCFileUtils.h"
#include "base/CCDirector.h"
//#include "CCEGLView.h"
#include <stdio.h>
#include <time.h>
#include <unistd.h>

#ifndef __EMSCRIPTEN__
#define __EMSCRIPTEN__ 1
#endif

#include <emscripten/emscripten.h>

#define  LOGD(...)  emscripten_log(EM_LOG_CONSOLE, ##__VA_ARGS__)

NS_CC_BEGIN

// sharedApplication pointer
Application * Application::sm_pSharedApplication = 0;

// convert the timespec into milliseconds
static long time2millis(struct timespec *times)
{
	return times->tv_sec * 1000 + times->tv_nsec / 1000000;
}

Application::Application()
{
    CCAssert(! sm_pSharedApplication, "");
    sm_pSharedApplication = this;
}

Application::~Application()
{
    CCAssert(this == sm_pSharedApplication, "");
    sm_pSharedApplication = NULL;
}


extern "C" void mainLoopIter()
{
	//CCEGLView::sharedOpenGLView()->handleEvents();
	//TIME_START(mainloop);
	Director::getInstance()->mainLoop();
	//TIME_TOTAL_LOG(mainloop, "one loop");
	//Director::getInstance()->getOpenGLView()->pollEvents();
}

int Application::run()
{

	struct timespec time_struct;
	double update_time;

	initGLContextAttrs();


	// Initialize instance and cocos2d.
	if (!applicationDidFinishLaunching())
	{
		return 1;
	}

	//clock_gettime(CLOCK_REALTIME, &time_struct);
	update_time = emscripten_get_now(); //time2millis(&time_struct);

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();

	glview->retain();

	CCLOG("Mainloop begin %f", director->getAnimationInterval());
	// XXX: Set to 1FPS while debugging
	emscripten_set_main_loop(&mainLoopIter, 0, 1);


	CCLOG("Mainloop end");
	if (glview->isOpenGLReady())
	{
		director->end();
		director->mainLoop();
		director = nullptr;
	}
	glview->release();

	return 0;          
}

void Application::setAnimationInterval(double interval)
{
	CCLOG("Animation interval %f", interval);
	_animationInterval = (long)(interval * 1000);
}

//////////////////////////////////////////////////////////////////////////
// static member function
//////////////////////////////////////////////////////////////////////////
Application* Application::getInstance()
{
    CCAssert(sm_pSharedApplication, "");
    return sm_pSharedApplication;
}

// @deprecated Use getInstance() instead
Application* Application::sharedApplication()
{
    return Application::getInstance();
}

const char * Application::getCurrentLanguageCode()
{
    static char code[3]="en";    
    return code;
}

LanguageType Application::getCurrentLanguage()
{    
    LanguageType ret = LanguageType::ENGLISH; 
    return ret;
}

Application::Platform Application::getTargetPlatform()
{
    return Platform::OS_EMSCRIPTEN;
}

void Application::setResourceRootPath(const std::string& rootResDir)
{

}

const std::string& Application::getResourceRootPath(void)
{
	return std::string("/");
}

bool Application::openURL(const std::string &url)
{
	return false;
}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

