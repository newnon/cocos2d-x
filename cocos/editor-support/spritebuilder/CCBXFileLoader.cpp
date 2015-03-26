#include "CCBXFileLoader.h"
#include "2d/CCLayer.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_CCBFILE("ccbFile");
    
FileLoader *FileLoader::create()
{
    FileLoader *ret = new FileLoader();
    ret->autorelease();
    return ret;
}

Node *FileLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    if(_file)
        return _file->createNode(parentSize, mainScale, additionalScale, owner, nullptr, rootNode);
    else
        return nullptr;
}

FileLoader::FileLoader()
    :_file(nullptr)
{
    
}
    
FileLoader::~FileLoader()
{
    CC_SAFE_RELEASE(_file);
}
    
void FileLoader::onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value)
{
    if(propertyName == PROPERTY_CCBFILE) {
        _file = value;
        CC_SAFE_RETAIN(_file);
    } else {
        NodeLoader::onHandlePropTypeCCBFile(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END