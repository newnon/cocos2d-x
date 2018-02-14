//
//  CCBXNodeLoader.cpp
//  cocos2d_libs
//
//  Created by Sergey on 29.10.14.
//
//

#include "CCBXNodeLoader.h"
#include "CCBXSequence.h"
#include "CCBXAnimationManager.h"
#include "physics/CCPhysicsBody.h"
#include "base/CCDirector.h"

NS_CC_BEGIN

namespace spritebuilder {
    
Vec2 getAbsoluteScale(float mainScale, float additionalScale, float scaleX, float scaleY, unsigned int type)
{
    
    float scaleXCoef = 1.0;
    float scaleYCoef = 1.0;
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_RESOURCES_SCALE))
    {
        scaleXCoef *= CCBXReader::getResolutionScale();
        scaleYCoef *= CCBXReader::getResolutionScale();
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_MAIN_SCALE))
    {
        scaleXCoef *= mainScale;
        scaleYCoef *= mainScale;
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_ADDITION_SCALE_X))
    {
        scaleXCoef *= additionalScale;
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_ADDITION_SCALE_Y))
    {
        scaleYCoef *= additionalScale;
    }
    
    if (type & static_cast<int>(ScaleType::INVERT_SCALE))
    {
        scaleXCoef = 1.0f/scaleXCoef;
        scaleYCoef = 1.0f/scaleYCoef;
    }
    
    return Vec2(scaleX*scaleXCoef,scaleY*scaleYCoef);
}
    
float getAbsoluteScale(float mainScale, float additionalScale, float scale, unsigned int type)
{
    
    float scaleCoef = CCBXReader::getResolutionScale();
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_RESOURCES_SCALE))
    {
        scaleCoef *= CCBXReader::getResolutionScale();
    }
    
    if (type & static_cast<int>(ScaleType::MULTIPLY_MAIN_SCALE))
    {
        scaleCoef *= mainScale;
    }
    
    if (type & static_cast<int>(ScaleType::INVERT_SCALE))
    {
        scaleCoef = 1.0f/scaleCoef;
    }
    
    return scale * scaleCoef;
}
    
Vec2 getAbsolutePosition(float mainScale, float additionalScale, const Point &pt, PositionReferenceCorner corner, PositionUnit xUnit, PositionUnit yUnit, const Size &containerSize)
{
    Point positionInPoints=Point(0,0);
    float x = 0;
    float y = 0;
    
    // Convert position to points
    if (xUnit == PositionUnit::POINTS) x = pt.x * CCBXReader::getResolutionScale();
    else if (xUnit == PositionUnit::UIPOINTS) x = pt.x * mainScale * CCBXReader::getResolutionScale();
    else if (xUnit == PositionUnit::NORMALIZED) x = pt.x * containerSize.width;
    
    if (yUnit == PositionUnit::POINTS) y = pt.y * CCBXReader::getResolutionScale();
    else if (yUnit == PositionUnit::UIPOINTS) y = pt.y * mainScale * CCBXReader::getResolutionScale();
    else if (yUnit == PositionUnit::NORMALIZED) y = pt.y * containerSize.height;
    
    // Account for reference corner
    //CCPositionReferenceCorner corner = type.corner;
    if (corner == PositionReferenceCorner::BOTTOMLEFT)
    {
        // Nothing needs to be done
    }
    else if (corner == PositionReferenceCorner::TOPLEFT)
    {
        // Reverse y-axis
        y = containerSize.height - y;
    }
    else if (corner == PositionReferenceCorner::TOPRIGHT)
    {
        // Reverse x-axis and y-axis
        x = containerSize.width - x;
        y = containerSize.height - y;
    }
    else if (corner == PositionReferenceCorner::BOTTOMRIGHT)
    {
        // Reverse x-axis
        x = containerSize.width - x;
    }
    
    positionInPoints.x = x;
    positionInPoints.y = y;
    
    return positionInPoints;
}

