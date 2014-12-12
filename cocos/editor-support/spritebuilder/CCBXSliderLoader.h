#ifndef _CCB_CCSLIDERLOADER_H_
#define _CCB_CCSLIDERLOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

/* Forward declaration. */
class CCBReader;

class SliderLoader : public WidgetLoader {
public:
    
    static SliderLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    SliderLoader();
    ~SliderLoader();

protected:
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
    
private:
};

}

NS_CC_END

#endif
