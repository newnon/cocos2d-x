#include "CCBXTextFieldLoader.h"
#include "ui/UITextField.h"

NS_CC_BEGIN
namespace spritebuilder {

static const std::string PROPERTY_FONTCOLOR("fontColor");
static const std::string PROPERTY_FONTNAME("fontName");
static const std::string PROPERTY_FONTSIZE("fontSize");
static const std::string PROPERTY_HORIZONTALALIGNMENT("horizontalAlignment");
static const std::string PROPERTY_VERTICALALIGNMENT("verticalAlignment");
static const std::string PROPERTY_STRING("string");

static const std::string PROPERTY_PLACEHOLDERFONTCOLOR("placeholderFontColor");
static const std::string PROPERTY_PLACEHOLDER("placeholder");
    
static const std::string PROPERTY_MAXLENGTH("maxLength");
static const std::string PROPERTY_PASSWORD("password");
    
    
TextFieldLoader *TextFieldLoader::create()
{
    TextFieldLoader *ret = new TextFieldLoader();
    ret->autorelease();
    return ret;
}

Node *TextFieldLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    ui::TextField *textField = ui::TextField::create(getNodeParamValue(params, PROPERTY_PLACEHOLDER, _placeholder), getNodeParamValue(params, PROPERTY_FONTNAME, _font), getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_FONTSIZE, _fontSize)));
    textField->setAnchorPoint(Vec2(0.0f, 0.0f));
    return textField;
}

void TextFieldLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    ui::TextField *textField = static_cast<ui::TextField*>(node);
    textField->setString(getNodeParamValue(params, PROPERTY_STRING, _label));
    textField->setPlaceHolder(getNodeParamValue(params, PROPERTY_PLACEHOLDER, _placeholder));
    textField->setTextHorizontalAlignment(static_cast<TextHAlignment>(getNodeParamValue(params, PROPERTY_HORIZONTALALIGNMENT, _textHAlignment)));
    textField->setTextVerticalAlignment(static_cast<TextVAlignment>(getNodeParamValue(params, PROPERTY_VERTICALALIGNMENT, _textVAlignment)));
    textField->setTextColor(getNodeParamValue(params, PROPERTY_FONTCOLOR, _fontColor));
    textField->setPlaceHolderColor(getNodeParamValue(params, PROPERTY_PLACEHOLDERFONTCOLOR, _placeholderFontColor));
    int maxLength = getNodeParamValue(params, PROPERTY_MAXLENGTH, _maxLength);
    if(maxLength>0)
    {
        textField->setMaxLengthEnabled(true);
        textField->setMaxLength(maxLength);
    }
    textField->setPasswordEnabled(getNodeParamValue(params, PROPERTY_PASSWORD, _password));
}

TextFieldLoader::TextFieldLoader()
    :_fontColor(cocos2d::Color4B::WHITE)
    ,_placeholderFontColor(cocos2d::Color3B::WHITE)
    ,_textHAlignment((int)TextHAlignment::LEFT)
    ,_textVAlignment((int)TextVAlignment::TOP)
    ,_maxLength(0)
    ,_password(false)
{
    
}
    
TextFieldLoader::~TextFieldLoader()
{
    
}
    
void TextFieldLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_PASSWORD) {
        _password = value;
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
void TextFieldLoader::onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value)
{
    if(propertyName == PROPERTY_PLACEHOLDERFONTCOLOR) {
        _placeholderFontColor = value;
    } else if(propertyName == PROPERTY_FONTCOLOR) {
        _fontColor = Color4B(value);
    } else {
        WidgetLoader::onHandlePropTypeColor3(propertyName, isExtraProp, value);
    }
}
    
void TextFieldLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    if(propertyName == PROPERTY_FONTCOLOR) {
        _fontColor = value;
    } else {
        WidgetLoader::onHandlePropTypeColor4(propertyName, isExtraProp, value);
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
    } else if(propertyName == PROPERTY_PLACEHOLDER) {
        _placeholder = value;
    } else {
        WidgetLoader::onHandlePropTypeText(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_FONTSIZE) {
        _fontSize = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}
    
void TextFieldLoader::onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_MAXLENGTH) {
        _maxLength = value;
    } else {
        WidgetLoader::onHandlePropTypeInteger(propertyName, isExtraProp, value);
    }
}

void TextFieldLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_HORIZONTALALIGNMENT) {
        _textHAlignment = value;
    } else if(propertyName == PROPERTY_VERTICALALIGNMENT) {
        _textVAlignment = value;
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

}
    
NS_CC_END
