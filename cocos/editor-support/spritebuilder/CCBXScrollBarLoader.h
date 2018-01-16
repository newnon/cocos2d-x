#ifndef _CCB_CCScrollBarLoader_H_
#define _CCB_CCScrollBarLoader_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class CC_DLL ScrollBarLoader : public WidgetLoader {
public:
    
    static ScrollBarLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const override;
    
CC_CONSTRUCTOR_ACCESS:
    ScrollBarLoader();
    ~ScrollBarLoader();

protected:
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value) override;
    virtual void onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value) override;
    
private:
    SpriteFrameDescription _backgroundSpriteFrame;
    SpriteFrameDescription _handleNormalSpriteFrame;
    SpriteFrameDescription _handleDisabledSpriteFrame;
    SpriteFrameDescription _handleHiglihtedSpriteFrame;
    SpriteFrameDescription _handleMouseOverSpriteFrame;
    FloatScaleDescription _imageScale;
    Vec4 _margins;
    Vec4 _handleMargins;
    float _zoomScale;
    float _percent;
    bool _isVertical;
};

}

NS_CC_END

#endif
