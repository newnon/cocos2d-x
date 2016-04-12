#include "HelloWorldScene.h"
#include "AppMacros.h"
#include <vector>
#include <sstream>
#include <iomanip>
#include "stdio.h"
#include "stdlib.h"

#include "network/HttpClient.h"


USING_NS_CC;


Scene* HelloWorld::scene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    HelloWorld *layer = HelloWorld::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

template<typename T>
void logData(const char* key)
{
    Data data = UserDefault::getInstance()->getDataForKey(key);
    T* buffer = (T*) data.getBytes();
    ssize_t length = data.getSize() / sizeof(T);
    
    std::ostringstream ss;
    ss << std::setprecision(2) << std::fixed;
    for (int i = 0; i < length; i++)
    {
        ss << buffer[i] << " ";
    }
    
    CCLOG("%s is %s", key, ss.str().c_str());
}

template<typename T>
void setData(const char* key)
{
    Data data;
    std::vector<T> v;
    
    for (int i = 0; i <= 5; i++)
    {
        v.push_back(static_cast<T>(i));
    }
    data.copy((unsigned char*) v.data(), v.size() * sizeof(T));
    UserDefault::getInstance()->setDataForKey(key, data);
}

template<typename T>
void setData2(const char* key)
{
    Data data;
    std::vector<T> v;
    
    for (int i = 5; i >= 0; i--)
    {
        v.push_back(static_cast<T>(i));
    }
    data.copy((unsigned char*) v.data(), v.size() * sizeof(T));
    UserDefault::getInstance()->setDataForKey(key, data);
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }
    
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                        "CloseNormal.png",
                                        "CloseSelected.png",
                                        CC_CALLBACK_1(HelloWorld::menuCloseCallback,this));
    
    closeItem->setPosition(origin + Vec2(visibleSize) - Vec2(closeItem->getContentSize() / 2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);
    
    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label
    
    _label = Label::createWithTTF("Hello World", "fonts/arial.ttf", TITLE_FONT_SIZE);
    
    // position the label on the center of the screen
    _label->setPosition(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - _label->getContentSize().height);

    // add the label as a child to this layer
    this->addChild(_label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize / 2) + origin);

    // add the sprite as a child to this layer
    this->addChild(sprite);
    
    UserDefault::getInstance()->setStringForKey("string", "value1");
    UserDefault::getInstance()->setIntegerForKey("integer", 10);
    UserDefault::getInstance()->setFloatForKey("float", 2.3f);
    UserDefault::getInstance()->setDoubleForKey("double", 2.4);
    UserDefault::getInstance()->setBoolForKey("bool", true);
    
    // test saving of Data buffers
    setData<int>("int_data");
    setData<float>("float_data");
    setData<double>("double_data");
    
    printValue();
    
    logData<int>("int_data");
    logData<float>("float_data");
    logData<double>("double_data");
    
    //CCUserDefault::getInstance()->flush();
    
    // change the value
    
    UserDefault::getInstance()->setStringForKey("string", "value2");
    UserDefault::getInstance()->setIntegerForKey("integer", 11);
    UserDefault::getInstance()->setFloatForKey("float", 2.5f);
    UserDefault::getInstance()->setDoubleForKey("double", 2.6);
    UserDefault::getInstance()->setBoolForKey("bool", false);
    
    setData2<int>("int_data");
    setData2<float>("float_data");
    setData2<double>("double_data");
    
    UserDefault::getInstance()->flush();
    
    // print value
    printValue();
    
    logData<int>("int_data");
    logData<float>("float_data");
    logData<double>("double_data");
    
    UserDefault::getInstance()->deleteValueForKey("string");
    UserDefault::getInstance()->deleteValueForKey("integer");
    UserDefault::getInstance()->deleteValueForKey("float");
    UserDefault::getInstance()->deleteValueForKey("double");
    UserDefault::getInstance()->deleteValueForKey("bool");
    
    // print value
    printValue();
    
    for (int i = 0; i < 2; ++i)
    {
        cocos2d::network::HttpRequest* request = new cocos2d::network::HttpRequest();
        request->setUrl("http://httpbin.org/get");
        request->setRequestType(cocos2d::network::HttpRequest::Type::GET);
        request->setResponseCallback([this](cocos2d::network::HttpClient* client, cocos2d::network::HttpResponse* response)
        {
            if (!response->isSucceed())
            {
                CCLOG("get data error: buffer: %s  code: %ld\n", response->getErrorBuffer(), response->getResponseCode());
            }
            else
            {
                CCLOG("get data success: buffer: %s  code: %ld\n", response->getResponseDataString(), response->getResponseCode());
            }
        });
        
        cocos2d::network::HttpClient::getInstance()->send(request);
        request->release();
    }
    
    return true;
}

void HelloWorld::printValue()
{
    char strTemp[256] = "";
    // print value
    std::string ret = UserDefault::getInstance()->getStringForKey("string");
    sprintf(strTemp, "string is %s", ret.c_str());
    this->_label->setString(this->_label->getString() + "\n" + strTemp);
    
    double d = UserDefault::getInstance()->getDoubleForKey("double");
    sprintf(strTemp, "double is %f", d);
    this->_label->setString(this->_label->getString() + "\n" + strTemp);
    
    int i = UserDefault::getInstance()->getIntegerForKey("integer");
    sprintf(strTemp, "integer is %d", i);
    this->_label->setString(this->_label->getString() + "\n" + strTemp);
    
    float f = UserDefault::getInstance()->getFloatForKey("float");
    sprintf(strTemp, "float is %f", f);
    this->_label->setString(this->_label->getString() + "\n" + strTemp);
    
    bool b = UserDefault::getInstance()->getBoolForKey("bool");
    if (b)
    {
        sprintf(strTemp, "bool is true");
        this->_label->setString(this->_label->getString() + "\n" + strTemp);
    }
    else
    {
        sprintf(strTemp, "bool is false");
        this->_label->setString(this->_label->getString() + "\n" + strTemp);
    }
}

void HelloWorld::menuCloseCallback(Ref* sender)
{
    Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif
}
