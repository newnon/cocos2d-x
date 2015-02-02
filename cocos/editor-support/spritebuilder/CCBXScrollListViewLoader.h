#ifndef _CCBX_CCSCROLLLISTVIEWLOADER_H_
#define _CCBX_CCSCROLLLISTVIEWLOADER_H_

#include "base/CCRef.h"

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class ScrollListViewLoader : public WidgetLoader {
public:
    
    static ScrollListViewLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    ScrollListViewLoader();
    ~ScrollListViewLoader();

protected:
    
    virtual void onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value) override;
    virtual void onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value) override;
    virtual void onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value) override;
private:
    bool _verticalScrollEnabled;
    bool _horizontalScrollEnabled;
    bool _clipping;
    bool _bounce;
    NodeLoader *_file;
};

}

NS_CC_END

#endif
