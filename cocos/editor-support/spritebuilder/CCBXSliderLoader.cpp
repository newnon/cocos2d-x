#include "CCBXSliderLoader.h"
#include "ui/UISlider.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_SPACING("spacing");
static const std::string PROPERTY_DIRECTION("direction");
    
    
SliderLoader *SliderLoader::create()
{
    SliderLoader *ret = new SliderLoader();
    ret->autorelease();
    return ret;
}
Node *SliderLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return ui::Slider::create();
}
void SliderLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::Slider *slider = dynamic_cast<ui::Slider*>(node);
}

SliderLoader::SliderLoader()
{
    
}

SliderLoader::~SliderLoader()
{
    
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