Size getAbsoluteSize(float mainScale, float additionalScale, const Size &contentSize, SizeUnit widthUnit, SizeUnit heightUnit, const Size &containerSize)
{
    Size size = Size::ZERO;
    
    // Width
    if (widthUnit == SizeUnit::POINTS)
    {
        size.width = CCBXReader::getResolutionScale() * contentSize.width;
    }
    else if (widthUnit == SizeUnit::UIPOINTS)
    {
        size.width = CCBXReader::getResolutionScale() * mainScale * contentSize.width;
    }
    else if (widthUnit == SizeUnit::NORMALIZED)
    {
        size.width = contentSize.width * containerSize.width;
    }
    else if (widthUnit == SizeUnit::INSETPOINTS)
    {
        size.width = containerSize.width - contentSize.width * CCBXReader::getResolutionScale();
    }
    else if (widthUnit == SizeUnit::INSETUIPOINTS)
    {
        size.width = containerSize.width - contentSize.width * CCBXReader::getResolutionScale() * mainScale;
    }
    
    // Height
    if (heightUnit == SizeUnit::POINTS)
    {
        size.height = CCBXReader::getResolutionScale() * contentSize.height;
    }
    else if (heightUnit == SizeUnit::UIPOINTS)
    {
        size.height = CCBXReader::getResolutionScale() * mainScale * contentSize.height;
    }
    else if (heightUnit == SizeUnit::NORMALIZED)
    {
        size.height = contentSize.height * containerSize.height;
    }
    else if (heightUnit == SizeUnit::INSETPOINTS)
    {
        size.height = containerSize.height - contentSize.height * CCBXReader::getResolutionScale();
    }
    else if (heightUnit == SizeUnit::INSETUIPOINTS)
    {
        size.height = containerSize.height - contentSize.height * CCBXReader::getResolutionScale() * mainScale;
    }
    
    return size;
}
    
Vec2 getAbsolutePosition(float mainScale, float additionalScale, const PositionDescription &position, const Size &containerSize)
{
    return getAbsolutePosition(mainScale, additionalScale, position.pos, position.referenceCorner, position.xUnits, position.yUnits, containerSize);
}
Size getAbsoluteSize(float mainScale, float additionalScale, const SizeDescription &size, const Size &containerSize)
{
    return getAbsoluteSize(mainScale, additionalScale, size.size, size.widthUnits, size.heightUnits, containerSize);
}
Vec2 getAbsoluteScale(float mainScale, float additionalScale, const ScaleDescription &scale)
{
    return getAbsoluteScale(mainScale, additionalScale, scale.xScale, scale.yScale, scale.type);
}
float getAbsoluteScale(float mainScale, float additionalScale, const FloatScaleDescription &scale)
{
    return getAbsoluteScale(mainScale, additionalScale, scale.scale, scale.type);
}


#define ASSERT_FAIL_UNEXPECTED_PROPERTY(PROPERTY) log("Unexpected property: '%s'!\n", PROPERTY.c_str()); assert(false)

static const std::string PROPERTY_POSITION("position");
static const std::string PROPERTY_CONTENTSIZE("contentSize");
static const std::string PROPERTY_SKEW("skew");
static const std::string PROPERTY_ANCHORPOINT("anchorPoint");
static const std::string PROPERTY_SCALE("scale");
static const std::string PROPERTY_ROTATION("rotation");
static const std::string PROPERTY_TAG("tag");
static const std::string PROPERTY_VISIBLE("visible");
static const std::string PROPERTY_NAME("name");
static const std::string PROPERTY_COLOR("color");
static const std::string PROPERTY_CASCADECOLOR("cascadeColorEnabled");
static const std::string PROPERTY_OPACITY("opacity");
static const std::string PROPERTY_CASCADEOPACITY("cascadeOpacityEnabled");
static const std::string PROPERTY_MEMBERVARASSIGNMENT("memberVarAssignment");

NodeLoader *NodeLoader::create()
{
    NodeLoader *ret = new NodeLoader();
    ret->autorelease();
    return ret;
}
    
