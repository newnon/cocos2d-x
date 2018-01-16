#include "CCBXScrollBarLoader.h"
#include "ui/UIScrollBar.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_SPACING("spacing");
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_BACKGROUND_OLD("backgroundSpriteFrame|Normal");
static const std::string PROPERTY_BACKGROUND("backgroundSpriteFrame");
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
static const std::string PROPERTY_IS_VERTICAL("vertical");
    
static const std::string PROPERTY_HANDLE_MARGIN("handleMargin");
    
ScrollBarLoader *ScrollBarLoader::create()
{
    ScrollBarLoader *ret = new ScrollBarLoader();
    ret->autorelease();
    return ret;
}
Node *ScrollBarLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    ui::ScrollBar *scrollBar = ui::ScrollBar::create();
    scrollBar->setAnchorPoint(Vec2(0.0f, 0.0f));
    return scrollBar;
}
    
inline ui::Widget::TextureResType convertTextureResType(SpriteFrameDescription::TextureResType value)
{
    return static_cast<ui::Widget::TextureResType>(static_cast<int>(value) - 1);
}
    
void ScrollBarLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    ui::ScrollBar *scrollBar = static_cast<ui::ScrollBar*>(node);
    scrollBar->setBarType(getNodeParamValue(params, PROPERTY_IS_VERTICAL, _isVertical) ? cocos2d::ui::ScrollBar::BarType::kVertical : cocos2d::ui::ScrollBar::BarType::kHorizontal);
    const Vec4 &margins = getNodeParamValue(params, PROPERTY_MARGIN, _margins);
    const Vec4 &handleMargins = getNodeParamValue(params, PROPERTY_HANDLE_MARGIN, _handleMargins);
    
    const SpriteFrameDescription &backgroundSpriteFrame = getNodeParamValue(params, PROPERTY_BACKGROUND, _backgroundSpriteFrame);
    const SpriteFrameDescription &handleNormalSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_NORMAL, _handleNormalSpriteFrame);
    const SpriteFrameDescription &handleDisabledSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_DISABLED, _handleDisabledSpriteFrame);
    const SpriteFrameDescription &handleHiglihtedSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_HIGHLIGHTED, _handleHiglihtedSpriteFrame);
    const SpriteFrameDescription &handleMouseOverSpriteFrame = getNodeParamValue(params, PROPERTY_HANDLE_MOUSEOVER, _handleMouseOverSpriteFrame);
    
    if(backgroundSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        scrollBar->loadBarTexture(backgroundSpriteFrame.path, convertTextureResType(backgroundSpriteFrame.type));
        scrollBar->setCapInsetsBarRenderer(calcMargins(margins, backgroundSpriteFrame.spriteFrame->getOriginalSize()));
        scrollBar->setScale9Enabled(margins != Vec4::ZERO);
    }
    
    if(handleNormalSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        scrollBar->setCapInsetsForBallRendereds(calcMargins(handleMargins, handleNormalSpriteFrame.spriteFrame->getOriginalSize()));
        scrollBar->loadSlidBallTextureNormal(handleNormalSpriteFrame.path, convertTextureResType(handleNormalSpriteFrame.type));
    }
    
    if(handleDisabledSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        scrollBar->loadSlidBallTextureDisabled(handleDisabledSpriteFrame.path, convertTextureResType(handleDisabledSpriteFrame.type));
    
    if(handleHiglihtedSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        scrollBar->loadSlidBallTexturePressed(handleHiglihtedSpriteFrame.path, convertTextureResType(handleHiglihtedSpriteFrame.type));
    
    if(handleMouseOverSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
        scrollBar->loadSlidBallTextureMouseOver(handleMouseOverSpriteFrame.path, convertTextureResType(handleMouseOverSpriteFrame.type));

    scrollBar->setPercent(getNodeParamValue(params, PROPERTY_SLIDER_VALUE, _percent));
    scrollBar->setZoomScale(getNodeParamValue(params, PROPERTY_ZOOM_SCALE, _zoomScale) - 1.0f);
    scrollBar->setImageScale(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_IMAGE_SCALE, _imageScale)) / CCBXReader::getResolutionScale());
}

ScrollBarLoader::ScrollBarLoader()
    :_zoomScale(1.0f)
    ,_percent(0.f)
    ,_imageScale{0, 1.f}
    ,_isVertical(false)
{
    
}

ScrollBarLoader::~ScrollBarLoader()
{
    
}
    
void ScrollBarLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_BACKGROUND || propertyName == PROPERTY_BACKGROUND_OLD) {
        _backgroundSpriteFrame = value;
    } else if(propertyName == PROPERTY_HANDLE_NORMAL) {
        _handleNormalSpriteFrame= value;
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
    
void ScrollBarLoader::onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value)
{
    if(propertyName == PROPERTY_MARGIN) {
        _margins = value;
    }
    else if (PROPERTY_HANDLE_MARGIN == propertyName) {
        _handleMargins = value;
    }
    else {
        NodeLoader::onHandlePropTypeOffsets(propertyName, isExtraProp, value);
    }
}
    
void ScrollBarLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_IS_VERTICAL) {
        _isVertical = value;
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
void ScrollBarLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
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
    } else if(propertyName == PROPERTY_SLIDER_VALUE) {
        _percent = value;
    } else{
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}
    
void ScrollBarLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_IMAGE_SCALE) {
        _imageScale = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END
