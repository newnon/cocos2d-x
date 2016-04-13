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
#include "CCGLViewImpl-emcc.h"
#include "base/CCDirector.h"
#include "base/ccMacros.h"
#include "base/CCDirector.h"
#include "base/CCTouch.h"
#include "base/CCEventDispatcher.h"
#include "base/CCEventKeyboard.h"
#include "base/CCEventMouse.h"
#include "base/CCIMEDispatcher.h"
#include "base/ccUtils.h"
#include "base/ccUTF8.h"


#include <emscripten/emscripten.h>

#include <SDL2/SDL.h>
#include <emscripten.h>

NS_CC_BEGIN


struct keyCodeItem
{
	int glfwKeyCode;
	EventKeyboard::KeyCode keyCode;
};

static std::unordered_map<int, EventKeyboard::KeyCode> g_keyCodeMap;

static keyCodeItem g_keyCodeStructArray[] = {
	/* The unknown key */
	{ SDLK_UNKNOWN, EventKeyboard::KeyCode::KEY_NONE },

	/* Printable keys */
	{ SDLK_SPACE, EventKeyboard::KeyCode::KEY_SPACE },
	{ SDLK_QUOTE, EventKeyboard::KeyCode::KEY_APOSTROPHE },
	{ SDLK_COMMA, EventKeyboard::KeyCode::KEY_COMMA },
	{ SDLK_MINUS, EventKeyboard::KeyCode::KEY_MINUS },
	{ SDLK_PERIOD, EventKeyboard::KeyCode::KEY_PERIOD },
	{ SDLK_SLASH, EventKeyboard::KeyCode::KEY_SLASH },
	{ SDLK_0, EventKeyboard::KeyCode::KEY_0 },
	{ SDLK_1, EventKeyboard::KeyCode::KEY_1 },
	{ SDLK_2, EventKeyboard::KeyCode::KEY_2 },
	{ SDLK_3, EventKeyboard::KeyCode::KEY_3 },
	{ SDLK_4, EventKeyboard::KeyCode::KEY_4 },
	{ SDLK_5, EventKeyboard::KeyCode::KEY_5 },
	{ SDLK_6, EventKeyboard::KeyCode::KEY_6 },
	{ SDLK_7, EventKeyboard::KeyCode::KEY_7 },
	{ SDLK_8, EventKeyboard::KeyCode::KEY_8 },
	{ SDLK_9, EventKeyboard::KeyCode::KEY_9 },
	{ SDLK_SEMICOLON, EventKeyboard::KeyCode::KEY_SEMICOLON },
	{ SDLK_EQUALS, EventKeyboard::KeyCode::KEY_EQUAL },
	{ SDLK_a, EventKeyboard::KeyCode::KEY_A },
	{ SDLK_b, EventKeyboard::KeyCode::KEY_B },
	{ SDLK_c, EventKeyboard::KeyCode::KEY_C },
	{ SDLK_d, EventKeyboard::KeyCode::KEY_D },
	{ SDLK_e, EventKeyboard::KeyCode::KEY_E },
	{ SDLK_f, EventKeyboard::KeyCode::KEY_F },
	{ SDLK_g, EventKeyboard::KeyCode::KEY_G },
	{ SDLK_h, EventKeyboard::KeyCode::KEY_H },
	{ SDLK_i, EventKeyboard::KeyCode::KEY_I },
	{ SDLK_j, EventKeyboard::KeyCode::KEY_J },
	{ SDLK_k, EventKeyboard::KeyCode::KEY_K },
	{ SDLK_l, EventKeyboard::KeyCode::KEY_L },
	{ SDLK_m, EventKeyboard::KeyCode::KEY_M },
	{ SDLK_n, EventKeyboard::KeyCode::KEY_N },
	{ SDLK_o, EventKeyboard::KeyCode::KEY_O },
	{ SDLK_p, EventKeyboard::KeyCode::KEY_P },
	{ SDLK_q, EventKeyboard::KeyCode::KEY_Q },
	{ SDLK_r, EventKeyboard::KeyCode::KEY_R },
	{ SDLK_s, EventKeyboard::KeyCode::KEY_S },
	{ SDLK_t, EventKeyboard::KeyCode::KEY_T },
	{ SDLK_u, EventKeyboard::KeyCode::KEY_U },
	{ SDLK_v, EventKeyboard::KeyCode::KEY_V },
	{ SDLK_w, EventKeyboard::KeyCode::KEY_W },
	{ SDLK_x, EventKeyboard::KeyCode::KEY_X },
	{ SDLK_y, EventKeyboard::KeyCode::KEY_Y },
	{ SDLK_z, EventKeyboard::KeyCode::KEY_Z },
	{ SDLK_LEFTBRACKET, EventKeyboard::KeyCode::KEY_LEFT_BRACKET },
	{ SDLK_BACKSLASH, EventKeyboard::KeyCode::KEY_BACK_SLASH },
	{ SDLK_RIGHTBRACKET, EventKeyboard::KeyCode::KEY_RIGHT_BRACKET },
	{ SDLK_BACKQUOTE, EventKeyboard::KeyCode::KEY_GRAVE },

	/* Function keys */
	{ SDLK_ESCAPE, EventKeyboard::KeyCode::KEY_ESCAPE },
	{ SDLK_RETURN, EventKeyboard::KeyCode::KEY_KP_ENTER },
	{ SDLK_TAB, EventKeyboard::KeyCode::KEY_TAB },
	{ SDLK_BACKSPACE, EventKeyboard::KeyCode::KEY_BACKSPACE },
	{ SDLK_INSERT, EventKeyboard::KeyCode::KEY_INSERT },
	{ SDLK_DELETE, EventKeyboard::KeyCode::KEY_DELETE },
	{ SDLK_RIGHT, EventKeyboard::KeyCode::KEY_RIGHT_ARROW },
	{ SDLK_LEFT, EventKeyboard::KeyCode::KEY_LEFT_ARROW },
	{ SDLK_DOWN, EventKeyboard::KeyCode::KEY_DOWN_ARROW },
	{ SDLK_UP, EventKeyboard::KeyCode::KEY_UP_ARROW },
	{ SDLK_PAGEUP, EventKeyboard::KeyCode::KEY_KP_PG_UP },
	{ SDLK_PAGEDOWN, EventKeyboard::KeyCode::KEY_KP_PG_DOWN },
	{ SDLK_HOME, EventKeyboard::KeyCode::KEY_KP_HOME },
	{ SDLK_END, EventKeyboard::KeyCode::KEY_END },
	{ SDLK_CAPSLOCK, EventKeyboard::KeyCode::KEY_CAPS_LOCK },
	{ SDLK_SCROLLLOCK, EventKeyboard::KeyCode::KEY_SCROLL_LOCK },
	{ SDLK_NUMLOCK, EventKeyboard::KeyCode::KEY_NUM_LOCK },
	{ SDLK_PRINTSCREEN, EventKeyboard::KeyCode::KEY_PRINT },
	{ SDLK_PAUSE, EventKeyboard::KeyCode::KEY_PAUSE },
	{ SDLK_F1, EventKeyboard::KeyCode::KEY_F1 },
	{ SDLK_F2, EventKeyboard::KeyCode::KEY_F2 },
	{ SDLK_F3, EventKeyboard::KeyCode::KEY_F3 },
	{ SDLK_F4, EventKeyboard::KeyCode::KEY_F4 },
	{ SDLK_F5, EventKeyboard::KeyCode::KEY_F5 },
	{ SDLK_F6, EventKeyboard::KeyCode::KEY_F6 },
	{ SDLK_F7, EventKeyboard::KeyCode::KEY_F7 },
	{ SDLK_F8, EventKeyboard::KeyCode::KEY_F8 },
	{ SDLK_F9, EventKeyboard::KeyCode::KEY_F9 },
	{ SDLK_F10, EventKeyboard::KeyCode::KEY_F10 },
	{ SDLK_F11, EventKeyboard::KeyCode::KEY_F11 },
	{ SDLK_F12, EventKeyboard::KeyCode::KEY_F12 },
	{ SDLK_F13, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F14, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F15, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F16, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F17, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F18, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F19, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F20, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F21, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F22, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F23, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_F24, EventKeyboard::KeyCode::KEY_NONE },
	{ SDLK_KP_0, EventKeyboard::KeyCode::KEY_0 },
	{ SDLK_KP_1, EventKeyboard::KeyCode::KEY_1 },
	{ SDLK_KP_2, EventKeyboard::KeyCode::KEY_2 },
	{ SDLK_KP_3, EventKeyboard::KeyCode::KEY_3 },
	{ SDLK_KP_4, EventKeyboard::KeyCode::KEY_4 },
	{ SDLK_KP_5, EventKeyboard::KeyCode::KEY_5 },
	{ SDLK_KP_6, EventKeyboard::KeyCode::KEY_6 },
	{ SDLK_KP_7, EventKeyboard::KeyCode::KEY_7 },
	{ SDLK_KP_8, EventKeyboard::KeyCode::KEY_8 },
	{ SDLK_KP_9, EventKeyboard::KeyCode::KEY_9 },
	{ SDLK_KP_DECIMAL, EventKeyboard::KeyCode::KEY_PERIOD },
	{ SDLK_KP_DIVIDE, EventKeyboard::KeyCode::KEY_KP_DIVIDE },
	{ SDLK_KP_MULTIPLY, EventKeyboard::KeyCode::KEY_KP_MULTIPLY },
	{ SDLK_KP_MEMSUBTRACT, EventKeyboard::KeyCode::KEY_KP_MINUS },
	{ SDLK_KP_MEMADD, EventKeyboard::KeyCode::KEY_KP_PLUS },
	{ SDLK_KP_ENTER, EventKeyboard::KeyCode::KEY_KP_ENTER },
	{ SDLK_KP_EQUALS, EventKeyboard::KeyCode::KEY_EQUAL },
	{ SDLK_LSHIFT, EventKeyboard::KeyCode::KEY_SHIFT },
	{ SDLK_LCTRL, EventKeyboard::KeyCode::KEY_CTRL },
	{ SDLK_LALT, EventKeyboard::KeyCode::KEY_ALT },
	{ SDLK_LGUI, EventKeyboard::KeyCode::KEY_HYPER },
	{ SDLK_RSHIFT, EventKeyboard::KeyCode::KEY_SHIFT },
	{ SDLK_RCTRL, EventKeyboard::KeyCode::KEY_CTRL },
	{ SDLK_RALT, EventKeyboard::KeyCode::KEY_ALT },
	{ SDLK_RGUI, EventKeyboard::KeyCode::KEY_HYPER },
	{ SDLK_MENU, EventKeyboard::KeyCode::KEY_MENU },
	{ SDLK_LAST, EventKeyboard::KeyCode::KEY_NONE }
};

