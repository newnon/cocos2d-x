#include "CCBXWidgetLoader.h"
#include "ui/UIWidget.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_ENABLED("userInteractionEnabled");
static const std::string PROPERTY_CLICK("block");
static const std::string PROPERTY_TOUCH("blockTouch");
static const std::string PROPERTY_SOUND("sound");
    
WidgetLoader *WidgetLoader::create()
{
    WidgetLoader *ret = new WidgetLoader();
    ret->autorelease();
    return ret;
}
    
Node *WidgetLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    ui::Widget *widget = ui::Widget::create();
    widget->setAnchorPoint(Vec2(0.0f, 0.0f));
    return widget;
}
    
void WidgetLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    ui::Widget *widget = dynamic_cast<ui::Widget*>(node);
    if(widget)
    {
        widget->setTouchEnabled(getNodeParamValue(params, PROPERTY_ENABLED, _enabled));
        widget->ignoreContentAdaptWithSize(false);
    }
}
    
void WidgetLoader::setCallbacks(Node* node, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *parentOwner, const NodeParams& params) const
{
    ccReaderClickCallback clickCallback;
    ccReaderTouchCallback touchCallback;
    
    const CallbackDescription &clickDesc = getNodeParamValue(params, PROPERTY_CLICK, _clickDesc);
    const CallbackDescription &touchDesc = getNodeParamValue(params, PROPERTY_TOUCH, _touchDesc);
    
    if(!clickDesc.name.empty())
    {
        switch (clickDesc.type) {
            case TargetType::NONE:
                CCLOG("no assigment type for name:%s", clickDesc.name.c_str());
                break;
                
            case TargetType::DOCUMENT_ROOT:
            {
                CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
                if(rootOwner)
                {
                    if(!(clickCallback = rootOwner->onResolveCCBClickSelector(clickDesc.name, node)))
                    {
                        CCLOG("callback not assigned for name:%s", clickDesc.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment document_root but root node is not CCBXReaderOwner for name:%s", clickDesc.name.c_str());
                }
            }
                break;
                
            case TargetType::OWNER:
                if(owner)
                {
                    if(!(clickCallback = owner->onResolveCCBClickSelector(clickDesc.name, node)))
                    {
                        CCLOG("callback not assigned for name:%s", clickDesc.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", clickDesc.name.c_str());
                }
                break;
                
            case TargetType::PARENT_OWNER:
                if(parentOwner)
                {
                    if(!(clickCallback = parentOwner->onResolveCCBClickSelector(clickDesc.name, node)))
                    {
                        CCLOG("callback not assigned for name:%s", clickDesc.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no parent owner for name:%s", clickDesc.name.c_str());
                }
                break;
        }
    }
    
    if(!touchDesc.name.empty())
    {
        switch (touchDesc.type) {
            case TargetType::NONE:
                CCLOG("no assigment type for name:%s", touchDesc.name.c_str());
                break;
                
            case TargetType::DOCUMENT_ROOT:
            {
                CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
                if(rootOwner)
                {
                    if(!(touchCallback = rootOwner->onResolveCCBTouchSelector(touchDesc.name, node)))
                    {
                        CCLOG("variable not assigned for name:%s", touchDesc.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment document_root but root node is not CCBXReaderOwner for name:%s", touchDesc.name.c_str());
                }
            }
                break;
                
            case TargetType::OWNER:
                if(owner)
                {
                    if(!(touchCallback = owner->onResolveCCBTouchSelector(touchDesc.name, node)))
                    {
                        CCLOG("variable not assigned for name:%s", touchDesc.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", touchDesc.name.c_str());
                }
                break;
                
            case TargetType::PARENT_OWNER:
                if(parentOwner)
                {
                    if(!(touchCallback = parentOwner->onResolveCCBTouchSelector(touchDesc.name, node)))
                    {
                        CCLOG("variable not assigned for name:%s", touchDesc.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", touchDesc.name.c_str());
                }
                break;
        }
    }
    
    ui::Widget *widget = dynamic_cast<ui::Widget*>(node);
    if(widget)
    {
        std::string sound = getNodeParamValue(params, PROPERTY_SOUND, _sound);
        if(!sound.empty() && clickCallback)
        {
            widget->addClickEventListener([clickCallback, sound](Ref* ref){
                CCBXReaderSoundManager* manager = CCBXReader::getSoundManager();
                if(manager)
                    manager->playSound(sound);
                clickCallback(ref);
            });
        }
        else if(!sound.empty())
        {
            widget->addClickEventListener([sound](Ref* ref){
                CCBXReaderSoundManager* manager = CCBXReader::getSoundManager();
                if(manager)
                    manager->playSound(sound);
            });
        }
        else if(clickCallback)
        {
            widget->addClickEventListener(clickCallback);
        }
    }
    if(widget && touchCallback)
    {
        widget->addTouchEventListener(touchCallback);
    }
}
    
WidgetLoader::WidgetLoader():_enabled(false)
{
    
}
    
WidgetLoader::~WidgetLoader()
{
    
}
    
void WidgetLoader::onHandlePropTypeClickCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    if(propertyName == PROPERTY_CLICK) {
        _clickDesc = value;
    } else if(propertyName == PROPERTY_TOUCH) {
        _touchDesc = value;
    } else {
        NodeLoader::onHandlePropTypeClickCallback(propertyName, isExtraProp, value);
    }
}

void WidgetLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_ENABLED) {
        _enabled = value;
    } else {
        NodeLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}
    
void WidgetLoader::onHandlePropTypeSoundFile(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_SOUND){
        _sound = value;
    } else {
        WidgetLoader::onHandlePropTypeSoundFile(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END
