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
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const override;
    
CC_CONSTRUCTOR_ACCESS:
    SliderLoader();
    ~SliderLoader();

protected:
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value) override;
    virtual void onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value) override;
    
private:
    SpriteFrameDescription _backgroundSpriteFrame;
    SpriteFrameDescription _progressSpriteFrame;
    SpriteFrameDescription _handleNormalSpriteFrame;
    SpriteFrameDescription _handleDisabledSpriteFrame;
    SpriteFrameDescription _handleHiglihtedSpriteFrame;
    SpriteFrameDescription _handleMouseOverSpriteFrame;
    FloatScaleDescription _imageScale;
    Vec4 _margins;
    float _zoomScale;
    int _percent;
    int _maxPercent;
};

}

NS_CC_END

#endif