GLViewImpl* GLViewImpl::createWithRect(const std::string& viewName, Rect rect, float frameZoomFactor)
{
	auto ret = new GLViewImpl;
    if(ret && ret->initWithRect(viewName, rect, frameZoomFactor)) {
		ret->autorelease();
		return ret;
	}

	return nullptr;
}

GLViewImpl* GLViewImpl::create(const std::string& viewName)
{
	auto ret = new GLViewImpl;
    if(ret && ret->initWithRect(viewName, Rect(0, 0, 960, 640), 1)) {
		ret->autorelease();
		return ret;
	}

	return nullptr;
}

GLViewImpl* GLViewImpl::createWithFullScreen(const std::string& viewName)
{
	auto ret = new GLViewImpl();
	if (ret && ret->initWithFullScreen(viewName)) {
		ret->autorelease();
		return ret;
	}

	return nullptr;
}

GLViewImpl::GLViewImpl() :
_mainWindow(nullptr),
_captured(false)
{
}

GLViewImpl::~GLViewImpl()
{
    SDL_Quit();
}

bool GLViewImpl::initWithRect(const std::string& viewName, Rect rect, float frameZoomFactor)
{
	setViewName(viewName); 

	CCLOG("GL Context attrs: %d %d %d %d %d %d", _glContextAttrs.redBits, _glContextAttrs.greenBits, _glContextAttrs.blueBits, _glContextAttrs.alphaBits, _glContextAttrs.depthBits, _glContextAttrs.stencilBits);
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, _glContextAttrs.redBits);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, _glContextAttrs.greenBits);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, _glContextAttrs.blueBits);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, _glContextAttrs.alphaBits);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, _glContextAttrs.depthBits);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, _glContextAttrs.stencilBits);
    
    _mainWindow = SDL_CreateWindow(
                                   viewName.c_str(),
                                   SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                   rect.size.width, rect.size.height, SDL_WINDOW_OPENGL | SDL_WINDOW_INPUT_GRABBED | SDL_WINDOW_RESIZABLE
                                   );
    
    _glContext = SDL_GL_CreateContext(_mainWindow);

    setFrameSize(rect.size.width, rect.size.height);
    
    emscripten_get_canvas_size(&_windowWidth, &_windowHeight, &_windowFullscreen);

	const GLubyte* glVersion = glGetString(GL_VERSION);
	CCLOG("Version %s", glVersion);
	return true;
}

