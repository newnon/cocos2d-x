#include "CCBXLayoutBoxLoader.h"
#include "ui/UILayout.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_SPACING("spacing");
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_CLIPCONTENT("clipContent");
    
    
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
        layout->setSpacing(getAbsoluteScale(mainScale, additionalScale, _spacing.scale, _spacing.type));
    }
}

LayoutBoxLoader::LayoutBoxLoader() :_spacing(FloatScaleDescription{0, 0}), _clipping(false)
{
    
}

LayoutBoxLoader::~LayoutBoxLoader()
{
    
}
    
void LayoutBoxLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_SPACING) {
        _spacing = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }

}
void LayoutBoxLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_DIRECTION) {
        _direction = static_cast<LayoutBoxDirection>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}
    
void LayoutBoxLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_CLIPCONTENT) {
        _clipping = value;
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END