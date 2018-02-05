#include "CCBXLabelTTFLoader.h"
#include "2d/CCLabel.h"
#include "platform/CCFileUtils.h"

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
    
static const std::string PROPERTY_CONTENTSIZE("contentSize");
static const std::string PROPERTY_ADJUSTSFONTSIZETOFIT("adjustsFontSizeToFit");
static const std::string PROPERTY_OVERFLOW("overflowType");
static const std::string PROPERTY_WORDWRAP("wordWrap");
    
static const std::string PROPERTY_GRADIENTCOLOR1("gradientColor1");
static const std::string PROPERTY_GRADIENTCOLOR2("gradientColor2");
static const std::string PROPERTY_GRADIENTTYPE("gradientType");
    
enum class GradientType {
    kHorizontal = 0,
    kVertical = 1
};
    
LabelTTFLoader *LabelTTFLoader::create()
{
    LabelTTFLoader *ret = new LabelTTFLoader();
    ret->autorelease();
    return ret;
}

Node *LabelTTFLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    Label *label = Label::create();
    label->setAnchorPoint(cocos2d::Vec2(0,0));
    return label;
}

void LabelTTFLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    Label *label = dynamic_cast<Label*>(node);
    if(label)
    {
        Size dimensions = getAbsoluteSize(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_DIMENSIONS, _dimensions), parentSize);
        float fontSize = getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_FONTSIZE, _fontSize));
        float outlineWidth = getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_OUTLINEWIDTH, _outlineWidth));
        const cocos2d::Color4B &outlineColor = getNodeParamValue(params, PROPERTY_OUTLINECOLOR, _outlineColor);
        const std::string font = getNodeParamValue(params, PROPERTY_FONTNAME, _font);
        if (FileUtils::getInstance()->isFileExist(font))
        {
            TTFConfig ttfConfig(font, fontSize, GlyphCollection::DYNAMIC);
            ttfConfig.outlineSize = outlineColor.a > 0 ? outlineWidth : 0;
            label->setTTFConfig(ttfConfig);
        }
        else
        {
            label->setSystemFontName(font);
            label->setSystemFontSize(fontSize);
        }
        
        label->setDimensions(dimensions.width, dimensions.height);
        label->setString(getNodeParamValue(params, PROPERTY_STRING, _label));
        
        float shadowBlurRadius = getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_SHADOWBLURRADIUS, _shadowBlurRadius));
        Vec2 shadowOffset = getAbsolutePosition(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_SHADOWOFFSET, _shadowOffset), parentSize);
        
        if (outlineColor.a > 0 && outlineWidth > 0)
            label->enableOutline(outlineColor, outlineWidth);
        const cocos2d::Color4B &shadowColor = getNodeParamValue(params, PROPERTY_SHADOWCOLOR, _shadowColor);
        if (shadowColor.a > 0)
            label->enableShadow(shadowColor, Size(shadowOffset.x, shadowOffset.y), shadowBlurRadius);
        const Color4B &fontColor = getNodeParamValue(params, PROPERTY_FONTCOLOR, _fontColor);
        if(fontColor != Color4B::WHITE)
            label->setTextColor(fontColor);
        
        Label::Overflow overflow = static_cast<Label::Overflow>(getNodeParamValue(params, PROPERTY_OVERFLOW, _overflowLabel));
        label->setOverflow((getNodeParamValue(params, PROPERTY_ADJUSTSFONTSIZETOFIT, _adjustsFontSizeToFit) && overflow == Label::Overflow::NONE) ? Label::Overflow::SHRINK : overflow);
        label->setLineBreakWithoutSpace(!getNodeParamValue(params, PROPERTY_WORDWRAP, _wordWrapLabel));
        label->setAlignment(static_cast<TextHAlignment>(getNodeParamValue(params, PROPERTY_HORIZONTALALIGNMENT, _textHAlignment)), static_cast<TextVAlignment>(getNodeParamValue(params, PROPERTY_VERTICALALIGNMENT, _textVAlignment)));
        
        int gradientType = getNodeParamValue(params, PROPERTY_GRADIENTTYPE, _gradientType);
        const Color4B &gradientColor1 = getNodeParamValue(params, PROPERTY_GRADIENTCOLOR1, _gradientColor1);
        const Color4B &gradientColor2 = getNodeParamValue(params, PROPERTY_GRADIENTCOLOR2, _gradientColor2);
        
        if (static_cast<int>(GradientType::kHorizontal) == gradientType) {
            label->setHGradientColor(gradientColor1, gradientColor2);
        }
        else if (static_cast<int>(GradientType::kVertical) == gradientType) {
            label->setVGradientColor(gradientColor1, gradientColor2);
        }
        const BlendFunc &blendFunc = getNodeParamValue(params, PROPERTY_BLENDFUNC, _blendFunc);
        label->setBlendFunc(blendFunc==BlendFunc::ALPHA_PREMULTIPLIED ? BlendFunc::ALPHA_NON_PREMULTIPLIED : blendFunc);
    }
}