bool GLViewImpl::initWithFullScreen(const std::string& viewName)
{
	return initWithRect(viewName, Rect(0, 0, 800, 600), 1.0f);
}

bool GLViewImpl::isOpenGLReady()
{
	return nullptr != _mainWindow;
}

void GLViewImpl::end()
{
	if (_mainWindow)
	{
        SDL_GL_DeleteContext(_glContext);
        SDL_DestroyWindow(_mainWindow);
		_mainWindow = nullptr;
	}

	// Release self. Otherwise, GLViewImpl could not be freed.
	release();
}

void GLViewImpl::swapBuffers()
{
	if (_mainWindow)
	{
        SDL_GL_SwapBuffers();
	}
}

void GLViewImpl::pollEvents()
{
    int windowWidth = 0;
    int windowHeight = 0;
    int windowFullscreen = 0;
    emscripten_get_canvas_size(&windowWidth, &windowHeight, &windowFullscreen);
    
    if (windowWidth != _windowWidth || windowHeight != _windowHeight || windowFullscreen != _windowFullscreen)
    {
        _windowWidth = windowWidth;
        _windowHeight = windowHeight;
        _windowFullscreen = windowFullscreen;
        
        screenSizeChanged(_windowWidth, _windowHeight);
        Application::getInstance()->applicationScreenSizeChanged(_windowWidth, _windowHeight);
        CCLOG("change window size(%i, %i, %i)\n", _windowWidth, _windowHeight, _windowFullscreen);
    }
    
    SDL_Event event;
    while (SDL_PollEvent(&event) > 0)
    {
        switch(event.type)
        {
            case SDL_FINGERMOTION:
            {
                SDL_TouchFingerEvent *touch = (SDL_TouchFingerEvent*)&event.tfinger;
                intptr_t touchId = (intptr_t)touch->fingerId;
                float mouseX = static_cast<float>(touch->x) / this->getFrameZoomFactor();
                float mouseY = static_cast<float>(touch->y) / this->getFrameZoomFactor();
                float maxForce = 1.0f;
                
                float cursorX = (mouseX - _viewPortRect.origin.x) / _scaleX;
                float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - mouseY) / _scaleY;
                
                auto glview = cocos2d::Director::getInstance()->getOpenGLView();
                glview->handleTouchesMove(1, &touchId, &cursorX, &cursorY, &touch->pressure, &maxForce);
                break;
            }
             
            case SDL_FINGERDOWN:
            {
                SDL_TouchFingerEvent *touch = (SDL_TouchFingerEvent*)&event.tfinger;
                intptr_t touchId = (intptr_t)touch->fingerId;
                float mouseX = static_cast<float>(touch->x) / this->getFrameZoomFactor();
                float mouseY = static_cast<float>(touch->y) / this->getFrameZoomFactor();
                
                float cursorX = (mouseX - _viewPortRect.origin.x) / _scaleX;
                float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - mouseY) / _scaleY;
                
                auto glview = cocos2d::Director::getInstance()->getOpenGLView();
                glview->handleTouchesBegin(1, &touchId, &cursorX, &cursorY);
                break;
            }
                
            case SDL_FINGERUP:
            {
                SDL_TouchFingerEvent *touch = (SDL_TouchFingerEvent*)&event.tfinger;
                intptr_t touchId = (intptr_t)touch->fingerId;
                float mouseX = static_cast<float>(touch->x) / this->getFrameZoomFactor();
                float mouseY = static_cast<float>(touch->y) / this->getFrameZoomFactor();
                
                float cursorX = (mouseX - _viewPortRect.origin.x) / _scaleX;
                float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - mouseY) / _scaleY;
                
                auto glview = cocos2d::Director::getInstance()->getOpenGLView();
                glview->handleTouchesEnd(1, &touchId, &cursorX, &cursorY);
                break;
            }
   
            case SDL_MOUSEMOTION:
            {
                SDL_MouseMotionEvent *mouse = (SDL_MouseMotionEvent*)&event;
                onMouseMoveCallBack(mouse->x, mouse->y);
                break;
            }
                
            case SDL_MOUSEBUTTONDOWN:
            case SDL_MOUSEBUTTONUP:
            {
                SDL_MouseButtonEvent *mouse = (SDL_MouseButtonEvent*)&event;
                onMouseCallBack(mouse->button, event.type, mouse->x, mouse->y);
                break;
            }
                
            case SDL_MOUSEWHEEL:
            {
                SDL_MouseWheelEvent *mouse = (SDL_MouseWheelEvent*)&event;
                onMouseScrollCallback(mouse->x, mouse->y);
                break;
            }
                
            case SDL_KEYDOWN:
            case SDL_KEYUP:
            {
                SDL_KeyboardEvent *key = (SDL_KeyboardEvent*)&event;
                onKeyCallback(key->keysym.sym, key->state, key->repeat);
                break;
            }
                