Node *NodeLoader::createNode(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, CCBAnimationManager *manager,  Node *rootNode, CCBXReaderOwner *parentOwner, const CreateNodeFunction &createNodeFunction, const std::function<void(cocos2d::Node*, AnimationCompleteType)> &defaultAnimationCallback, bool nestedPrefab, const cocos2d::ValueMap *customProperties, const PrefabParams *paramsProperties) const
{
    const NodeParams* params = nullptr;
    
    if(paramsProperties)
    {
        auto it = paramsProperties->find(_uuid);
        if(it != paramsProperties->end())
            params = &(it->second);
    }
    
    Node *ret;
    if(createNodeFunction)
        ret = createNodeFunction(parentSize, mainScale, additionalScale);
    else
        ret = createNodeInstance(parentSize, mainScale, additionalScale, owner, rootNode, parentOwner, customProperties ? *customProperties : _customProperties, params ? *params : NodeParams());
    if(!ret)
        return nullptr;
    
    if(!loadNode(ret, parentSize, mainScale, additionalScale, owner, manager, rootNode, parentOwner, defaultAnimationCallback, nestedPrefab, customProperties, paramsProperties))
        return nullptr;

    return ret;
}
    
bool NodeLoader::loadNode(Node *node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, CCBAnimationManager *manager, Node *rootNode, CCBXReaderOwner *parentOwner, const std::function<void(cocos2d::Node*, AnimationCompleteType)> &defaultAnimationCallback, bool nestedPrefab, const cocos2d::ValueMap *customProperties, const PrefabParams *paramsProperties) const
{
    if(!node)
        return false;
    
    const NodeParams emptyParams;
    const NodeParams* params = &emptyParams;
    
    if(paramsProperties)
    {
        auto it = paramsProperties->find(_uuid);
        if(it != paramsProperties->end())
            params = &(it->second);
    }
    
    setProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, params ? *params : NodeParams());
    setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties ? *customProperties : _customProperties, params ? *params : NodeParams());
    setCallbacks(node, owner, rootNode, parentOwner, params ? *params : NodeParams());
    setVariables(node, owner, rootNode, parentOwner, params ? *params : NodeParams());
    
    if(!rootNode)
    {
        rootNode = node;
    }
    
    bool newManager = false;
    
    if(manager == nullptr)
    {
        newManager = true;
        manager = new CCBAnimationManager(mainScale, additionalScale, node ,owner);
        manager->setSequences(_sequences);
        manager->setAutoPlaySequenceId(_autoPlaySequenceId);
        manager->setRootContainerSize(parentSize);
        node->setUserObject(manager);
        manager->release();
    }
    for(const auto &it:_baseValues)
    {
        manager->setBaseValue(it.second, node, it.first);
    }
    for(const auto &it:_objects)
    {
        manager->setObject(it.second, node, it.first);
    }
    setAnimation(node, manager);
#if CC_USE_PHYSICS
    if(_physicsLoader)
        node->setPhysicsBody(_physicsLoader->createBody(node));
#endif
    CCBXReaderOwner *newParentOwner = dynamic_cast<CCBXReaderOwner*>(node);
    if(newParentOwner)
        parentOwner = newParentOwner;
    for(auto child:_children)
    {
        Node *childNode = child->createNode(node->getContentSize(), mainScale, additionalScale, owner, manager, rootNode, parentOwner, nullptr, nullptr, false, nullptr, paramsProperties);
        
        if (childNode)
        {
            node->addChild(childNode);
        }
    }
    if(newManager && !nestedPrefab)
    {
        if (node && _autoPlaySequenceId != -1)
        {
            // Auto play animations
            manager->runAnimationsForSequenceIdTweenDuration(_autoPlaySequenceId, 0, defaultAnimationCallback);
        }
        else if (defaultAnimationCallback)
        {
            rootNode->scheduleOnce([rootNode,defaultAnimationCallback](float) { defaultAnimationCallback(rootNode, AnimationCompleteType::COMPLETED); }, 0, "defaultAnimationCallback");
        }
    }
    
    CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
    if(rootOwner)
        rootOwner->onNodeLoaded(node);
    if(owner)
        owner->onNodeLoaded(node);
    
    if(!nestedPrefab)
        onNodeLoaded(node);
    
    return true;
}
    
