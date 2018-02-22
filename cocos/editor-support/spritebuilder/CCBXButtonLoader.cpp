#include "CCBXButtonLoader.h"
#include "2d/CCLabel.h"
#include "ui/UIButton.h"

NS_CC_BEGIN
namespace spritebuilder {

static const std::string PROPERTY_ZOOMONCLICK("zoomOnClick");
static const std::string PROPERTY_LABELANCHORPOINT("labelAnchorPoint");
static const std::string PROPERTY_PREFERREDSIZE("preferredSize");
static const std::string PROPERTY_CONTENTSIZE("contentSize");
static const std::string PROPERTY_MAXSIZE("maxSize");
    
static const std::string PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL("backgroundSpriteFrame|Normal");
static const std::string PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED("backgroundSpriteFrame|Highlighted");
static const std::string PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED("backgroundSpriteFrame|Disabled");
static const std::string PROPERTY_BACKGROUNDSPRITEFRAME_MOUSEOVER("backgroundSpriteFrame|MouseOver");
    
static const std::string PROPERTY_BACKGROUNDOPACITY_NORMAL("backgroundOpacity|Normal");
static const std::string PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED("backgroundOpacity|Highlighted");
static const std::string PROPERTY_BACKGROUNDOPACITY_DISABLED("backgroundOpacity|Disabled");
static const std::string PROPERTY_BACKGROUNDOPACITY_MOUSEOVER("backgroundOpacity|MouseOver");
    
static const std::string PROPERTY_LABELOPACITY_NORMAL("labelOpacity|Normal");
static const std::string PROPERTY_LABELOPACITY_HIGHLIGHTED("labelOpacity|Highlighted");
static const std::string PROPERTY_LABELOPACITY_DISABLED("labelOpacity|Disabled");
static const std::string PROPERTY_LABELOPACITY_MOUSEOVER("labelOpacity|MouseOver");
    
static const std::string PROPERTY_TITLE("title");
    
static const std::string PROPERTY_TITLECOLOR_NORMAL("labelColor|Normal");
static const std::string PROPERTY_TITLECOLOR_HIGHLIGHTED("labelColor|Highlighted");
static const std::string PROPERTY_TITLECOLOR_DISABLED("labelColor|Disabled");
static const std::string PROPERTY_TITLECOLOR_MOUSEOVER("labelColor|MouseOver");
    
static const std::string PROPERTY_BACKGROUNDCOLOR_NORMAL("backgroundColor|Normal");
static const std::string PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED("backgroundColor|Highlighted");
static const std::string PROPERTY_BACKGROUNDCOLOR_DISABLED("backgroundColor|Disabled");
static const std::string PROPERTY_BACKGROUNDCOLOR_MOUSEOVER("backgroundColor|MouseOver");
    
static const std::string PROPERTY_HORIZONTALPADDING("horizontalPadding");
static const std::string PROPERTY_VERTICALPADDING("verticalPadding");
    
static const std::string PROPERTY_LEFTPADDING("leftPadding");
static const std::string PROPERTY_RIGHTPADDING("rightPadding");
static const std::string PROPERTY_TOPPADDING("topPadding");
static const std::string PROPERTY_BOTTOMPADDING("bottomPadding");
    
static const std::string PROPERTY_HORIZONTALALIGMENT("horizontalAlignment");
static const std::string PROPERTY_VERTICALALIGMENT("verticalAlignment");
    
static const std::string PROPERTY_FONTNAME("fontName");
static const std::string PROPERTY_FONTSIZE("fontSize");
    
static const std::string PROPERTY_ADJUSTS_FONT_SIZE_TO_FIT("adjustsFontSizeToFit");
    
static const std::string PROPERTY_FONTCOLOR("fontColor");
static const std::string PROPERTY_OUTLINECOLOR("outlineColor");
static const std::string PROPERTY_OUTLINEWIDTH("outlineWidth");
    
static const std::string PROPERTY_SHADOWBLURRADIUS("shadowBlurRadius");
static const std::string PROPERTY_SHADOWWIDTH("shadowWidth");
static const std::string PROPERTY_SHADOWCOLOR("shadowColor");
    
static const std::string PROPERTY_SHADOWOFFSET("shadowOffset");
    
static const std::string PROPERTY_MARGIN("margin");
static const std::string PROPERTY_OFFSET("offset");
    
static const std::string PROPERTY_MARGIN_LEFT("marginLeft");
static const std::string PROPERTY_MARGIN_TOP("marginTop");
static const std::string PROPERTY_MARGIN_RIGHT("marginRight");
static const std::string PROPERTY_MARGIN_BOTTOM("marginBottom");
    
static const std::string PROPERTY_OFFSET_LEFT("offsetLeft");
static const std::string PROPERTY_OFFSET_TOP("offsetTop");
static const std::string PROPERTY_OFFSET_RIGHT("offsetRight");
static const std::string PROPERTY_OFFSET_BOTTOM("offsetBottom");
    
static const std::string PROPERTY_IMAGESCALE("imageScale");
static const std::string PROPERTY_OVERFLOW("overflowType");
static const std::string PROPERTY_WORDWRAP("wordWrap");
    
ButtonLoader *ButtonLoader::create()
{
    ButtonLoader *ret = new ButtonLoader();
    ret->autorelease();
    return ret;
}

Node *ButtonLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    ui::Button *button = ui::Button::create();
    button->setAnchorPoint(Vec2(0.0f, 0.0f));
    return button;
}
    
