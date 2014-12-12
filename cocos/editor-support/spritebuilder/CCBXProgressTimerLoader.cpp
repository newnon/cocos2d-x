#include "CCBXProgressTimerLoader.h"
#include "2d/CCSprite.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_FLIP("flip");
static std::string PROPERTY_SPRITEFRAME("spriteFrame");
static std::string PROPERTY_BLENDFUNC("blendFunc");
static std::string PROPERTY_TYPE("type");
static std::string PROPERTY_MIDPOINT("midpoint");
static std::string PROPERTY_BARCHANGERATE("barChangeRate");
static std::string PROPERTY_PERCENTAGE("percentage");
static std::string PROPERTY_REVERSEDIR("reverseDirection");
    
ProgressTimerLoader *ProgressTimerLoader::create()
{
    ProgressTimerLoader *ret = new ProgressTimerLoader();
    ret->autorelease();
    return ret;
}

Node *ProgressTimerLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return ProgressTimer::create(_spriteFrame.spriteFrame?Sprite::createWithSpriteFrame(_spriteFrame.spriteFrame):Sprite::create());
}

void ProgressTimerLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ProgressTimer *progressTimer = static_cast<ProgressTimer*>(node);
    progressTimer->setBarChangeRate(_barChangeRate);
    progressTimer->setMidpoint(_midpoint);
    progressTimer->setType(_type);
    progressTimer->setPercentage(_percentage);
    progressTimer->getSprite()->setBlendFunc(_blendFunc);
}
    
ProgressTimerLoader:: ProgressTimerLoader()
    :_reverseDirection(false)
    ,_blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED)
{
    
}

ProgressTimerLoader::~ProgressTimerLoader()
{
    
}
    
void ProgressTimerLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
    if(propertyName == PROPERTY_BLENDFUNC) {
        _blendFunc = value;
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(propertyName, isExtraProp, value);
    }
}
    
void ProgressTimerLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_SPRITEFRAME) {
        _spriteFrame = value;
    } else {
        NodeLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void ProgressTimerLoader::onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &value)
{
    if(propertyName == PROPERTY_FLIP) {
        _flipped = value;
    } else {
        NodeLoader::onHandlePropTypeFlip(propertyName, isExtraProp, value);
    }
}
    
void ProgressTimerLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if (propertyName == PROPERTY_TYPE) {
        _type =  static_cast<ProgressTimer::Type>(value);
    } else {
        NodeLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}
    
void ProgressTimerLoader::onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const cocos2d::Point &value)
{
    if (propertyName == PROPERTY_MIDPOINT) {
        _midpoint = value;
    } else if (propertyName == PROPERTY_BARCHANGERATE) {
        _barChangeRate = value;
    } else {
        NodeLoader::onHandlePropTypePoint(propertyName, isExtraProp, value);
    }
}
    
void ProgressTimerLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if (propertyName == PROPERTY_PERCENTAGE) {
        _percentage = value;
    } else {
        NodeLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}

void ProgressTimerLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if (propertyName == PROPERTY_REVERSEDIR) {
        _reverseDirection = value;
    }
    else {
        NodeLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
    
/*void ProgressTimerLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _percentage = 0.0f;
}
    
void ProgressTimerLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    ((ProgressTimer *)pNode)->setPercentage(_percentage);
}

void ProgressTimerLoader::onHandlePropTypeSpriteFrame(Node * pNode, Node * pParent, const char * pPropertyName, SpriteFrame * pCCSpriteFrame, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_DISPLAYFRAME) == 0) {
        if(pCCSpriteFrame != NULL) {
            Sprite *temp=((ProgressTimer *)pNode)->getSprite();
            temp->retain();
            ((ProgressTimer *)pNode)->setSprite(NULL);
			temp->setSpriteFrame(pCCSpriteFrame);
            ((ProgressTimer *)pNode)->setSprite(temp);
            temp->release();
        } else {
            CCLOG("ERROR: SpriteFrame NULL");
        }
    } else {
        NodeLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeFlip(Node * pNode, Node * pParent, const char * pPropertyName, bool * pFlip, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FLIP) == 0) {
        ((ProgressTimer *)pNode)->getSprite()->setFlippedX(pFlip[0]);
        ((ProgressTimer *)pNode)->getSprite()->setFlippedY(pFlip[1]);
        ((ProgressTimer *)pNode)->setSprite(((ProgressTimer *)pNode)->getSprite());
    } else {
        NodeLoader::onHandlePropTypeFlip(pNode, pParent, pPropertyName, pFlip, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeBlendFunc(Node * pNode, Node * pParent, const char * pPropertyName, BlendFunc pCCBlendFunc, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        ((ProgressTimer *)pNode)->getSprite()->setBlendFunc(pCCBlendFunc);
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeIntegerLabeled(Node *pNode, Node *pParent, const char *pPropertyName, int pIntegerLabeled, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_TYPE) == 0) {
        ((ProgressTimer *)pNode)->setType((ProgressTimer::Type)pIntegerLabeled);
    } else {
        NodeLoader::onHandlePropTypeIntegerLabeled(pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypePoint(Node *pNode, Node *pParent, const char *pPropertyName, const Point &pPoint, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_MIDPOINT) == 0) {
        ((ProgressTimer *)pNode)->setMidpoint(pPoint);
    } else if (strcmp(pPropertyName, PROPERTY_BARCHANGERATE) == 0) {
        ((ProgressTimer *)pNode)->setBarChangeRate(pPoint);
    } else if (strcmp(pPropertyName, "anchorPoint") == 0) {
        ((ProgressTimer *)pNode)->setAnchorPoint(pPoint);
    }
    else {
        NodeLoader::onHandlePropTypePoint(pNode, pParent, pPropertyName, pPoint, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeFloat(Node *pNode, Node *pParent, const char *pPropertyName, float pFloat, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_PERCENTAGE) == 0) {
        _percentage = pFloat;
    } else {
        NodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeCheck(Node *pNode, Node *pParent, const char *pPropertyName, bool pCheck, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_REVERSEDIR) == 0) {
        ((ProgressTimer *)pNode)->setReverseDirection(pCheck);
    }
    else {
        NodeLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, pCCBReader);
    }
}*/

}

NS_CC_END