void NodeLoader::setVariables(Node* node, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *parentOwner, const NodeParams& params) const
{
    VarAssignmentDescription memberVarAssignment = getNodeParamValue(params, PROPERTY_MEMBERVARASSIGNMENT, _memberVarAssignment);
    
    if(!memberVarAssignment.name.empty())
    {
        switch (memberVarAssignment.type) {
            case TargetType::NONE:
                CCLOG("no assigment type for name:%s", memberVarAssignment.name.c_str());
                break;
                
            case TargetType::DOCUMENT_ROOT:
            {
                CCBXReaderOwner *rootOwner = dynamic_cast<CCBXReaderOwner*>(rootNode);
                if(rootOwner)
                {
                    if(!rootOwner->onAssignCCBMemberVariable(memberVarAssignment.name, node))
                    {
                        CCLOG("variable not assigned for name:%s", memberVarAssignment.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment document_root but root node is not CCBXReaderOwner for name:%s", memberVarAssignment.name.c_str());
                }
            }
                break;
                
            case TargetType::OWNER:
                if(owner)
                {
                    if(!owner->onAssignCCBMemberVariable(memberVarAssignment.name, node))
                    {
                        CCLOG("variable not assigned for name:%s", memberVarAssignment.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no owner for name:%s", memberVarAssignment.name.c_str());
                }
                break;
                
            case TargetType::PARENT_OWNER:
                if(parentOwner)
                {
                    if(!parentOwner->onAssignCCBMemberVariable(memberVarAssignment.name, node))
                    {
                        CCLOG("variable not assigned for name:%s", memberVarAssignment.name.c_str());
                    }
                }
                else
                {
                    CCLOG("assigment type owner but no parent owner for name:%s", memberVarAssignment.name.c_str());
                }
                break;
        }
    }
    else
    {
        if(memberVarAssignment.type != TargetType::NONE)
        {
            CCLOG("variable name set but no assigment type for name:%s", memberVarAssignment.name.c_str());
        }
    }
}

void NodeLoader::setProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const NodeParams& params) const
{
    //don't check params if tey are empty
    if(params.empty())
    {
        node->setVisible(_visible);
        node->setPosition(getAbsolutePosition(mainScale, additionalScale, _position.pos, _position.referenceCorner, _position.xUnits , _position.yUnits, parentSize));
        cocos2d::Vec2 scales = getAbsoluteScale(mainScale, additionalScale, _scale.xScale, _scale.yScale, _scale.type);
        if(_anchorPointLoaded)
            node->setAnchorPoint(_anchorPoint);
        if(_sizeLoaded)
            node->setContentSize(getAbsoluteSize(mainScale, additionalScale, _size.size, _size.widthUnits, _size.heightUnits, parentSize));
        node->setScaleX(scales.x);
        node->setScaleY(scales.y);
        node->setRotation(_rotation);
        node->setTag(_tag);
        node->setName(_name);
        node->setCascadeColorEnabled(_cascadeColorEnabled);
        node->setCascadeOpacityEnabled(_cascadeOpacityEnabled);
        node->setColor(_color);
        node->setOpacity(_opacity);
        node->setSkewX(_skew.x);
        node->setSkewY(_skew.y);
    }
    else
    {
        node->setVisible(getNodeParamValue(params, PROPERTY_VISIBLE, _visible));
        const PositionDescription &position = getNodeParamValue(params, PROPERTY_POSITION, _position);
        node->setPosition(getAbsolutePosition(mainScale, additionalScale, position.pos, position.referenceCorner, position.xUnits , position.yUnits, parentSize));
        const ScaleDescription &scale = getNodeParamValue(params, PROPERTY_SCALE, _scale);
        cocos2d::Vec2 scales = getAbsoluteScale(mainScale, additionalScale, scale.xScale, scale.yScale, scale.type);
        
        auto anchorPointIt = params.find(PROPERTY_ANCHORPOINT);
        if(anchorPointIt != params.end())
            node->setAnchorPoint(mpark::get<Vec2>(anchorPointIt->second));
        else
            if(_anchorPointLoaded) node->setAnchorPoint(_anchorPoint);
        
        auto contentSizeIt = params.find(PROPERTY_CONTENTSIZE);
        if(contentSizeIt != params.end())
        {
            const SizeDescription &size = mpark::get<SizeDescription>(contentSizeIt->second);
            node->setContentSize(getAbsoluteSize(mainScale, additionalScale, size.size, size.widthUnits, size.heightUnits, parentSize));
        }
        else
        {
            if(_sizeLoaded)
                node->setContentSize(getAbsoluteSize(mainScale, additionalScale, _size.size, _size.widthUnits, _size.heightUnits, parentSize));
        }
        node->setScaleX(scales.x);
        node->setScaleY(scales.y);
        node->setRotation(getNodeParamValue(params, PROPERTY_ROTATION, _rotation));
        node->setTag(getNodeParamValue(params, PROPERTY_TAG, _tag));
        node->setName(getNodeParamValue(params, PROPERTY_NAME, _name));
        node->setCascadeColorEnabled(getNodeParamValue(params, PROPERTY_CASCADECOLOR, _cascadeColorEnabled));
        node->setCascadeOpacityEnabled(getNodeParamValue(params, PROPERTY_CASCADEOPACITY, _cascadeOpacityEnabled));
        node->setColor(getNodeParamValue(params, PROPERTY_COLOR, _color));
        
        auto opacityIt = params.find(PROPERTY_OPACITY);
        if(opacityIt != params.end())
        {
            float opacity = mpark::get<float>(opacityIt->second) * 255.0f;
            node->setOpacity((opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity)));
        }
        else
        {
            node->setOpacity(_opacity);
        }
        const Vec2 &skew = getNodeParamValue(params, PROPERTY_SKEW, _skew);
        node->setSkewX(skew.x);
        node->setSkewY(skew.y);
    }
}
    
void NodeLoader::setAnimation(Node* node, CCBAnimationManager *manager) const
{
    manager->addNode(node, _nodeSequences);
}
    
void NodeLoader::setSceneScaleType(SceneScaleType sceneScaleType)
{
    _sceneScaleType = sceneScaleType;
}

Node *NodeLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    return Node::create();
}
    
void NodeLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    
}
    