inline ui::Widget::TextureResType convertTextureResType(SpriteFrameDescription::TextureResType value)
{
    return static_cast<ui::Widget::TextureResType>(static_cast<int>(value) - 1);
}

void ButtonLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    ui::Button *button = static_cast<ui::Button*>(node);
    
    const Vec4 &margins = getNodeParamValue(params, PROPERTY_MARGIN, _margins);
    
    const SpriteFrameDescription &normalSpriteFrame = getNodeParamValue(params, PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL, _normalSpriteFrame);
    if(normalSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        button->loadTextureNormal(normalSpriteFrame.path, convertTextureResType(normalSpriteFrame.type));
        button->setCapInsetsNormalRenderer(calcMargins(margins, normalSpriteFrame.spriteFrame->getOriginalSize()));
    }
    const SpriteFrameDescription &selectedSpriteFrame = getNodeParamValue(params, PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED, _selectedSpriteFrame);
    if(selectedSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        button->loadTexturePressed(selectedSpriteFrame.path, convertTextureResType(selectedSpriteFrame.type));
        button->setCapInsetsPressedRenderer(calcMargins(margins, selectedSpriteFrame.spriteFrame->getOriginalSize()));
    }
    const SpriteFrameDescription &disabledSpriteFrame = getNodeParamValue(params, PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED, _disabledSpriteFrame);
    if(disabledSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        button->loadTextureDisabled(disabledSpriteFrame.path, convertTextureResType(disabledSpriteFrame.type));
        button->setCapInsetsDisabledRenderer(calcMargins(margins, disabledSpriteFrame.spriteFrame->getOriginalSize()));
    }
    const SpriteFrameDescription &mouseOverSpriteFrame = getNodeParamValue(params, PROPERTY_BACKGROUNDSPRITEFRAME_MOUSEOVER, _mouseOverSpriteFrame);
    if(mouseOverSpriteFrame.type != SpriteFrameDescription::TextureResType::NONE)
    {
        button->loadTextureMouseOver(mouseOverSpriteFrame.path, convertTextureResType(mouseOverSpriteFrame.type));
        button->setCapInsetsMouseOverRenderer(calcMargins(margins, mouseOverSpriteFrame.spriteFrame->getOriginalSize()));
    }
    
    button->setScale9Enabled(margins != Vec4::ZERO);
    button->setImageScale(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_IMAGESCALE, _imageScale)) / CCBXReader::getResolutionScale());
    button->setPressedActionEnabled(true);
    button->setZoomScale(getNodeParamValue(params, PROPERTY_ZOOMONCLICK, _zoomOnClick) - 1.0);
    
    button->setNormalTitleColor(getNodeParamValue(params, PROPERTY_TITLECOLOR_NORMAL, _normalLabelColor));
    button->setNormalBackgroundColor(getNodeParamValue(params, PROPERTY_BACKGROUNDCOLOR_NORMAL, _normalBackgroundColor));
    button->setNormalTitleOpacity(getNodeOpacityParamValue(params, PROPERTY_LABELOPACITY_NORMAL, _normalLabelOpacity));
    button->setNormalBackgroundOpacity(getNodeOpacityParamValue(params, PROPERTY_BACKGROUNDOPACITY_NORMAL, _normalBackgroundOpacity));
    
    button->setPressedTitleColor(getNodeParamValue(params, PROPERTY_TITLECOLOR_HIGHLIGHTED, _selectedLabelColor));
    button->setPressedBackgroundColor(getNodeParamValue(params, PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED, _selectedBackgroundColor));
    button->setPressedTitleOpacity(getNodeOpacityParamValue(params, PROPERTY_LABELOPACITY_HIGHLIGHTED, _selectedLabelOpacity));
    button->setPressedBackgroundOpacity(getNodeOpacityParamValue(params, PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED, _selectedBackgroundOpacity));
    
    button->setDisabledTitleColor(getNodeParamValue(params, PROPERTY_TITLECOLOR_DISABLED, _disabledLabelColor));
    button->setDisabledBackgroundColor(getNodeParamValue(params, PROPERTY_BACKGROUNDCOLOR_DISABLED, _disabledBackgroundColor));
    button->setDisabledTitleOpacity(getNodeOpacityParamValue(params, PROPERTY_LABELOPACITY_DISABLED, _disabledLabelOpacity));
    button->setDisabledBackgroundOpacity(getNodeOpacityParamValue(params, PROPERTY_BACKGROUNDOPACITY_DISABLED, _disabledBackgroundOpacity));
    
    button->setMouseOverTitleColor(getNodeParamValue(params, PROPERTY_TITLECOLOR_MOUSEOVER, _mouseOverLabelColor));
    button->setMouseOverBackgroundColor(getNodeParamValue(params, PROPERTY_BACKGROUNDCOLOR_MOUSEOVER, _mouseOverBackgroundColor));
    button->setMouseOverTitleOpacity(getNodeOpacityParamValue(params, PROPERTY_LABELOPACITY_MOUSEOVER, _mouseOverLabelOpacity));
    button->setMouseOverBackgroundOpacity(getNodeOpacityParamValue(params, PROPERTY_BACKGROUNDOPACITY_MOUSEOVER, _mouseOverBackgroundOpacity));
    
    button->setTitleFontName(getNodeParamValue(params, PROPERTY_FONTNAME, _font));
    //button->getTitleRenderer()->setSystemFontName(_font);
    button->setTitleFontSize(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_FONTSIZE, _fontSize)));
    button->setTitleAlignment(static_cast<TextHAlignment>(getNodeParamValue(params, PROPERTY_HORIZONTALALIGMENT, _textHAlignment)), static_cast<TextVAlignment>(getNodeParamValue(params, PROPERTY_VERTICALALIGMENT, _textVAlignment)));
    
    button->setPadding(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_LEFTPADDING, _leftPadding)),
                       getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_TOPPADDING, _topPadding)),
                       getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_RIGHTPADDING, _rightPadding)),
                       getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_BOTTOMPADDING, _bottomPadding)));
    
    const Vec4 &offset = getNodeParamValue(params, PROPERTY_OFFSET, _offset);
    button->setOffsets(offset.x, offset.y, offset.z, offset.w);
    button->setTitleText(getNodeParamValue(params, PROPERTY_TITLE, _label));
    
    Label::Overflow overflow = static_cast<Label::Overflow>(_overflowLabel);
    button->setOverflow((getNodeParamValue(params, PROPERTY_ADJUSTS_FONT_SIZE_TO_FIT, _adjustsFontSizeToFit) && overflow == Label::Overflow::NONE) ? static_cast<int>(Label::Overflow::SHRINK) : getNodeParamValue(params, PROPERTY_OVERFLOW, _overflowLabel));
    button->setLabelWordWrap(getNodeParamValue(params, PROPERTY_WORDWRAP, _wordWrapLabel));
    
    float outlineWidth = getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_OUTLINEWIDTH, _outlineWidth));
    float shadowBlurRadius = getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_SHADOWBLURRADIUS, _shadowBlurRadius));
    Vec2 shadowOffset = getAbsolutePosition(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_SHADOWOFFSET, _shadowOffset), parentSize);

    Label* titleRenderer = button->getTitleRenderer();
    if(titleRenderer)
    {
        const cocos2d::Color4B &outlineColor = getNodeParamValue(params, PROPERTY_OUTLINECOLOR, _outlineColor);
        if (outlineColor.a > 0 && outlineWidth > 0)
            titleRenderer->enableOutline(outlineColor, outlineWidth);
        const cocos2d::Color4B &shadowColor = getNodeParamValue(params, PROPERTY_SHADOWCOLOR, _shadowColor);
        if (shadowColor.a > 0)
            titleRenderer->enableShadow(shadowColor, Size(shadowOffset.x, shadowOffset.y), shadowBlurRadius);
    }
}

