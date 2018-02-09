#ifndef _CCB_CCCCSCISSORSNODELOADER_H_
#define _CCB_CCCCSCISSORSNODELOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

/* Forward declaration. */
class CCBReader;

class CC_DLL ScissorsNodeLoader : public WidgetLoader {
public:
    
    static ScissorsNodeLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const override;
    
CC_CONSTRUCTOR_ACCESS:
    ScissorsNodeLoader();
    ~ScissorsNodeLoader();
};

}

NS_CC_END

#endif
