#ifndef _CCBX_FILELOADERLOADER_H_
#define _CCBX_FILELOADERLOADER_H_

#include "CCBXNodeLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class FileLoader : public NodeLoader {
public:
    static FileLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    
CC_CONSTRUCTOR_ACCESS:
    FileLoader();
    ~FileLoader();

protected:
    virtual void onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value) override;
    
private:
    NodeLoader *_file;
};

}

NS_CC_END

#endif