//            case SDL_TEXTINPUT:
//            {
//                SDL_TextInputEvent *key = (SDL_TextInputEvent*)&event;
//                
//                CCLOG("text input %c", key->text[0]);
//                
//                for(int i = 0; i < SDL_TEXTINPUTEVENT_TEXT_SIZE; ++i)
//                {
//                    onCharCallback(static_cast<unsigned int>(key->text[i]));
//                }
//                break;
//            }
        }
    }
}

void GLViewImpl::setIMEKeyboardState(bool bOpen)
{
}

void GLViewImpl::onMouseCallBack(int button, int action, double x, double y)
{
    float mouseX = static_cast<float>(x) / this->getFrameZoomFactor();
    float mouseY = static_cast<float>(y) / this->getFrameZoomFactor();
    Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
    
	if (SDL_BUTTON_LEFT == button)
	{
		if (SDL_MOUSEBUTTONDOWN == action)
		{
			_captured = true;
			if (this->getViewPortRect().equals(Rect::ZERO) || this->getViewPortRect().containsPoint(Vec2(mouseX, mouseY)))
			{
				intptr_t id = 0;
				this->handleTouchesBegin(1, &id, &mouseX, &mouseY);
			}
		}
		else if (SDL_MOUSEBUTTONUP == action)
		{
			if (_captured)
			{
				_captured = false;
				intptr_t id = 0;
				this->handleTouchesEnd(1, &id, &mouseX, &mouseY);
			}
		}
	}
    
	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    if (SDL_MOUSEBUTTONDOWN == action || SDL_MOUSEBUTTONUP == action)
    {
        float cursorX = (mouseX - _viewPortRect.origin.x) / _scaleX;
        float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - mouseY) / _scaleY;
        
        EventMouse event(SDL_MOUSEBUTTONDOWN == action ? EventMouse::MouseEventType::MOUSE_DOWN : EventMouse::MouseEventType::MOUSE_UP);
        event.setCursorPosition(cursorX, cursorY);
        event.setMouseButton(button);
        Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
    }
}

