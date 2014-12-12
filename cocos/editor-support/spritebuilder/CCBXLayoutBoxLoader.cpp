#include "CCBXLayoutBoxLoader.h"
#include "ui/UILayout.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_SPACING("spacing");
static std::string PROPERTY_DIRECTION("direction");
    
    
LayoutBoxLoader *LayoutBoxLoader::create()
{
    LayoutBoxLoader *ret = new LayoutBoxLoader();
    ret->autorelease();
    return ret;
}
Node *LayoutBoxLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return ui::Layout::create();
}
void LayoutBoxLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::Layout *layout = dynamic_cast<ui::Layout*>(node);
    if(layout)
    {
        if(_direction == Horizontal)
            layout->setLayoutType(ui::Layout::Type::HORIZONTAL);
        else
            layout->setLayoutType(ui::Layout::Type::VERTICAL);
        layout->setSpacing(_spacing);
    }
}

LayoutBoxLoader::LayoutBoxLoader():_spacing(0)
{
    
}

LayoutBoxLoader::~LayoutBoxLoader()
{
    
}
    
void LayoutBoxLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_SPACING) {
        _spacing = value.scale;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }

}
void LayoutBoxLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_DIRECTION) {
        _direction = static_cast<LayoutBoxDirection>(value);
        //((LayoutBox *)pNode)->setDirection(static_cast<cocos2d::LayoutBoxDirection>(pIntegerLabeled));
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END