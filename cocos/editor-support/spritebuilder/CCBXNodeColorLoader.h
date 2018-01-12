#ifndef _CCBX_COLORNODELOADER_H_
#define _CCBX_COLORNODELOADER_H_

#include "CCBXNodeLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class CC_DLL NodeColorLoader : public NodeLoader{
public:
    static NodeColorLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties) const override;
    
CC_CONSTRUCTOR_ACCESS:
    NodeColorLoader();
    ~NodeColorLoader();

protected:
    virtual void onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value) override;
    
private:
    BlendFunc _blendFunc;
};

}

NS_CC_END

#endif