void GLViewImpl::onMouseMoveCallBack(double x, double y)
{
	float mouseX = static_cast<float>(x) / this->getFrameZoomFactor();
	float mouseY = static_cast<float>(y) / this->getFrameZoomFactor();
    Uint32 mouseState = SDL_GetMouseState(nullptr, nullptr);
    
	if (_captured)
	{
		intptr_t id = 0;
		this->handleTouchesMove(1, &id, &mouseX, &mouseY);
	}

	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
    float cursorX = (mouseX - _viewPortRect.origin.x) / _scaleX;
    float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - mouseY) / _scaleY;
    
    EventMouse event(EventMouse::MouseEventType::MOUSE_MOVE);
    
	// Set current button
	if (mouseState & SDL_BUTTON(SDL_BUTTON_LEFT))
	{
		event.setMouseButton(SDL_BUTTON_LEFT);
	}
	else if (mouseState & SDL_BUTTON(SDL_BUTTON_RIGHT))
	{
		event.setMouseButton(SDL_BUTTON_RIGHT);
	}
	else if (mouseState & SDL_BUTTON(SDL_BUTTON_MIDDLE))
	{
		event.setMouseButton(SDL_BUTTON_MIDDLE);
	}
    
	event.setCursorPosition(cursorX, cursorY);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GLViewImpl::onMouseScrollCallback(double x, double y)
{
	CCLOG("Scroll %f %f", x, y);
    float mouseX = static_cast<float>(x) / this->getFrameZoomFactor();
    float mouseY = static_cast<float>(y) / this->getFrameZoomFactor();
    
	EventMouse event(EventMouse::MouseEventType::MOUSE_SCROLL);
	//Because OpenGL and cocos2d-x uses different Y axis, we need to convert the coordinate here
	float cursorX = (mouseX - _viewPortRect.origin.x) / _scaleX;
	float cursorY = (_viewPortRect.origin.y + _viewPortRect.size.height - mouseY) / _scaleY;
	event.setScrollData(static_cast<float>(x), -static_cast<float>(y));
	event.setCursorPosition(cursorX, cursorY);
	Director::getInstance()->getEventDispatcher()->dispatchEvent(&event);
}

void GLViewImpl::onKeyCallback(int key, int action, int repeat)
{
    if (repeat == 0)
	{
		EventKeyboard event(g_keyCodeMap[key], SDL_PRESSED == action);
		auto dispatcher = Director::getInstance()->getEventDispatcher();
		dispatcher->dispatchEvent(&event);
	}
    
	if (SDL_RELEASED != action && g_keyCodeMap[key] == EventKeyboard::KeyCode::KEY_BACKSPACE)
	{
		IMEDispatcher::sharedDispatcher()->dispatchDeleteBackward();
	}
}

//void GLViewImpl::onCharCallback(unsigned int character)
//{
//	char16_t wcharString[2] = { (char16_t)character, 0 };
//	std::string utf8String;
//
//	StringUtils::UTF16ToUTF8(wcharString, utf8String);
//	IMEDispatcher::sharedDispatcher()->dispatchInsertText(utf8String.c_str(), utf8String.size());
//}

NS_CC_END

#endif // CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
