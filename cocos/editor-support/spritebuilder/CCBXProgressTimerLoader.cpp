#include "CCBXProgressTimerLoader.h"
#include "2d/CCSprite.h"
#include "2d/CCProgressTimer.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_FLIP("flip");
static const std::string PROPERTY_SPRITEFRAME("spriteFrame");
static const std::string PROPERTY_BLENDFUNC("blendFunc");
static const std::string PROPERTY_TYPE("type");
static const std::string PROPERTY_MIDPOINT("midpoint");
static const std::string PROPERTY_BARCHANGERATE("barChangeRate");
static const std::string PROPERTY_PERCENTAGE("percentage");
static const std::string PROPERTY_REVERSEDIR("reverseDirection");
    
ProgressTimerLoader *ProgressTimerLoader::create()
{
    ProgressTimerLoader *ret = new ProgressTimerLoader();
    ret->autorelease();
    return ret;
}

Node *ProgressTimerLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    const SpriteFrameDescription &spriteFrame = getNodeParamValue(params, PROPERTY_SPRITEFRAME, _spriteFrame);
    ProgressTimer *progressTimer = ProgressTimer::create(spriteFrame.spriteFrame ? Sprite::createWithSpriteFrame(spriteFrame.spriteFrame) : Sprite::create());
    progressTimer->setAnchorPoint(Vec2(0.0f, 0.0f));
    return progressTimer;
}

void ProgressTimerLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    ProgressTimer *progressTimer = static_cast<ProgressTimer*>(node);
    progressTimer->setBarChangeRate(getNodeParamValue(params, PROPERTY_BARCHANGERATE, _barChangeRate));
    progressTimer->setMidpoint(getNodeParamValue(params, PROPERTY_MIDPOINT, _midpoint));
    progressTimer->setType(static_cast<ProgressTimer::Type>(getNodeParamValue(params, PROPERTY_TYPE, _type)));
    progressTimer->setPercentage(getNodeParamValue(params, PROPERTY_PERCENTAGE, _percentage));
    progressTimer->getSprite()->setBlendFunc(getNodeParamValue(params, PROPERTY_BLENDFUNC, _blendFunc));
}
    
ProgressTimerLoader:: ProgressTimerLoader()
    :_reverseDirection(false)
    ,_blendFunc(BlendFunc::ALPHA_NON_PREMULTIPLIED)
    ,_type(0)
    ,_percentage(0)
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
        _type = value;
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

}

NS_CC_END
