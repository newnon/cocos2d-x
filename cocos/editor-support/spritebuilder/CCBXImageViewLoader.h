#ifndef _CCBX_IMAGEVIEWLOADER_H_
#define _CCBX_IMAGEVIEWLOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class CC_DLL ImageViewLoader : public WidgetLoader {
public:
    
    static ImageViewLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const cocos2d::ValueMap &customProperties) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner) const override;
    
CC_CONSTRUCTOR_ACCESS:
    enum class RenderingType
    {
        AUTO = -1,
        SIMPLE,
        SLICE,
        TILED
    };
    
    ImageViewLoader(RenderingType renderingType = RenderingType::AUTO);
    ~ImageViewLoader();

protected:
    virtual void onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value) override;
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;

private:
    
    SizeDescription _size;
    FloatScaleDescription _imageScale;
    SpriteFrameDescription _spriteFrame;
    Vec4 _margins;
    BlendFunc _blendFunc;
    std::pair<bool,bool> _flipped;
    RenderingType _renderingType;
};
    
class CC_DLL TileImageViewLoader : public ImageViewLoader
{
public:
    
    static TileImageViewLoader *create();
    
CC_CONSTRUCTOR_ACCESS:
    TileImageViewLoader();
    
protected:
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
};

}

NS_CC_END

#endif
