#ifndef _CCBX_CCPHYSICSNODELOADER_H_
#define _CCBX_CCPHYSICSNODELOADER_H_

#include "CCBXNodeLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class CC_DLL PhysicsNodeLoader : public NodeLoader {
public:
    
    static PhysicsNodeLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    PhysicsNodeLoader();
    ~PhysicsNodeLoader();

protected:
    virtual void onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const Vec2 &value) override;
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    
private:
    Vec2 _gravity;
    float _sleepTimeThreshold;
};

}

NS_CC_END

#endif
