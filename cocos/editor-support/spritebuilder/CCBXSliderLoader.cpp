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
    
    
SliderLoader *SliderLoader::create()
{
    SliderLoader *ret = new SliderLoader();
    ret->autorelease();
    return ret;
}
Node *SliderLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    ui::Slider *slider = ui::Slider::create();
    switch(_backGround.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
        {
            slider->loadBarTexture(_backGround.path, ui::Widget::TextureResType::LOCAL);
        }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
        {
            slider->loadBarTexture(_backGround.path, ui::Widget::TextureResType::PLIST);
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