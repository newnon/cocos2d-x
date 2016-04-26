#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "network/WebSocket.h"

class HelloWorld : public cocos2d::Layer, public cocos2d::network::WebSocket::Delegate
{
public:
    virtual bool init() override;

    static cocos2d::Scene* scene();

    // a selector callback
    void menuCloseCallback(Ref* sender);
    
    virtual void onOpen(cocos2d::network::WebSocket* ws) override;
    virtual void onMessage(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::Data& data) override;
    virtual void onClose(cocos2d::network::WebSocket* ws) override;
    virtual void onError(cocos2d::network::WebSocket* ws, const cocos2d::network::WebSocket::ErrorCode& error) override;

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);
    
    cocos2d::Label *_label = nullptr;
    cocos2d::network::WebSocket *ws = nullptr;
};

#endif // __HELLOWORLD_SCENE_H__