ButtonLoader::ButtonLoader()
    :_zoomOnClick(1.0f)
    ,_outlineColor(0,0,0,0)
    ,_shadowColor(0,0,0,127)
    ,_outlineWidth(FloatScaleDescription{0, 0.0f})
    ,_shadowBlurRadius(FloatScaleDescription{0, 0.0f})
    ,_shadowOffset(PositionDescription{PositionReferenceCorner::BOTTOMLEFT, PositionUnit::POINTS, PositionUnit::POINTS, Vec2(0, 0)})
    ,_normalLabelColor(Color3B::WHITE)
    ,_normalBackgroundColor(Color3B::WHITE)
    ,_normalLabelOpacity(255)
    ,_normalBackgroundOpacity(255)
    ,_selectedLabelColor(Color3B::WHITE)
    ,_selectedBackgroundColor(Color3B::WHITE)
    ,_selectedLabelOpacity(255)
    ,_selectedBackgroundOpacity(255)
    ,_disabledLabelColor(Color3B::WHITE)
    ,_disabledBackgroundColor(Color3B::WHITE)
    ,_disabledLabelOpacity(255)
    ,_disabledBackgroundOpacity(127)
    ,_mouseOverLabelColor(Color3B::WHITE)
    ,_mouseOverBackgroundColor(Color3B::WHITE)
    ,_mouseOverLabelOpacity(255)
    ,_mouseOverBackgroundOpacity(255)
    ,_textHAlignment((int)TextHAlignment::LEFT)
    ,_textVAlignment((int)TextVAlignment::TOP)
    ,_leftPadding(FloatScaleDescription{0, 0.0f})
    ,_rightPadding(FloatScaleDescription{0, 0.0f})
    ,_topPadding(FloatScaleDescription{0, 0.0f})
    ,_bottomPadding(FloatScaleDescription{0, 0.0f})
    ,_adjustsFontSizeToFit(false)
    ,_imageScale{0,1.f}
    ,_overflowLabel(static_cast<int>(cocos2d::Label::Overflow::NONE))
    ,_wordWrapLabel(true)
{
    
}

