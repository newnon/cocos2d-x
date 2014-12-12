#ifndef _CCBX_IMAGEVIEWLOADER_H_
#define _CCBX_IMAGEVIEWLOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class ImageViewLoader : public WidgetLoader {
public:
    
    static ImageViewLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    ImageViewLoader();
    ~ImageViewLoader();

protected:
    
    virtual void onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
private:
    SizeDescription _size;
    SpriteFrameDescription _spriteFrame;
};

}

NS_CC_END

#endif
