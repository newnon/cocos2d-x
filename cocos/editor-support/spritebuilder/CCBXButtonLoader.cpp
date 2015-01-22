#include "CCBXButtonLoader.h"
#include "2d/CCLabel.h"
#include "ui/UIButton.h"

NS_CC_BEGIN
namespace spritebuilder {

static std::string PROPERTY_ZOOMONCLICK("zoomOnClick");
static std::string PROPERTY_LABELANCHORPOINT("labelAnchorPoint");
static std::string PROPERTY_PREFERREDSIZE("preferredSize");
static std::string PROPERTY_CONTENTSIZE("contentSize");
static std::string PROPERTY_MAXSIZE("maxSize");
    
static std::string PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL("backgroundSpriteFrame|Normal");
static std::string PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED("backgroundSpriteFrame|Highlighted");
static std::string PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED("backgroundSpriteFrame|Disabled");
    
static std::string PROPERTY_BACKGROUNDOPACITY_NORMAL("backgroundOpacity|Normal");
static std::string PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED("backgroundOpacity|Highlighted");
static std::string PROPERTY_BACKGROUNDOPACITY_DISABLED("backgroundOpacity|Disabled");
    
static std::string PROPERTY_LABELOPACITY_NORMAL("labelOpacity|Normal");
static std::string PROPERTY_LABELOPACITY_HIGHLIGHTED("labelOpacity|Highlighted");
static std::string PROPERTY_LABELOPACITY_DISABLED("labelOpacity|Disabled");
    
static std::string PROPERTY_TITLE("title");
    
static std::string PROPERTY_TITLECOLOR_NORMAL("labelColor|Normal");
static std::string PROPERTY_TITLECOLOR_HIGHLIGHTED("labelColor|Highlighted");
static std::string PROPERTY_TITLECOLOR_DISABLED("labelColor|Disabled");
    
static std::string PROPERTY_BACKGROUNDCOLOR_NORMAL("backgroundColor|Normal");
static std::string PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED("backgroundColor|Highlighted");
static std::string PROPERTY_BACKGROUNDCOLOR_DISABLED("backgroundColor|Disabled");
    
static std::string PROPERTY_HORIZONTALPADDING("horizontalPadding");
static std::string PROPERTY_VERTICALPADDING("verticalPadding");
    
static std::string PROPERTY_HORIZONTALALIGMENT("horizontalAlignment");
static std::string PROPERTY_VERTICALALIGMENT("verticalAlignment");
    
static std::string PROPERTY_FONTNAME("fontName");
static std::string PROPERTY_FONTSIZE("fontSize");
    
static std::string PROPERTY_FONTCOLOR("fontColor");
static std::string PROPERTY_OUTLINECOLOR("outlineColor");
static std::string PROPERTY_OUTLINEWIDTH("outlineWidth");
    
static std::string PROPERTY_SHADOWBLURRADIUS("shadowBlurRadius");
static std::string PROPERTY_SHADOWWIDTH("shadowWidth");
static std::string PROPERTY_SHADOWCOLOR("shadowColor");
    
static std::string PROPERTY_SHADOWOFFSET("shadowOffset");
    
static std::string PROPERTY_MARGIN_LEFT("marginLeft");
static std::string PROPERTY_MARGIN_TOP("marginTop");
static std::string PROPERTY_MARGIN_RIGHT("marginRight");
static std::string PROPERTY_MARGIN_BOTTOM("marginBottom");
    
static std::string PROPERTY_OFFSET_LEFT("offsetLeft");
static std::string PROPERTY_OFFSET_TOP("offsetTop");
static std::string PROPERTY_OFFSET_RIGHT("offsetRight");
static std::string PROPERTY_OFFSET_BOTTOM("offsetBottom");
    
ButtonLoader *ButtonLoader::create()
{
    ButtonLoader *ret = new ButtonLoader();
    ret->autorelease();
    return ret;
}

Node *ButtonLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    Rect margin(_margins.origin.x,_margins.origin.y,1.0-_margins.size.width-_margins.origin.x,1.0-_margins.size.height-_margins.origin.y);
    ui::Button *button = ui::Button::create();
    //button->setScale9Enabled(true);
    switch(_normalSpriteFrame.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
            {
                Size size = _normalSpriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                button->loadTextureNormal(_normalSpriteFrame.path, ui::Widget::TextureResType::LOCAL);
                button->setCapInsetsNormalRenderer(realMargins);
            }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
            {
                Size size = _normalSpriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                button->loadTextureNormal(_normalSpriteFrame.path, ui::Widget::TextureResType::PLIST);
                button->setCapInsetsNormalRenderer(realMargins);
            }
            break;
        default:
            break;
    };
    switch(_selectedSpriteFrame.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
            {
                Size size = _selectedSpriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                button->loadTexturePressed(_selectedSpriteFrame.path, ui::Widget::TextureResType::LOCAL);
                button->setCapInsetsPressedRenderer(realMargins);
            }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
            {
                Size size = _selectedSpriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                button->loadTexturePressed(_selectedSpriteFrame.path, ui::Widget::TextureResType::PLIST);
                button->setCapInsetsPressedRenderer(realMargins);
            }
            break;
        default:
            break;
    };
    switch(_disabledSpriteFrame.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
            {
                Size size = _disabledSpriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                button->loadTextureDisabled(_disabledSpriteFrame.path, ui::Widget::TextureResType::LOCAL);
                button->setCapInsetsDisabledRenderer(realMargins);
            }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
            {
                Size size = _disabledSpriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                button->loadTextureDisabled(_disabledSpriteFrame.path, ui::Widget::TextureResType::PLIST);
                button->setCapInsetsDisabledRenderer(realMargins);
            }
            break;
        default:
            break;
    };
    button->setScale9Enabled(true);
    return button;
}

void ButtonLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::Button *button = dynamic_cast<ui::Button*>(node);
    if(button)
    {
        button->setPressedActionEnabled(_zoomOnClick!=1.0f);
        button->setZoomScale(_zoomOnClick);
        button->setNormalTitleColor(_normalLabelColor);
        button->setNormalBackgroundColor(_normalBackgroundColor);
        button->setNormalTitleOpacity(_normalLabelOpacity);
        button->setNormalBackgroundOpacity(_normalBackgroundOpacity);
        
        button->setPressedTitleColor(_selectedLabelColor);
        button->setPressedBackgroundColor(_selectedBackgroundColor);
        button->setPressedTitleOpacity(_selectedLabelOpacity);
        button->setPressedBackgroundOpacity(_selectedBackgroundOpacity);
        
        button->setDisabledTitleColor(_disabledLabelColor);
        button->setDisabledBackgroundColor(_disabledBackgroundColor);
        button->setDisabledTitleOpacity(_disabledLabelOpacity);
        button->setDisabledBackgroundOpacity(_disabledBackgroundOpacity);
        
        button->getTitleRenderer()->setSystemFontName(_font);
        button->setTitleFontSize(getAbsoluteScale(mainScale, additionalScale, _fontSize.scale, _fontSize.type));
        button->getTitleRenderer()->setVerticalAlignment(_textVAlignment);
        button->getTitleRenderer()->setHorizontalAlignment(_textHAlignment);
        button->setHorizontalPadding(getAbsoluteScale(mainScale, additionalScale, _horizPadding.scale, _horizPadding.type));
        button->setVerticalPadding(getAbsoluteScale(mainScale, additionalScale, _vertPadding.scale, _vertPadding.type));
        button->setOffsets(_offset.origin.x, _offset.origin.y, _offset.size.width, _offset.size.height);
        button->setTitleText(_label);
    }
}

ButtonLoader::ButtonLoader()
    :_zoomOnClick(1.0f)
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
    ,_disabledBackgroundOpacity(255)
    ,_textHAlignment(TextHAlignment::CENTER)
    ,_textVAlignment(TextVAlignment::CENTER)
    ,_horizPadding{0,0.0f}
    ,_vertPadding{0,0.0f}
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

void ButtonLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.origin.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.origin.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.size.width = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.size.height = value;
    } else if(propertyName == PROPERTY_OFFSET_LEFT) {
        _offset.origin.x = value;
    } else if(propertyName == PROPERTY_OFFSET_TOP) {
        _offset.origin.y = value;
    } else if(propertyName == PROPERTY_OFFSET_RIGHT) {
        _offset.size.width = value;
    } else if(propertyName == PROPERTY_OFFSET_BOTTOM) {
        _offset.size.height = value;
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_NORMAL) {
        float opacity = value * 255.0f;
        _normalBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED) {
        float opacity = value * 255.0f;
        _selectedBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_BACKGROUNDOPACITY_DISABLED) {
        float opacity = value * 255.0f;
        _disabledBackgroundOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_NORMAL) {
        float opacity = value * 255.0f;
        _normalLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_HIGHLIGHTED) {
        float opacity = value * 255.0f;
        _selectedLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_LABELOPACITY_DISABLED) {
        float opacity = value * 255.0f;
        _disabledLabelOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
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
        _horizPadding = value;
    } else if(propertyName == PROPERTY_VERTICALPADDING) {
        _vertPadding = value;
    } else if(propertyName == PROPERTY_OUTLINEWIDTH) {
        //_outlineWidth = pFloatScale;
    } else if(propertyName == PROPERTY_SHADOWBLURRADIUS) {
        //_shadowBlurRadius = pFloatScale;
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
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_NORMAL) {
        _normalBackgroundColor = value;
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED) {
        _selectedBackgroundColor = value;
    } else if(propertyName == PROPERTY_BACKGROUNDCOLOR_DISABLED) {
       _disabledBackgroundColor = value;
    } else {
        WidgetLoader::onHandlePropTypeColor3(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    if(propertyName == PROPERTY_FONTCOLOR){
        //((ButtonControl *)pNode)->getLabel()->setTextColor(pColor4B);
    } else if(propertyName == PROPERTY_OUTLINECOLOR){
        //_outlineColor = pColor4B;
    } else if(propertyName == PROPERTY_SHADOWCOLOR){
        //_shadowColor = pColor4B;
    } else {
        WidgetLoader::onHandlePropTypeColor4(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_SHADOWOFFSET) {
        //_shadowOffset = pPosition;
    } else {
        WidgetLoader::onHandlePropTypePosition(propertyName, isExtraProp, value);
    }
}

void ButtonLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_HORIZONTALALIGMENT){
        _textHAlignment = static_cast<TextHAlignment>(value);
    } else if(propertyName == PROPERTY_VERTICALALIGMENT) {
        _textVAlignment = static_cast<TextVAlignment>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}


/*void ButtonLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _margins=Rect::ZERO;
    _outlineColor = Color4B(0,0,0,0);
    _shadowColor = Color4B(0,0,0,0);
    _outlineWidth = 0.0f;
    _shadowBlurRadius = 0.0f;
    _shadowOffset = Vec2(0,0);
}

void ButtonLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    ((ButtonControl *)pNode)->setCapInsets(Rect(_margins.origin.x,_margins.origin.y,1.0-_margins.size.width-_margins.origin.x,1.0-_margins.size.height-_margins.origin.y));
    if (_outlineColor.a > 0 && _outlineWidth > 0)
        ((ButtonControl *)pNode)->getLabel()->enableOutline(_outlineColor, _outlineWidth);
    if (_shadowColor.a > 0)
        ((ButtonControl *)pNode)->getLabel()->enableShadow(_shadowColor, _shadowOffset, _shadowBlurRadius);
}
    
void ButtonLoader::onHandlePropTypeIntegerLabeled(cocos2d::Node * pNode, cocos2d::Node * pParent, const char* pPropertyName, int pIntegerLabeled, CCBReader * ccbReader)
{
    if(propertyName == PROPERTY_HORIZONTALALIGMENT) == 0){
        ((ButtonControl *)pNode)->setHorizontalAlignment(static_cast<TextHAlignment>(pIntegerLabeled));
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGMENT) == 0) {
        ((ButtonControl *)pNode)->setVerticalAlignment(static_cast<TextVAlignment>(pIntegerLabeled));
    } else {
        ControlLoader::onHandlePropTypeIntegerLabeled(pNode, pParent, pPropertyName, pIntegerLabeled, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeCheck(Node * pNode, Node * pParent, const char * pPropertyName, bool pCheck, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_ZOOMWHENHIGHLIGHTED) == 0){
        ((ButtonControl *)pNode)->setZoomWhenHighlighted(pCheck);
    } else if(strcmp(pPropertyName, PROPERTY_TOGGLESSELECTEDSTATE) == 0) {
        ((ButtonControl *)pNode)->setTogglesSelectedState(pCheck);
    } else {
        ControlLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeString(Node * pNode, Node * pParent, const char * pPropertyName, const char * pString, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLE) == 0) {
        ((ButtonControl *)pNode)->setTitle(pString);

    } else {
        ControlLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeText(Node * pNode, Node * pParent, const char * pPropertyName, const char * pString, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLE) == 0) {
        ((ButtonControl *)pNode)->setTitle(pString);
    } else {
        ControlLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeFontTTF(Node * pNode, Node * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
        ((ButtonControl *)pNode)->setFontName(pFontTTF);
    } else {
        ControlLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypeFloat(cocos2d::Node * pNode, cocos2d::Node * pParent, const char* pPropertyName, float pFloat, CCBReader * ccbReader)
{
    if(strcmp(pPropertyName, PROPERTY_MARGIN_LEFT) == 0) {
        _margins.origin.x = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_TOP) == 0) {
        _margins.origin.y = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_RIGHT) == 0) {
        _margins.size.width = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_BOTTOM) == 0) {
        _margins.size.height = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_LEFT) == 0) {
        ((ButtonControl *)pNode)->setLeftOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_TOP) == 0) {
        ((ButtonControl *)pNode)->setTopOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_RIGHT) == 0) {
        ((ButtonControl *)pNode)->setRightOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_BOTTOM) == 0) {
        ((ButtonControl *)pNode)->setBottomOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::SELECTED_HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::SELECTED_HIGH_LIGHTED);
    } else {
        ControlLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeFloatScale(Node * pNode, Node * pParent, const char * pPropertyName, float pFloatScale, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
        ((ButtonControl *)pNode)->setFontSize(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_HORIZONTALPADDING) == 0) {
        ((ButtonControl *)pNode)->setHorizontalPadding(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALPADDING) == 0) {
        ((ButtonControl *)pNode)->setVerticalPadding(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_OUTLINEWIDTH) == 0) {
        _outlineWidth = pFloatScale;
    } else if(strcmp(pPropertyName, PROPERTY_SHADOWBLURRADIUS) == 0) {
        _shadowBlurRadius = pFloatScale;
    } else {
        ControlLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypePoint(Node * pNode, Node * pParent, const char * pPropertyName, const Point &pPoint, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_LABELANCHORPOINT) == 0) {
        //((Button *)pNode)->setLabelAnchorPoint(pPoint);
    } else {
        ControlLoader::onHandlePropTypePoint(pNode, pParent, pPropertyName, pPoint, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeSize(Node * pNode, Node * pParent, const char * pPropertyName, const Size &pSize, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_PREFERREDSIZE) == 0) {
        ((ButtonControl *)pNode)->setPreferredSize(pSize);
    } else if(strcmp(pPropertyName, PROPERTY_MAXSIZE) == 0) {
        ((ButtonControl *)pNode)->setMaxSize(pSize);
    } else {
        ControlLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeSpriteFrame(Node * pNode, Node * pParent, const char * pPropertyName, SpriteFrame * pSpriteFrame, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::NORMAL);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::HIGH_LIGHTED);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::DISABLED);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_SELECTED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::SELECTED);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIHGLIGHTED_SELECTED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::SELECTED_HIGH_LIGHTED);
        }
    } else {
        ControlLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pSpriteFrame, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypeBlock(cocos2d::Node * pNode, cocos2d::Node * pParent, const char * pPropertyName, BlockData * pBlockData, CCBReader * ccbReader)
{
    if(strcmp(pPropertyName, PROPERTY_BLOCK) == 0) {
        if(pBlockData)
            ((ButtonControl *)pNode)->setCallback( std::bind( pBlockData->mSELMenuHandler, pBlockData->_target, std::placeholders::_1) );
    } else {
        NodeLoader::onHandlePropTypeBlock(pNode, pParent, pPropertyName, pBlockData, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypeColor4(Node * pNode, Node * pParent, const char * pPropertyName, const Color4B &pColor4B, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTCOLOR) == 0){
		((ButtonControl *)pNode)->getLabel()->setTextColor(pColor4B);
    } else if(strcmp(pPropertyName, PROPERTY_OUTLINECOLOR) == 0){
        _outlineColor = pColor4B;
    } else if(strcmp(pPropertyName, PROPERTY_SHADOWCOLOR) == 0){
        _shadowColor = pColor4B;
    } else {
        ControlLoader::onHandlePropTypeColor4(pNode, pParent, pPropertyName, pColor4B, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeColor3(Node * pNode, Node * pParent, const char * pPropertyName, const Color3B &pColor3B, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::SELECTED_HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::SELECTED_HIGH_LIGHTED);
    } else {
        ControlLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pColor3B, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypePosition(Node * pNode, Node * pParent, const char* pPropertyName, const Point &pPosition, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_SHADOWOFFSET) == 0) {
        _shadowOffset = pPosition;
    } else {
        NodeLoader::onHandlePropTypePosition(pNode, pParent, pPropertyName, pPosition, pCCBReader);
    }
}*/

}

NS_CC_END