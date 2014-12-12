#include "CCBXPhysicsNodeLoader.h"
#include "2d/CCPhysicsNode.h"
#if CC_USE_PHYSICS
#include "physics/CCPhysicsWorld.h"
#endif

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_GRAVITY("gravity");
static std::string PROPERTY_SLEEPTIMETHRESHOLD("sleepTimeThreshold");
    
    
PhysicsNodeLoader *PhysicsNodeLoader::create()
{
    PhysicsNodeLoader *ret = new PhysicsNodeLoader();
    ret->autorelease();
    return ret;
}
    
Node *PhysicsNodeLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return PhysicsNode::create();
}
    
void PhysicsNodeLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
#if CC_USE_PHYSICS
    PhysicsNode *physicsNode = dynamic_cast<PhysicsNode*>(node);
    if(physicsNode)
    {
        physicsNode->getPhysicsWorld()->setGravity(_gravity);
    }
#endif
}
    
PhysicsNodeLoader::PhysicsNodeLoader():_gravity(0,-100),_sleepTimeThreshold(0.5)
{
    
}
PhysicsNodeLoader::~PhysicsNodeLoader(){
    
}

void PhysicsNodeLoader::onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const Vec2 &value) {
    if(propertyName == PROPERTY_GRAVITY) {
        _gravity = value;
    } else {
        NodeLoader::onHandlePropTypePoint(propertyName, isExtraProp, value);
    }
}
    
void PhysicsNodeLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_SLEEPTIMETHRESHOLD) {
        _sleepTimeThreshold = value;
    } else {
        NodeLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END