void NodeLoader::setCallbacks(Node* node, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *parentOwner, const NodeParams& params) const
{
    
}
    
void NodeLoader::setMemberVarAssignment(TargetType type, const std::string &name)
{
    _memberVarAssignment.type = type;
    _memberVarAssignment.name = name;
}
    
void NodeLoader::setUUID(unsigned value)
{
    _uuid = value;
}
    
void NodeLoader::setPhysicsLoader(PhysicsBodyLoader *loader)
{
    CC_SAFE_RELEASE(_physicsLoader);
    _physicsLoader = loader;
    CC_SAFE_RETAIN(_physicsLoader);
}

void NodeLoader::addChild(NodeLoader *loader)
{
    _children.pushBack(loader);
}
    
void NodeLoader::setSequences(const Vector<CCBSequence*> &sequences)
{
    _sequences = sequences;
}
    
void NodeLoader::setAutoPlaySequenceId(int id)
{
    _autoPlaySequenceId = id;
}
    
void NodeLoader::setBaseValues(const std::map<std::string, cocos2d::Value> &values)
{
    _baseValues = values;
}
    
void NodeLoader::setObjects(const std::map<std::string, cocos2d::Ref*> &objects)
{
    _objects = objects;
}
    
void NodeLoader::setNodeSequences(const std::map<int, Map<std::string, CCBSequenceProperty*>> &sequences)
{
    _nodeSequences = sequences;
}
    
void NodeLoader::setPrefabParams(const PrefabParams &params)
{
    _params = params;
}
    
