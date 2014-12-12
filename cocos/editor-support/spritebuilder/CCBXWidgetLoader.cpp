#include "CCBXWidgetLoader.h"
#include "ui/UIWidget.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_ENABLED("userInteractionEnabled");
static std::string PROPERTY_CLICK("block");
static std::string PROPERTY_TOUCH("blockTouch");
    
WidgetLoader *WidgetLoader::create()
{
    WidgetLoader *ret = new WidgetLoader();
    ret->autorelease();
    return ret;
}
    
Node *WidgetLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return ui::Widget::create();
}
    
void WidgetLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::Widget *widget = dynamic_cast<ui::Widget*>(node);
    widget->setEnabled(_enabled);
}
    
void WidgetLoader::setCallbacks(Node* node, CCBXReaderOwner *owner, Node *rootNode)
{
    ccReaderClickCallback click;
    ccReaderTouchCallback touch;
    if(!_click.name.empty())
    {
        switch (_click.type) {
            case TargetType::NONE:
                CCLOG("no assigment type for name:%s", _click.name.c_str());
                break;
                
            case TargetType::DOCUMENT_ROOT:
            {
                CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
                if(rootOwner)
                {
                    if(!(click = rootOwner->onResolveCCBClickSelector(_click.name, node)))
                    {
                        CCLOG("callback not assigned for name:%s", _click.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment document_root but root node is not CCBXReaderOwner for name:%s", _click.name.c_str());
                }
            }
                break;
                
            case TargetType::OWNER:
                if(owner)
                {
                    if(!(click = owner->onResolveCCBClickSelector(_click.name, node)))
                    {
                        CCLOG("callback not assigned for name:%s", _click.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", _click.name.c_str());
                }
                break;
        }
    }
    else
    {
        if(_click.type != TargetType::NONE)
        {
            CCLOG("callback name set but no assigment type for name:%s", _click.name.c_str());
        }
    }
    
    if(!_touch.name.empty())
    {
        switch (_touch.type) {
            case TargetType::NONE:
                CCLOG("no assigment type for name:%s", _touch.name.c_str());
                break;
                
            case TargetType::DOCUMENT_ROOT:
            {
                CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
                if(rootOwner)
                {
                    if(!(touch = rootOwner->onResolveCCBTouchSelector(_touch.name, node)))
                    {
                        CCLOG("variable not assigned for name:%s", _touch.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment document_root but root node is not CCBXReaderOwner for name:%s", _touch.name.c_str());
                }
            }
                break;
                
            case TargetType::OWNER:
                if(owner)
                {
                    if(!(touch = owner->onResolveCCBTouchSelector(_touch.name, node)))
                    {
                        CCLOG("variable not assigned for name:%s", _touch.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", _touch.name.c_str());
                }
                break;
        }
    }
    else
    {
        if(_touch.type != TargetType::NONE)
        {
            CCLOG("variable name set but no assigment type for name:%s", _touch.name.c_str());
        }
    }
    
    ui::Widget *widget = dynamic_cast<ui::Widget*>(node);
    if(widget && click)
    {
        widget->addClickEventListener(click);
    }
    if(widget && touch)
    {
        widget->addTouchEventListener(touch);
    }
}
    
WidgetLoader::WidgetLoader():_enabled(true)
{
    
}
    
WidgetLoader::~WidgetLoader()
{
    
}
    
void WidgetLoader::onHandlePropTypeClickCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    if(propertyName == PROPERTY_CLICK) {
        _click = value;
    } else if(propertyName == PROPERTY_TOUCH) {
        _touch = value;
    } else {
        NodeLoader::onHandlePropTypeClickCallback(propertyName, isExtraProp, value);
    }
}
    
void WidgetLoader::onHandlePropTypeTouchCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    if(propertyName == PROPERTY_TOUCH) {
        _touch = value;
    } else {
        NodeLoader::onHandlePropTypeTouchCallback(propertyName, isExtraProp, value);
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

}

NS_CC_END
