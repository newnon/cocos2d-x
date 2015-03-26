#include "CCBXTextFieldLoader.h"
#include "ui/UITextField.h"

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
    
TextFieldLoader *TextFieldLoader::create()
{
    TextFieldLoader *ret = new TextFieldLoader();
    ret->autorelease();
    return ret;
}

Node *TextFieldLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    ui::TextField *textField = ui::TextField::create(_label, _font, getAbsoluteScale(mainScale, additionalScale, _fontSize.scale, _fontSize.type));
    return textField;
}

void TextFieldLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale);
}

TextFieldLoader::TextFieldLoader()
{
    
}
    
TextFieldLoader::~TextFieldLoader()
{
    
}
    
void TextFieldLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_ADJUSTSFONTSIZETOFIT){
        //((Label *)pNode)->setTextColor(pColor4B);
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
void TextFieldLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    if(propertyName == PROPERTY_FONTCOLOR){
        //((Label *)pNode)->setTextColor(pColor4B);
    } else if(propertyName == PROPERTY_OUTLINECOLOR){
        _outlineColor = value;
    } else if(propertyName == PROPERTY_SHADOWCOLOR){
        _shadowColor = value;
    } else {
        WidgetLoader::onHandlePropTypeColor4(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
    if(propertyName == PROPERTY_BLENDFUNC) {
        //((LayerColor *)pNode)->setBlendFunc(pBlendFunc);
    } else {
        WidgetLoader::onHandlePropTypeBlendFunc(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_FONTNAME) {
        _font = value;
    } else {
        WidgetLoader::onHandlePropTypeFontTTF(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_STRING) {
        _label = value;
    } else {
        WidgetLoader::onHandlePropTypeText(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
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

void TextFieldLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_HORIZONTALALIGNMENT) {
        _textHAlignment = static_cast<TextHAlignment>(value);
    } else if(propertyName == PROPERTY_VERTICALALIGNMENT) {
        _textVAlignment = static_cast<TextVAlignment>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    if(propertyName == PROPERTY_DIMENSIONS) {
        //NodeLoader::onHandlePropTypeSize(PROPERTY_CONTENTSIZE, isExtraProp, value);
    } else {
        WidgetLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_SHADOWOFFSET) {
        //_shadowOffset = pPosition;
    } else {
        WidgetLoader::onHandlePropTypePosition(propertyName, isExtraProp, value);
    }
}

    
/*void LabelTTFLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _outlineColor = Color4B(0,0,0,0);
    _shadowColor = Color4B(0,0,0,0);
    _outlineWidth = 0.0f;
    _shadowBlurRadius = 0.0f;
    _shadowOffset = Vec2(0,0);
}
    
void LabelTTFLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    if (_outlineColor.a > 0 && _outlineWidth > 0)
        ((Label *)pNode)->enableOutline(_outlineColor, _outlineWidth);
    if (_shadowColor.a > 0)
        ((Label *)pNode)->enableShadow(_shadowColor, _shadowOffset, _shadowBlurRadius);
}
    
void LabelTTFLoader::onHandlePropTypeColor4(Node * pNode, Node * pParent, const char * pPropertyName, const Color4B &pColor4B, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTCOLOR) == 0){
        ((Label *)pNode)->setTextColor(pColor4B);
    } else if(strcmp(pPropertyName, PROPERTY_OUTLINECOLOR) == 0){
        _outlineColor = pColor4B;
    } else if(strcmp(pPropertyName, PROPERTY_SHADOWCOLOR) == 0){
        _shadowColor = pColor4B;
    } else {
        NodeLoader::onHandlePropTypeColor4(pNode, pParent, pPropertyName, pColor4B, ccbReader);
    }
}

void LabelTTFLoader::onHandlePropTypePosition(Node * pNode, Node * pParent, const char* pPropertyName, const Point &pPosition, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_SHADOWOFFSET) == 0) {
        _shadowOffset = pPosition;
    } else {
        NodeLoader::onHandlePropTypePosition(pNode, pParent, pPropertyName, pPosition, pCCBReader);
    }
}

void LabelTTFLoader::onHandlePropTypeBlendFunc(Node * pNode, Node * pParent, const char * pPropertyName, BlendFunc pBlendFunc, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        ((Label *)pNode)->setBlendFunc(pBlendFunc);
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pBlendFunc, ccbReader);
    }
}

void LabelTTFLoader::onHandlePropTypeFontTTF(Node * pNode, Node * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
        ((Label *)pNode)->setSystemFontName(pFontTTF);
    } else {
        NodeLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, ccbReader);
    }
}

void LabelTTFLoader::onHandlePropTypeText(Node * pNode, Node * pParent, const char * pPropertyName, const char * pText, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_STRING) == 0) {
        ((Label *)pNode)->setString(pText);
    } else {
        NodeLoader::onHandlePropTypeText(pNode, pParent, pPropertyName, pText, ccbReader);
    }
}

void LabelTTFLoader::onHandlePropTypeFloatScale(Node * pNode, Node * pParent, const char * pPropertyName, float pFloatScale, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
        ((Label *)pNode)->setSystemFontSize(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_OUTLINEWIDTH) == 0) {
        _outlineWidth = pFloatScale;
	} else if(strcmp(pPropertyName, PROPERTY_SHADOWBLURRADIUS) == 0) {
        _shadowBlurRadius = pFloatScale;
    } else {
        NodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, ccbReader);
    }
}

void LabelTTFLoader::onHandlePropTypeIntegerLabeled(Node * pNode, Node * pParent, const char * pPropertyName, int pIntegerLabeled, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_HORIZONTALALIGNMENT) == 0) {
        ((Label *)pNode)->setHorizontalAlignment(TextHAlignment(pIntegerLabeled));
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGNMENT) == 0) {
        ((Label *)pNode)->setVerticalAlignment(TextVAlignment(pIntegerLabeled));
    } else {
        NodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pIntegerLabeled, ccbReader);
    }
}

void LabelTTFLoader::onHandlePropTypeSize(Node * pNode, Node * pParent, const char * pPropertyName, const Size &size, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_DIMENSIONS) == 0) {
        ((Label *)pNode)->setDimensions(size.width,size.height);
    } else {
        NodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, size, ccbReader);
    }
}*/

}
    
NS_CC_END
