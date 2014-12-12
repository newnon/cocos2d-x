#ifndef _CCB_CCSCALE9SPRITELOADER_H_
#define _CCB_CCSCALE9SPRITELOADER_H_

#include "CCBXNodeLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class Scale9SpriteLoader : public NodeLoader {
public:
    
    static Scale9SpriteLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    Scale9SpriteLoader();
    ~Scale9SpriteLoader();

protected:
    virtual void onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    
private:
    Rect _margins;
    SizeDescription _size;
    SpriteFrameDescription _spriteFrame;
    BlendFunc _blendFunc;
};

}

NS_CC_END

#endif