ButtonLoader::~ButtonLoader()
{
    
}

void ButtonLoader::onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_TITLE) {
        _label = value;
    } else {
        WidgetLoader::onHandlePropTypeText(propertyName, isExtraProp, value);
    }
}
    
void ButtonLoader::onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_FONTNAME) {
        _font = value;
    } else {
        WidgetLoader::onHandlePropTypeFontTTF(propertyName, isExtraProp, value);
    }
}
    
void ButtonLoader::onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value)
{
    if(propertyName == PROPERTY_MARGIN) {
        _margins = value;
    } else if(propertyName == PROPERTY_OFFSET) {
        _offset = value;
    } else {
        WidgetLoader::onHandlePropTypeOffsets(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.z = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.w = value;
    } else if(propertyName == PROPERTY_OFFSET_LEFT) {
        _offset.x = value;
    } else if(propertyName == PROPERTY_OFFSET_TOP) {
        _offset.y = value;
    } else if(propertyName == PROPERTY_OFFSET_RIGHT) {
        _offset.z = value;
    } else if(propertyName == PROPERTY_OFFSET_BOTTOM) {
        _offset.w = value;
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_NORMAL) {
        float opacity = value * 255.0f;
        _normalBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED) {
        float opacity = value * 255.0f;
        _selectedBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_DISABLED) {
        float opacity = value * 255.0f;
        _disabledBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_MOUSEOVER) {
        float opacity = value * 255.0f;
        _mouseOverBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_NORMAL) {
        float opacity = value * 255.0f;
        _normalLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_HIGHLIGHTED) {
        float opacity = value * 255.0f;
        _selectedLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_DISABLED) {
        float opacity = value * 255.0f;
        _disabledLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_MOUSEOVER) {
        float opacity = value * 255.0f;
        _mouseOverLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_ZOOMONCLICK) {
        _zoomOnClick = value;
    } else {
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_FONTSIZE) {
        _fontSize = value;
    } else if(propertyName == PROPERTY_HORIZONTALPADDING) {
        _leftPadding = value;
        _rightPadding = value;
    } else if(propertyName == PROPERTY_VERTICALPADDING) {
        _topPadding = value;
        _bottomPadding = value;
    } else  if(propertyName == PROPERTY_LEFTPADDING) {
        _leftPadding = value;
    } else if(propertyName == PROPERTY_TOPPADDING) {
        _topPadding = value;
    } else  if(propertyName == PROPERTY_RIGHTPADDING) {
        _rightPadding = value;
    } else if(propertyName == PROPERTY_BOTTOMPADDING) {
        _bottomPadding = value;
    } else if(propertyName == PROPERTY_OUTLINEWIDTH) {
        _outlineWidth = value;
    } else if(propertyName == PROPERTY_SHADOWBLURRADIUS) {
        _shadowBlurRadius = value;
    } else if(propertyName == PROPERTY_IMAGESCALE) {
        _imageScale = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    if(propertyName == PROPERTY_PREFERREDSIZE) {
        //WidgetLoader::onHandlePropTypeSize(PROPERTY_CONTENTSIZE, isExtraProp, value);
    } else if(propertyName == PROPERTY_MAXSIZE) {
        //((ButtonControl *)pNode)->setMaxSize(pSize);
    } else {
        WidgetLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL) {
        _normalSpriteFrame = value;
    } else if(propertyName == PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED) {
        _selectedSpriteFrame = value;
    } else if(propertyName == PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED) {
        _disabledSpriteFrame = value;
    } else if(propertyName == PROPERTY_BACKGROUNDSPRITEFRAME_MOUSEOVER) {
        _mouseOverSpriteFrame = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value)
{
    if(propertyName == PROPERTY_TITLECOLOR_NORMAL) {
        _normalLabelColor = value;
    } else if(propertyName == PROPERTY_TITLECOLOR_HIGHLIGHTED) {
        _selectedLabelColor = value;
    } else if(propertyName == PROPERTY_TITLECOLOR_DISABLED) {
        _disabledLabelColor = value;
    } else if(propertyName == PROPERTY_TITLECOLOR_MOUSEOVER) {
        _mouseOverLabelColor = value;
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_NORMAL) {
        _normalBackgroundColor = value;
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED) {
        _selectedBackgroundColor = value;
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_DISABLED) {
       _disabledBackgroundColor = value;
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_MOUSEOVER) {
        _mouseOverBackgroundColor = value;
    } else {
        WidgetLoader::onHandlePropTypeColor3(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    if(propertyName == PROPERTY_FONTCOLOR){
        //_fontColor = value;
    } else if(propertyName == PROPERTY_OUTLINECOLOR){
        _outlineColor = value;
    } else if(propertyName == PROPERTY_SHADOWCOLOR){
        _shadowColor = value;
    } else {
        WidgetLoader::onHandlePropTypeColor4(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_SHADOWOFFSET) {
        _shadowOffset = value;
    } else {
        WidgetLoader::onHandlePropTypePosition(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_HORIZONTALALIGMENT){
        _textHAlignment = value;
    } else if(propertyName == PROPERTY_VERTICALALIGMENT) {
        _textVAlignment = value;
    }else if (propertyName == PROPERTY_OVERFLOW) {
        _overflowLabel = value;
    }else if (propertyName == PROPERTY_WORDWRAP) {
        _wordWrapLabel = static_cast<bool>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}
    
void ButtonLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_ADJUSTS_FONT_SIZE_TO_FIT){
        _adjustsFontSizeToFit = value;
    } else if (propertyName == PROPERTY_WORDWRAP) {
        _wordWrapLabel = value;
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END
