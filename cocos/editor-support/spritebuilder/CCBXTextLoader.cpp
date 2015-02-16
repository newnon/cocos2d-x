#include "CCBXTextLoader.h"
#include "ui/UIText.h"

NS_CC_BEGIN
namespace spritebuilder {

static const std::string PROPERTY_BLENDFUNC("blendFunc");
static const std::string PROPERTY_FONTNAME("fontName");
static const std::string PROPERTY_FONTSIZE("fontSize");
static const std::string PROPERTY_HORIZONTALALIGNMENT("horizontalAlignment");
static const std::string PROPERTY_VERTICALALIGNMENT("verticalAlignment");
static const std::string PROPERTY_STRING("string");
static const std::string PROPERTY_DIMENSIONS("dimensions");

static const std::string PROPERTY_FONTCOLOR("fontColor");
static const std::string PROPERTY_OUTLINECOLOR("outlineColor");
static const std::string PROPERTY_OUTLINEWIDTH("outlineWidth");

static const std::string PROPERTY_SHADOWCOLOR("shadowColor");

static const std::string PROPERTY_SHADOWBLURRADIUS("shadowBlurRadius");
static const std::string PROPERTY_SHADOWWIDTH("shadowWidth");

static const std::string PROPERTY_SHADOWOFFSET("shadowOffset");
    
static const std::string PROPERTY_ADJUSTSFONTSIZETOFIT("adjustsFontSizeToFit");
static const std::string PROPERTY_CONTENTSIZE("contentSize");
    
TextLoader *TextLoader::create()
{
    TextLoader *ret = new TextLoader();
    ret->autorelease();
    return ret;
}

Node *TextLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    ui::Text *text = ui::Text::create(_label, _font, getAbsoluteScale(mainScale, additionalScale, _fontSize.scale, _fontSize.type));
    return text;
}

void TextLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale);
    ui::Text *text = static_cast<ui::Text*>(node);
    if(_dimensions.size.width == 0 || _dimensions.size.height == 0)
        text->ignoreContentAdaptWithSize(true);
    //label->setBlendFunc(_blendFunc);
    float outlineWidth = getAbsoluteScale(mainScale, additionalScale, _outlineWidth.scale, _outlineWidth.type);
    float shadowBlurRadius = getAbsoluteScale(mainScale, additionalScale, _shadowBlurRadius.scale, _shadowBlurRadius.type);
    Vec2 shadowOffset = getAbsolutePosition(mainScale, additionalScale, _shadowOffset.pos, _shadowOffset.referenceCorner, _shadowOffset.xUnits, _shadowOffset.yUnits, parentSize);
    if (_outlineColor.a > 0 && outlineWidth > 0)
        text->enableOutline(_outlineColor, outlineWidth);
    if (_shadowColor.a > 0)
        text->enableShadow(_shadowColor, Size(shadowOffset.x, shadowOffset.y), shadowBlurRadius);
    if(_fontColor != Color4B::WHITE)
        text->setTextColor(_fontColor);
    text->setTextHorizontalAlignment(_textHAlignment);
    text->setTextVerticalAlignment(_textVAlignment);
    text->setTextAreaSize(getAbsoluteSize(mainScale, additionalScale, _dimensions.size, _dimensions.widthUnits, _dimensions.heightUnits, parentSize));
    text->setAdjustsFontSizeToFit(_adjustsFontSizeToFit);
}

TextLoader::TextLoader()
    :_blendFunc(BlendFunc::ALPHA_PREMULTIPLIED)
    ,_outlineColor(0,0,0,0)
    ,_shadowColor(0,0,0,0)
	,_outlineWidth(FloatScaleDescription{0, 0.0f})
	,_shadowBlurRadius(FloatScaleDescription{0, 0.0f})
	,_shadowOffset(PositionDescription{PositionReferenceCorner::TOPLEFT, PositionUnit::POINTS, PositionUnit::POINTS, Vec2(0, 0)})
    ,_textHAlignment(TextHAlignment::LEFT)
    ,_textVAlignment(TextVAlignment::TOP)
	,_dimensions(SizeDescription{SizeUnit::POINTS, SizeUnit::POINTS, {0, 0}})
    ,_fontColor(Color4B::WHITE)
    ,_adjustsFontSizeToFit(false)
{
    
}
    
TextLoader::~TextLoader()
{
    
}
    
void TextLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_ADJUSTSFONTSIZETOFIT){
        _adjustsFontSizeToFit = value;
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
void TextLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    if(propertyName == PROPERTY_FONTCOLOR){
        _fontColor = value;
    } else if(propertyName == PROPERTY_OUTLINECOLOR){
        _outlineColor = value;
    } else if(propertyName == PROPERTY_SHADOWCOLOR){
        _shadowColor = value;
    } else {
        WidgetLoader::onHandlePropTypeColor4(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
    if(propertyName == PROPERTY_BLENDFUNC) {
        _blendFunc = value;
    } else {
        WidgetLoader::onHandlePropTypeBlendFunc(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_FONTNAME) {
        _font = value;
    } else {
        WidgetLoader::onHandlePropTypeFontTTF(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_STRING) {
        _label = value;
    } else {
        WidgetLoader::onHandlePropTypeText(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_FONTSIZE) {
        _fontSize = value;
    } else if(propertyName == PROPERTY_OUTLINEWIDTH) {
        _outlineWidth = value;
    } else if(propertyName == PROPERTY_SHADOWBLURRADIUS) {
        _shadowBlurRadius = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_HORIZONTALALIGNMENT) {
        _textHAlignment = static_cast<TextHAlignment>(value);
    } else if(propertyName == PROPERTY_VERTICALALIGNMENT) {
        _textVAlignment = static_cast<TextVAlignment>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    if(propertyName == PROPERTY_DIMENSIONS) {
        _dimensions = value;
    } else {
        WidgetLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
    }
}

void TextLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_SHADOWOFFSET) {
        _shadowOffset = value;
    } else {
        WidgetLoader::onHandlePropTypePosition(propertyName, isExtraProp, value);
    }
}

}
    
NS_CC_END
