//
//  SpriteBuilder.cpp
//  cocos2d_libs
//
//  Created by Sergey on 28.10.14.
//
//

#include "SpriteBuilder.h"
#include <set>

#include "CCBXParser.h"
#include "CCBXNodeLoaderLibrary.h"
#include "CCBXNodeLoader.h"
#include "base/CCDirector.h"
#include "CCBXReaderParams.h"

NS_CC_BEGIN

namespace spritebuilder {
    
float CCBXReader::_resolutionScale = 1.0f;
Map<std::string,CCBReaderParams*> CCBXReader::_paramsMap;
    
CCBXReader* CCBXReader::createFromFile(const std::string &pCCBFileName, const std::string &rootPath, const NodeLoaderLibrary *library)
{
    CCBReaderParams* params = createParams(rootPath);
    if(!params)
        return nullptr;
    NodeLoader * loader = ParseCCBXFile(pCCBFileName, library?library:NodeLoaderLibrary::getDefault(), rootPath, params);
    if(!loader)
        return nullptr;
    CCBXReader *ret = new CCBXReader(rootPath, params, loader);
    ret->autorelease();
    return ret;
}
    
CCBXReader* CCBXReader::createFromData(const Data &data, const std::string &rootPath, const NodeLoaderLibrary *library)
{
    CCBReaderParams* params = createParams(rootPath);
    if(!params)
        return nullptr;
    NodeLoader * loader = ParseCCBXData(data, library?library:NodeLoaderLibrary::getDefault(), rootPath, params);
    if(!loader)
        return nullptr;
    CCBXReader *ret = new CCBXReader(rootPath, params, loader);
    ret->autorelease();
    return ret;
}
    
void CCBXReader::calcScales(SceneScaleType scaleType, float &mainScale, float &additionalScale) const
{
    const Size &designResolution = _params->getDesignResolution();
    float designScale = _params->getDesignResolutionScale();
    if(scaleType == SceneScaleType::NONE)
    {
        mainScale = 1.0f;
        additionalScale = 1.0f;
    }
    else if(scaleType == SceneScaleType::CUSTOM)
    {
        //do nothing
    }
    else
    {
        float resolutionAspectX = Director::getInstance()->getWinSize().width / getResolutionScale();
        float resolutionAspectY = Director::getInstance()->getWinSize().height / getResolutionScale();
        
        float designAspectX = designResolution.width / designScale;
        float designAspectY = designResolution.height / designScale;
        
        if(scaleType == SceneScaleType::MINSCALE)
        {
            float mainScale1 = resolutionAspectX / designAspectX;
            float mainScale2 = resolutionAspectY / designAspectY;
            if(mainScale1<mainScale2)
            {
                mainScale = mainScale1;
                additionalScale = mainScale2/mainScale1;
            }
            else
            {
                mainScale = mainScale2;
                additionalScale = mainScale1/mainScale2;
            }
        }
        else if(scaleType == SceneScaleType::MAXSCALE)
        {
            float mainScale1 = resolutionAspectX / designAspectX;
            float mainScale2 = resolutionAspectY / designAspectY;
            if(mainScale1>mainScale2)
            {
                mainScale = mainScale1;
                additionalScale = mainScale2/mainScale1;
            }
            else
            {
                mainScale = mainScale2;
                additionalScale = mainScale1/mainScale2;
            }
        }
        
        else if((designResolution.width>designResolution.height)==(scaleType == SceneScaleType::MINSIZE))
        {
            mainScale = resolutionAspectY / designAspectY;
            additionalScale = (resolutionAspectX/mainScale)/designAspectX;
        }
        else
        {
            mainScale = resolutionAspectX / designAspectX;
            additionalScale = (resolutionAspectY/mainScale)/designAspectY;
        }
    }
}

    
Node *CCBXReader::createNode(CCBXReaderOwner *pOwner, SceneScaleType scaleType, const CreateNodeFunction &createNodeFunction, const std::function<void(cocos2d::Node*, AnimationCompleteType)> &defaultAnimationCallback) const
{
    float mainScale = 1.0f;
    float additionalScale = 1.0f;
    calcScales(scaleType, mainScale, additionalScale);
    Node * ret = _rootNodeLoader->createNode(Director::getInstance()->getWinSize(), mainScale, additionalScale, pOwner, nullptr, nullptr, createNodeFunction);
    return ret;
}

float CCBXReader::getResolutionScale()
{
    return _resolutionScale;
}
void CCBXReader::setResolutionScale(float scale)
{
    _resolutionScale = scale;
}
    
const std::string& CCBXReader::getRootPath()
{
    return _rootPath;
}
    
CCBReaderParams* CCBXReader::getParams() const
{
    return _params;
}
    
CCBReaderParams* CCBXReader::createParams(const std::string &rootPath)
{
    auto it = _paramsMap.find(rootPath);
    if(it != _paramsMap.end())
        return it->second;
    CCBReaderParams* ret = CCBReaderParams::create(rootPath);
    if(ret)
    {
        _paramsMap.insert(rootPath, ret);
    }
    return ret;
}

CCBXReader::CCBXReader(const std::string &rootPath, CCBReaderParams *params, NodeLoader *rootNodeLoader)
    :_rootPath(rootPath)
    ,_params(params)
    ,_rootNodeLoader(rootNodeLoader)
{
    _rootNodeLoader->retain();
}
CCBXReader::~CCBXReader()
{
    _rootNodeLoader->release();
}

}

NS_CC_END