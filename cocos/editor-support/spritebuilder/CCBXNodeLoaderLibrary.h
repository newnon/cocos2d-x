//
//  CCBXNodeLoaderLibrary.h
//  cocos2d_libs
//
//  Created by Sergey on 29.10.14.
//
//

#ifndef __cocos2d_libs__CCBXNodeLoaderLibrary__
#define __cocos2d_libs__CCBXNodeLoaderLibrary__

#include "base/CCRef.h"
#include <functional>
#include <map>
#include <string>

NS_CC_BEGIN
namespace spritebuilder {
    
class NodeLoader;

class NodeLoaderLibrary : public Ref
{
public:
    typedef std::function<NodeLoader *()> NodeLoaderCreateFuntion;
    
    static NodeLoaderLibrary *create();
    static NodeLoaderLibrary *createDefault();
    static NodeLoaderLibrary *getDefault();
    
    NodeLoader *createNodeLoader(const std::string &name) const;
    void registerNodeLoader(const std::string &name, const NodeLoaderCreateFuntion &func);
    
CC_CONSTRUCTOR_ACCESS:
    NodeLoaderLibrary();
    
private:
    std::map<std::string, NodeLoaderCreateFuntion> _loaders;
    void registerDefaultLoaders();
};
    
}

NS_CC_END

#endif /* defined(__cocos2d_libs__CCBXNodeLoaderLibrary__) */