NodeLoader::NodeLoader()
    :_uuid(0)
    ,_position(PositionDescription{PositionReferenceCorner::BOTTOMLEFT, PositionUnit::POINTS, PositionUnit::POINTS, Vec2(0, 0)})
    ,_anchorPointLoaded(false)
    ,_anchorPoint(0,0)
    ,_sizeLoaded(false)
	,_size(SizeDescription{SizeUnit::POINTS, SizeUnit::POINTS, Size(0, 0)})
	,_scale(ScaleDescription{0, 1.0f, 1.0f})
    ,_opacity(255)
    ,_rotation(0)
    ,_skew(0,0)
    ,_tag(0)
    ,_visible(true)
    ,_cascadeColorEnabled(false)
    ,_cascadeOpacityEnabled(false)
    ,_color(Color3B::WHITE)
    ,_memberVarAssignment{TargetType::NONE, ""}
    ,_physicsLoader(nullptr)
    ,_autoPlaySequenceId(-1)
    ,_sceneScaleType(SceneScaleType::PROJECT_DEFAULT)
{
}

NodeLoader::~NodeLoader()
{
     CC_SAFE_RELEASE(_physicsLoader);
}

void NodeLoader::onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value)
{
    if(propertyName == PROPERTY_POSITION) {
        _position = value;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const Point &value)
{
    if(propertyName == PROPERTY_ANCHORPOINT) {
        _anchorPoint = value;
        _anchorPointLoaded = true;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}

void NodeLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    if(propertyName == PROPERTY_CONTENTSIZE) {
        _size = value;
        _sizeLoaded = true;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypeScale(const std::string &propertyName, bool isExtraProp, const ScaleDescription &value)
{
    if(propertyName == PROPERTY_SCALE) {
        _scale = value;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}

void NodeLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_OPACITY) {
        float opacity = value * 255.0f;
        _opacity = (opacity<0.0f)?0:((opacity>255.0f)?255:static_cast<GLubyte>(opacity));
    } else {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}
    
void NodeLoader::onHandlePropTypeDegrees(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_ROTATION) {
        _rotation = value;
    } else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_TAG) {
        _tag = value;
    } else {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}

void NodeLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeFloatVar(const std::string &propertyName, bool isExtraProp, const Vec2 &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeAnimation(const std::string &propertyName, bool isExtraProp, int value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_VISIBLE) {
        _visible = value;
    }  else if(propertyName == PROPERTY_CASCADECOLOR) {
        _cascadeColorEnabled = value;
    } else if(propertyName == PROPERTY_CASCADEOPACITY) {
        _cascadeOpacityEnabled = value;
    } else {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}

void NodeLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeTexture(const std::string &propertyName, bool isExtraProp, const TextureDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value)
{
    if(propertyName == PROPERTY_COLOR) {
        _color = value;
    }
    else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}

void NodeLoader::onHandlePropTypeColor4FVar(const std::string &propertyName, bool isExtraProp, const std::pair<Color4F, Color4F> &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &flip)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeFntFile(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    if(propertyName == PROPERTY_NAME) {
        _name = value;
    } else
    {
        // It may be a custom property, add it to custom property dictionary.
        if(isExtraProp){
            _customProperties[propertyName] = Value(value);
        }
        else {
            ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
        }
    }
}

void NodeLoader::onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeClickCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeTouchCallback(const std::string &propertyName, bool isExtraProp, const CallbackDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}

void NodeLoader::onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, const NodeLoaderDescription &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeFloatXY(const std::string &propertyName, bool isExtraProp, const Vec2 &value)
{
    if(propertyName == PROPERTY_SKEW) {
        _skew = value;
    }
    else {
        ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
    }
}
    
void NodeLoader::onHandlePropTypeSoundFile(const std::string &propertyName, bool isExtraProp, const std::string &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value)
{
    ASSERT_FAIL_UNEXPECTED_PROPERTY(propertyName);
}
    
void NodeLoader::onLoaded()
{
    
}
    
void NodeLoader::onNodeLoaded(Node *node) const
{
    
}

    
}

NS_CC_END
