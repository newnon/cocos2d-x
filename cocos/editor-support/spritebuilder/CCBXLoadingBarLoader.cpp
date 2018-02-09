#include "CCBXLoadingBarLoader.h"
#include "ui/UILoadingBar.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_FLIP("flip");
static const std::string PROPERTY_SPRITEFRAME("spriteFrame");
static const std::string PROPERTY_BLENDFUNC("blendFunc");
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_PERCENTAGE("percentage");
    
static const std::string PROPERTY_MARGIN("margin");
static const std::string PROPERTY_MARGIN_LEFT("marginLeft");
static const std::string PROPERTY_MARGIN_TOP("marginTop");
static const std::string PROPERTY_MARGIN_RIGHT("marginRight");
static const std::string PROPERTY_MARGIN_BOTTOM("marginBottom");
    
static const std::string PROPERTY_IMAGESCALE("imageScale");
    
LoadingBarLoader *LoadingBarLoader::create()
{
    LoadingBarLoader *ret = new LoadingBarLoader();
    ret->autorelease();
    return ret;
}

Node *LoadingBarLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    ui::LoadingBar *loadingBar = ui::LoadingBar::create();
    loadingBar->setAnchorPoint(Vec2(0.0f, 0.0f));
    return loadingBar;
}
    
inline ui::Widget::TextureResType convertTextureResType(SpriteFrameDescription::TextureResType value)
{
    return static_cast<ui::Widget::TextureResType>(static_cast<int>(value) - 1);
}

void LoadingBarLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    ui::LoadingBar *loadingBar = static_cast<ui::LoadingBar*>(node);
    const Vec4 &margins = getNodeParamValue(params, PROPERTY_MARGIN, _margins);
    const SpriteFrameDescription &spriteFrame = getNodeParamValue(params, PROPERTY_SPRITEFRAME, _spriteFrame);
    loadingBar->setScale9Enabled(true);
    if(spriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        loadingBar->loadTexture(spriteFrame.path, convertTextureResType(spriteFrame.type));
        loadingBar->setCapInsets(calcMargins(margins, spriteFrame.spriteFrame->getOriginalSize()));
    }
    loadingBar->setImageScale(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_IMAGESCALE, _imageScale)) / CCBXReader::getResolutionScale());
    loadingBar->setPercent(getNodeParamValue(params, PROPERTY_PERCENTAGE, _percentage));
    loadingBar->setDirection(static_cast<ui::LoadingBar::Direction>(getNodeParamValue(params, PROPERTY_DIRECTION, _direction)));
}
    
LoadingBarLoader::LoadingBarLoader()
    :_direction((int)ui::LoadingBar::Direction::LEFT)
    ,_percentage(0)
    ,_imageScale{0,1.f}
{
    
}

LoadingBarLoader::~LoadingBarLoader()
{
    
}
    
void LoadingBarLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
        
}
    
void LoadingBarLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_SPRITEFRAME) {
        _spriteFrame = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void LoadingBarLoader::onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &value)
{
    if(propertyName == PROPERTY_FLIP) {
        _flipped = value;
    } else {
        WidgetLoader::onHandlePropTypeFlip(propertyName, isExtraProp, value);
    }
}
    
void LoadingBarLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if (propertyName == PROPERTY_DIRECTION) {
        _direction = value;
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}
    
void LoadingBarLoader::onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value)
{
    if(propertyName == PROPERTY_MARGIN) {
        _margins = value;
    } else {
        NodeLoader::onHandlePropTypeOffsets(propertyName, isExtraProp, value);
    }
}

void LoadingBarLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.z = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.w = value;
    } else if (propertyName == PROPERTY_PERCENTAGE) {
        _percentage = value;
    } else {
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}
    
void LoadingBarLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_IMAGESCALE) {
        _imageScale = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END
