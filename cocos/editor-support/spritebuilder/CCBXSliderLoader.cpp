#include "CCBXSliderLoader.h"
#include "ui/UISlider.h"

NS_CC_BEGIN
namespace spritebuilder {

static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_BACKGROUND_OLD("backgroundSpriteFrame|Normal");
static const std::string PROPERTY_BACKGROUND("backgroundSpriteFrame");
static const std::string PROPERTY_PROGRESS("progressSpriteFrame");
static const std::string PROPERTY_HANDLE_NORMAL("handleSpriteFrame|Normal");
static const std::string PROPERTY_HANDLE_HIGHLIGHTED("handleSpriteFrame|Highlighted");
static const std::string PROPERTY_HANDLE_DISABLED("handleSpriteFrame|Disabled");
static const std::string PROPERTY_HANDLE_MOUSEOVER("handleSpriteFrame|MouseOver");
    
static const std::string PROPERTY_MARGIN("margin");
static const std::string PROPERTY_MARGIN_LEFT("marginLeft");
static const std::string PROPERTY_MARGIN_TOP("marginTop");
static const std::string PROPERTY_MARGIN_RIGHT("marginRight");
static const std::string PROPERTY_MARGIN_BOTTOM("marginBottom");
    
static const std::string PROPERTY_ZOOM_SCALE("zoomScale");
static const std::string PROPERTY_MAX_PERCENT("maxPercent");
static const std::string PROPERTY_SLIDER_VALUE("percent");
    
static const std::string PROPERTY_IMAGE_SCALE("imageScale");
    
SliderLoader *SliderLoader::create()
{
    SliderLoader *ret = new SliderLoader();
    ret->autorelease();
    return ret;
}
Node *SliderLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    ui::Slider *slider = ui::Slider::create();
    slider->setAnchorPoint(Vec2(0.0f, 0.0f));
    return slider;

}
    
inline ui::Widget::TextureResType convertTextureResType(SpriteFrameDescription::TextureResType value)
{
    return static_cast<ui::Widget::TextureResType>(static_cast<int>(value) - 1);
}
    
void SliderLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    ui::Slider *slider = static_cast<ui::Slider*>(node);
    const Vec4 &margins = getNodeParamValue(params, PROPERTY_MARGIN, _margins);
    
    const SpriteFrameDescription &backgroundSpriteFrame = getNodeParamValue(params, PROPERTY_BACKGROUND, _backgroundSpriteFrame);
    const SpriteFrameDescription &progressSpriteFrame = getNodeParamValue(params, PROPERTY_PROGRESS, _progressSpriteFrame);
    const SpriteFrameDescription &handleNormalSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_NORMAL, _handleNormalSpriteFrame);
    const SpriteFrameDescription &handleDisabledSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_DISABLED, _handleDisabledSpriteFrame);
    const SpriteFrameDescription &handleHiglihtedSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_HIGHLIGHTED, _handleHiglihtedSpriteFrame);
    const SpriteFrameDescription &handleMouseOverSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_MOUSEOVER, _handleMouseOverSpriteFrame);
    
    slider->setScale9Enabled(margins != Vec4::ZERO);
    
    if(backgroundSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        slider->loadBarTexture(backgroundSpriteFrame.path, convertTextureResType(backgroundSpriteFrame.type));
        slider->setCapInsetsBarRenderer(calcMargins(margins, backgroundSpriteFrame.spriteFrame->getOriginalSize()));
    }
    
    if(progressSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        slider->loadProgressBarTexture(progressSpriteFrame.path, convertTextureResType(progressSpriteFrame.type));
        slider->setCapInsetProgressBarRenderer(calcMargins(margins, progressSpriteFrame.spriteFrame->getOriginalSize()));
    }
    
    if(handleNormalSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        slider->loadSlidBallTextureNormal(handleNormalSpriteFrame.path, convertTextureResType(handleNormalSpriteFrame.type));
    
    if(handleDisabledSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        slider->loadSlidBallTextureDisabled(handleDisabledSpriteFrame.path, convertTextureResType(handleDisabledSpriteFrame.type));
    
    if(handleHiglihtedSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        slider->loadSlidBallTexturePressed(handleHiglihtedSpriteFrame.path, convertTextureResType(handleHiglihtedSpriteFrame.type));
    
    if(handleMouseOverSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        slider->loadSlidBallTextureMouseOver(handleMouseOverSpriteFrame.path, convertTextureResType(handleMouseOverSpriteFrame.type));
    
    slider->setMaxPercent(getNodeParamValue(params, PROPERTY_MAX_PERCENT, _maxPercent));
    slider->setPercent(getNodeParamValue(params, PROPERTY_SLIDER_VALUE, _percent));
    slider->setZoomScale(getNodeParamValue(params, PROPERTY_ZOOM_SCALE, _zoomScale) - 1.0f);
    slider->setImageScale(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_IMAGE_SCALE, _imageScale)) / CCBXReader::getResolutionScale());
}

SliderLoader::SliderLoader()
    :_zoomScale(1.0f)
    ,_maxPercent(100)
    ,_percent(0)
    ,_imageScale{0,1.f}
{
    
}

SliderLoader::~SliderLoader()
{
    
}
    
void SliderLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_BACKGROUND || propertyName == PROPERTY_BACKGROUND_OLD) {
        _backgroundSpriteFrame = value;
    } else if(propertyName == PROPERTY_PROGRESS) {
        _progressSpriteFrame = value;
    } else if(propertyName == PROPERTY_HANDLE_NORMAL) {
        _handleNormalSpriteFrame = value;
    } else if(propertyName == PROPERTY_HANDLE_HIGHLIGHTED) {
        _handleHiglihtedSpriteFrame = value;
    } else if(propertyName == PROPERTY_HANDLE_DISABLED) {
        _handleDisabledSpriteFrame = value;
    } else if(propertyName == PROPERTY_HANDLE_MOUSEOVER) {
        _handleMouseOverSpriteFrame = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value)
{
    if(propertyName == PROPERTY_MARGIN) {
        _margins = value;
    } else {
        NodeLoader::onHandlePropTypeOffsets(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.z = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.w = value;
    } else if(propertyName == PROPERTY_ZOOM_SCALE) {
        _zoomScale = value;
    } else{
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_IMAGE_SCALE) {
        _imageScale = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_MAX_PERCENT) {
        _maxPercent = value;
    } else if(propertyName == PROPERTY_SLIDER_VALUE) {
        _percent = value;
    } else{
        WidgetLoader::onHandlePropTypeInteger(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_DIRECTION) {
        //possible add support for direction
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END
