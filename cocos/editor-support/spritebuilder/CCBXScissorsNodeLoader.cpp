#include "CCBXScissorsNodeLoader.h"
#include "ui/UILayout.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_SPACING("spacing");
static std::string PROPERTY_DIRECTION("direction");
    
    
CCBXScissorsNodeLoader *CCBXScissorsNodeLoader::create()
{
    CCBXScissorsNodeLoader *ret = new CCBXScissorsNodeLoader();
    ret->autorelease();
    return ret;
}
Node *CCBXScissorsNodeLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    return ui::Layout::create();
}
void CCBXScissorsNodeLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::Layout *layout = dynamic_cast<ui::Layout*>(node);
    layout->setClippingEnabled(true);
    layout->setClippingType(ui::Layout::ClippingType::SCISSOR);
}

CCBXScissorsNodeLoader::CCBXScissorsNodeLoader()
{
    
}

CCBXScissorsNodeLoader::~CCBXScissorsNodeLoader()
{
    
}

}

NS_CC_END