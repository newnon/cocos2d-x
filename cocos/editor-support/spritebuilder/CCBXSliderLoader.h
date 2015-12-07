#ifndef _CCB_CCSLIDERLOADER_H_
#define _CCB_CCSLIDERLOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

/* Forward declaration. */
class CCBReader;

class CC_DLL SliderLoader : public WidgetLoader {
public:
    
    static SliderLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) override;
    
CC_CONSTRUCTOR_ACCESS:
    SliderLoader();
    ~SliderLoader();

protected:
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value) override;
    
private:
    SpriteFrameDescription _background;
    SpriteFrameDescription _progress;
    SpriteFrameDescription _handleNormal;
    SpriteFrameDescription _handleDisabled;
    SpriteFrameDescription _handleHiglihted;
    cocos2d::Rect _margins;
    float _zoomScale;
    int _maxPercent;
};

}

NS_CC_END

#endif
