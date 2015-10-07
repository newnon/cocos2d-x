#include "CCBXSliderLoader.h"
#include "ui/UISlider.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_SPACING("spacing");
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_BACKGROUND("backgroundSpriteFrame|Normal");
static const std::string PROPERTY_HANDLE_NORMAL("handleSpriteFrame|Normal");
static const std::string PROPERTY_HANDLE_HIGHLIGHTED("handleSpriteFrame|Highlighted");
static const std::string PROPERTY_HANDLE_DISABLED("handleSpriteFrame|Disabled");
    
static const std::string PROPERTY_MARGIN_LEFT("marginLeft");
static const std::string PROPERTY_MARGIN_TOP("marginTop");
static const std::string PROPERTY_MARGIN_RIGHT("marginRight");
static const std::string PROPERTY_MARGIN_BOTTOM("marginBottom");
    
    
SliderLoader *SliderLoader::create()
{
    SliderLoader *ret = new SliderLoader();
    ret->autorelease();
    return ret;
}
Node *SliderLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    Rect margin(_margins.origin.x,_margins.origin.y,1.0-_margins.size.width-_margins.origin.x,1.0-_margins.size.height-_margins.origin.y);
    ui::Slider *slider = ui::Slider::create();
    slider->ignoreContentAdaptWithSize(false);
    switch(_backGround.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
        {
            Size size = _backGround.spriteFrame->getOriginalSize();
            Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
            slider->loadBarTexture(_backGround.path, ui::Widget::TextureResType::LOCAL);
            slider->setCapInsetsBarRenderer(realMargins);
            slider->setScale9Enabled(!realMargins.size.equals(size) || realMargins.origin != Vec2::ZERO);
        }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
        {
            Size size = _backGround.spriteFrame->getOriginalSize();
            Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
            slider->loadBarTexture(_backGround.path, ui::Widget::TextureResType::PLIST);
            slider->setCapInsetsBarRenderer(realMargins);
            slider->setScale9Enabled(!realMargins.size.equals(size) || realMargins.origin != Vec2::ZERO);
        }
            break;
        default:
            break;
    };
    switch(_handleNormal.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
        {
            slider->loadSlidBallTextureNormal(_handleNormal.path, ui::Widget::TextureResType::LOCAL);
        }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
        {
            slider->loadSlidBallTextureNormal(_handleNormal.path, ui::Widget::TextureResType::PLIST);
        }
            break;
        default:
            break;
    };
    switch(_handleDisabled.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
        {
            slider->loadSlidBallTextureDisabled(_handleDisabled.path, ui::Widget::TextureResType::LOCAL);
        }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
        {
            slider->loadSlidBallTextureDisabled(_handleDisabled.path, ui::Widget::TextureResType::PLIST);
        }
            break;
        default:
            break;
    };
    switch(_handleHiglihted.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
        {
            slider->loadSlidBallTexturePressed(_handleHiglihted.path, ui::Widget::TextureResType::LOCAL);
        }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
        {
            slider->loadSlidBallTexturePressed(_handleHiglihted.path, ui::Widget::TextureResType::PLIST);
        }
            break;
        default:
            break;
    };
    return slider;

}
void SliderLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    //WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale);
    //ui::Slider *slider = dynamic_cast<ui::Slider*>(node);
}

SliderLoader::SliderLoader()
{
    
}

SliderLoader::~SliderLoader()
{
    
}
    
void SliderLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_BACKGROUND) {
        _backGround = value;
    } else if(propertyName == PROPERTY_HANDLE_NORMAL) {
        _handleNormal = value;
    } else if(propertyName == PROPERTY_HANDLE_HIGHLIGHTED) {
        _handleHiglihted = value;
    } else if(propertyName == PROPERTY_HANDLE_DISABLED) {
        _handleDisabled = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.origin.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.origin.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.size.width = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.size.height = value;
    } else {
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_SPACING) {
        //((LayoutBox *)pNode)->setSpacing(pFloatScale);
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}
    
void SliderLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_DIRECTION) {
        //((LayoutBox *)pNode)->setDirection(static_cast<cocos2d::LayoutBoxDirection>(pIntegerLabeled));
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END