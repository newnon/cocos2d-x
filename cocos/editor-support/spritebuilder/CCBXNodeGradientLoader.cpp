#include "CCBXNodeGradientLoader.h"
#include "2d/CCLayer.h"

NS_CC_BEGIN
namespace spritebuilder {

static const std::string PROPERTY_STARTCOLOR("startColor");
static const std::string PROPERTY_ENDCOLOR("endColor");
static const std::string PROPERTY_STARTOPACITY("startOpacity");
static const std::string PROPERTY_ENDOPACITY("endOpacity");
static const std::string PROPERTY_VECTOR("vector");
static const std::string PROPERTY_BLENDFUNC("blendFunc");
    
NodeGradientLoader *NodeGradientLoader::create()
{
    NodeGradientLoader *ret = new NodeGradientLoader();
    ret->autorelease();
    return ret;
}

Node *NodeGradientLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    LayerGradient *layer = LayerGradient::create();
    layer->setAnchorPoint(cocos2d::Vec2(0.0f, 0.0f));
    layer->setIgnoreAnchorPointForPosition(false);
    layer->setOpacity(255);
    layer->setColor(Color3B::WHITE);
    return layer;
}

void NodeGradientLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    NodeColorLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    LayerGradient *layerColor = static_cast<LayerGradient*>(node);
    layerColor->setStartColor(getNodeParamValue(params, PROPERTY_STARTCOLOR, _startColor));
    layerColor->setEndColor(getNodeParamValue(params, PROPERTY_ENDCOLOR, _endColor));
    layerColor->setStartOpacity(getNodeOpacityParamValue(params, PROPERTY_STARTOPACITY, _startOpacity));
    layerColor->setEndOpacity(getNodeOpacityParamValue(params, PROPERTY_ENDOPACITY, _endOpacity));
    layerColor->setVector(getNodeParamValue(params, PROPERTY_VECTOR, _vector));
}
    
NodeGradientLoader::NodeGradientLoader()
    :_startColor(Color3B::WHITE)
    ,_endColor(Color3B::WHITE)
    ,_startOpacity(255)
    ,_endOpacity(255)
    ,_vector(0,-1)
{
    
}
NodeGradientLoader::~NodeGradientLoader(){
    
}

void NodeGradientLoader::onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value) {
    if(propertyName == PROPERTY_STARTCOLOR) {
        _startColor = value;
    } else if(propertyName == PROPERTY_ENDCOLOR) {
        _endColor = value;
    } else {
        NodeColorLoader::onHandlePropTypeColor3(propertyName, isExtraProp, value);
    }
}

void NodeGradientLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value){
    if(propertyName == PROPERTY_STARTOPACITY) {
        float opacity = value * 255.0f;
        _startOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else if(propertyName == PROPERTY_ENDOPACITY) {
        float opacity = value * 255.0f;
        _endOpacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else {
        NodeColorLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}


void NodeGradientLoader::onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const Point &value) {
    if(propertyName == PROPERTY_VECTOR) {
        _vector = Point(-value.x, -value.y);
    } else {
        NodeColorLoader::onHandlePropTypePoint(propertyName, isExtraProp, value);
    }
}
    
}
NS_CC_END
