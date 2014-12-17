#ifndef _CCB_CCBXREADER_H_
#define _CCB_CCBXREADER_H_

#include <string>
#include <vector>

#include "2d/CCNode.h"
#include "base/CCValue.h"
#include "2d/CCSpriteFrame.h"
#include "base/CCData.h"
#include "base/CCMap.h"
#include "base/CCVector.h"
#include "CCBXAnimationManager.h"

NS_CC_BEGIN

namespace ui { enum class WidgetTouchEventType; }

namespace spritebuilder {
    
enum class SceneScaleType
{
    NONE,
    CUSTOM,
    MINSIZE,
    MAXSIZE,
    MINSCALE,
    MAXSCALE
};
    
enum class TargetType {
    NONE = 0,
    DOCUMENT_ROOT = 1,
    OWNER = 2,
};

    
typedef std::function<void(Ref*, ui::WidgetTouchEventType)> ccReaderTouchCallback;
typedef std::function<void(Ref*)> ccReaderClickCallback;
typedef std::function<void(Node*)> ccReaderEventCallback;

class CCBXReaderOwner
{
public:
    virtual bool onAssignCCBMemberVariable(const std::string &memberVariableName, Node* node) { return false; }
    virtual bool onAssignCCBCustomProperty(const std::string &memberVariableName, const Value& value) { return false; }
    virtual ccReaderClickCallback onResolveCCBClickSelector(const std::string &selectorName, Node* node) { return ccReaderClickCallback(); }
    virtual ccReaderTouchCallback onResolveCCBTouchSelector(const std::string &selectorName, Node* node) { return ccReaderTouchCallback(); }
    virtual ccReaderEventCallback onResolveCCBCallFuncSelector(const std::string &selectorName, Node* node) { return ccReaderEventCallback(); };
    virtual ~CCBXReaderOwner() {};
    
};
    
#define CCBX_MEMBERVARIABLEASSIGNER_GLUE(MEMBERVARIABLENAME, MEMBERVARIABLE) \
    if(cocos2d::spritebuilder::membervariableAssign(MEMBERVARIABLENAME, memberVariableName, MEMBERVARIABLE, node)) {\
        return true; \
}
    
#define CCBX_MEMBERVARIABLEASSIGNER_GLUE_VECTOR(MEMBERVARIABLENAME, MEMBERVARIABLE) \
    if(cocos2d::spritebuilder::membervariableAssignVector(MEMBERVARIABLENAME, memberVariableName, MEMBERVARIABLE, node)) {\
        return true; \
}

#define CCBX_SELECTORRESOLVER_CLICK_GLUE(TARGET, SELECTORNAME, METHOD) \
    if(selectorName == SELECTORNAME) { \
        return std::bind(&METHOD, TARGET, std::placeholders::_1); \
}
    
#define CCBX_SELECTORRESOLVER_TOUCH_GLUE(TARGET, SELECTORNAME, METHOD) \
    if(selectorName == SELECTORNAME) { \
        return std::bind(&METHOD, TARGET, std::placeholders::_1, std::placeholders::_2); \
}
    
#define CCBX_SELECTORRESOLVER_EVENT_GLUE(TARGET, SELECTORNAME, METHOD) \
    if(selectorName == SELECTORNAME) { \
        return std::bind(&METHOD, TARGET, std::placeholders::_1); \
}
    
template <class T>
inline bool membervariableAssign(const std::string &name, const std::string &memberVariableName, T *&variable, Node* node)
{
    if(memberVariableName != name)
        return false;
    T *result = dynamic_cast<T*>(node);
    if(result)
    {
        variable = result;
        return true;
    }
    return false;
}
    
template <class T>
inline bool membervariableAssignVector(const std::string &name, const std::string &memberVariableName, Vector<T*> &variable, Node* node)
{
    if(memberVariableName != name)
        return false;
    T *result = dynamic_cast<T*>(node);
    if(result)
    {
        variable.pushBack(result);
        return true;
    }
    return false;
}
    
class NodeLoader;
class NodeLoaderLibrary;
class CCBReaderParams;
    
typedef std::function<Node*(const Size &parentSize, float mainScale, float additionalScale)> CreateNodeFunction;

class CCBXReader : public Ref
{
public:
    
    static CCBXReader* createFromFile(const std::string &pCCBFileName, const std::string &rootPath = "", const NodeLoaderLibrary *library = nullptr);
    static CCBXReader* createFromData(const Data &data, const std::string &rootPath = "", const NodeLoaderLibrary *library = nullptr);
    
    Node *createNode(CCBXReaderOwner *pOwner = nullptr, SceneScaleType scaleType = SceneScaleType::NONE, const CreateNodeFunction &createNodeFunction = nullptr, const std::function<void(cocos2d::Node*, AnimationCompleteType)> &defaultAnimationCallback = nullptr) const;
    
    static float getResolutionScale();
    static void setResolutionScale(float scale);
    
    const std::string& getRootPath();
    
    CCBReaderParams* getParams() const;
    
    void calcScales(SceneScaleType scaleType, float &mainScale, float &additionalScale) const;
    
CC_CONSTRUCTOR_ACCESS:
    CCBXReader(const std::string &rootPath, CCBReaderParams *params, NodeLoader *rootNodeLoader);
    virtual ~CCBXReader();
    
private:
    static CCBReaderParams* createParams(const std::string &rootPath);
    static Map<std::string,CCBReaderParams*> _paramsMap;
    std::string _rootPath;
    NodeLoader *_rootNodeLoader;
    CCBReaderParams *_params;
    static float _resolutionScale;
};

}

NS_CC_END

#endif