LabelTTFLoader::LabelTTFLoader()
    :_blendFunc(BlendFunc::ALPHA_PREMULTIPLIED)
    ,_outlineColor(0,0,0,0)
    ,_shadowColor(0,0,0,0)
	,_outlineWidth(FloatScaleDescription{0, 0.0f})
	,_shadowBlurRadius(FloatScaleDescription{0, 0.0f})
	,_shadowOffset(PositionDescription{PositionReferenceCorner::BOTTOMLEFT, PositionUnit::POINTS, PositionUnit::POINTS, Vec2(0, 0)})
    ,_textHAlignment((int)TextHAlignment::LEFT)
    ,_textVAlignment((int)TextVAlignment::TOP)
	,_dimensions(SizeDescription{SizeUnit::POINTS, SizeUnit::POINTS, {0, 0}})
    ,_fontColor(Color4B::WHITE)
    ,_adjustsFontSizeToFit(false)
    ,_overflowLabel(static_cast<int>(cocos2d::Label::Overflow::NONE))
    ,_wordWrapLabel(true)
    ,_gradientColor1(Color4B::WHITE)
    ,_gradientColor2(Color4B::WHITE)
    ,_gradientType(static_cast<int>(GradientType::kVertical))
{
    
}
    
LabelTTFLoader::~LabelTTFLoader()
{
    
}
    
void LabelTTFLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_ADJUSTSFONTSIZETOFIT){
        _adjustsFontSizeToFit = value;
    } else if (propertyName == PROPERTY_WORDWRAP) {
        _wordWrapLabel = value;
    } else {
        NodeLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
void LabelTTFLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    if(propertyName == PROPERTY_FONTCOLOR){
        _fontColor = value;
    } else if(propertyName == PROPERTY_OUTLINECOLOR){
        _outlineColor = value;
    } else if(propertyName == PROPERTY_SHADOWCOLOR){
        _shadowColor = value;
    } else if(propertyName == PROPERTY_GRADIENTCOLOR1){
        _gradientColor1 = value;
    } else if(propertyName == PROPERTY_GRADIENTCOLOR2){
        _gradientColor2 = value;
    } else {
        NodeLoader::onHandlePropTypeColor4(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
    if(propertyName == PROPERTY_BLENDFUNC) {
        _blendFunc = value;
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_FONTNAME) {
        _font = value;
    } else {
        NodeLoader::onHandlePropTypeFontTTF(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_STRING) {
        _label = value;
    } else {
        NodeLoader::onHandlePropTypeText(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_FONTSIZE) {
        _fontSize = value;
    } else if(propertyName == PROPERTY_OUTLINEWIDTH) {
        _outlineWidth = value;
    } else if(propertyName == PROPERTY_SHADOWBLURRADIUS) {
        _shadowBlurRadius = value;
    } else {
        NodeLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_HORIZONTALALIGNMENT) {
        _textHAlignment = value;
    } else if(propertyName == PROPERTY_VERTICALALIGNMENT) {
        _textVAlignment = value;
    }else if (propertyName == PROPERTY_OVERFLOW) {
        _overflowLabel = value;
    }else if (propertyName == PROPERTY_WORDWRAP) {
        _wordWrapLabel = static_cast<bool>(value);
    }else if (propertyName == PROPERTY_GRADIENTTYPE) {
        _gradientType = value;
    }else {
        NodeLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    if(propertyName == PROPERTY_DIMENSIONS) {
        _dimensions = value;
    } else {
        NodeLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
    }
}

void LabelTTFLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_SHADOWOFFSET) {
        _shadowOffset = value;
    } else {
        NodeLoader::onHandlePropTypePosition(propertyName, isExtraProp, value);
    }
}

}
    
NS_CC_END
