#include "CCBXScrollViewLoader.h"
#include "ui/UIScrollView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_CONTAINER("container");
static std::string PROPERTY_DIRECTION("direction");
static std::string PROPERTY_CLIPSTOBOUNDS("clipsToBounds");
static std::string PROPERTY_BOUNCES("bounces");
static std::string PROPERTY_SCALE("scale");
static std::string PROPERTY_CONTENTNODE("contentNode");
static std::string PROPERTY_VERTICALSCROLLENABLED("verticalScrollEnabled");
static std::string PROPERTY_HORIZONTALSCROLLENABLED("horizontalScrollEnabled");
static std::string PROPERTY_PAGINGENABLED("pagingEnabled");
    
ScrollViewLoader *ScrollViewLoader::create()
{
    ScrollViewLoader *ret = new ScrollViewLoader();
    ret->autorelease();
    return ret;
}

Node *ScrollViewLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    ui::ScrollView *scrollView = ui::ScrollView::create();
    scrollView->setClippingType(ui::Layout::ClippingType::SCISSOR);
    return scrollView;
}

void ScrollViewLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::ScrollView *scrollView = static_cast<ui::ScrollView*>(node);
}

ScrollViewLoader::ScrollViewLoader()
{
    
}
    
ScrollViewLoader::~ScrollViewLoader()
{
    
}
    
void ScrollViewLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    WidgetLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
}

void ScrollViewLoader::onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value)
{
    if(propertyName == PROPERTY_CONTENTNODE) {
    } else {
        WidgetLoader::onHandlePropTypeCCBFile(propertyName, isExtraProp, value);
    }
}
    
void ScrollViewLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_CLIPSTOBOUNDS) {
        //((ScrollView *)pNode)->setClippingToBounds(pCheck);
    } else if(propertyName == PROPERTY_BOUNCES) {
        //((ScrollView *)pNode)->setBounceable(pCheck);
    } else if(propertyName == PROPERTY_VERTICALSCROLLENABLED) {
        //_verticalScrollEnabled = pCheck;
    } else if(propertyName == PROPERTY_HORIZONTALSCROLLENABLED) {
        //_horizontalScrollEnabled = pCheck;
    } else if(propertyName == PROPERTY_PAGINGENABLED) {
        //_horizontalScrollEnabled = pCheck;
    } else{
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}

void ScrollViewLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
}

void ScrollViewLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    
}

    
/*
void ScrollViewLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _verticalScrollEnabled = false;
    _horizontalScrollEnabled = false;
    pNode->ignoreAnchorPointForPosition(false);
}

void ScrollViewLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    if(ccbReader->getVersion()>5)
    {
        if(_verticalScrollEnabled)
        {
            if(_horizontalScrollEnabled)
                ((ScrollView *)pNode)->setDirection(ScrollView::Direction::BOTH);
            else
                ((ScrollView *)pNode)->setDirection(ScrollView::Direction::VERTICAL);
        }
        else
        {
            if(_horizontalScrollEnabled)
                ((ScrollView *)pNode)->setDirection(ScrollView::Direction::HORIZONTAL);
            else
                ((ScrollView *)pNode)->setDirection(ScrollView::Direction::NONE);
        }
    }
}

void ScrollViewLoader::onHandlePropTypeSize(Node * pNode, Node * pParent, const char * pPropertyName, const Size &pSize, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_CONTENTSIZE) == 0) {
        ((ScrollView *)pNode)->setViewSize(pSize);
    } else {
        NodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, ccbReader);
    }
}

void ScrollViewLoader::onHandlePropTypeCheck(Node * pNode, Node * pParent, const char * pPropertyName, bool pCheck, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_CLIPSTOBOUNDS) == 0) {
        ((ScrollView *)pNode)->setClippingToBounds(pCheck);
    } else if(strcmp(pPropertyName, PROPERTY_BOUNCES) == 0) {
        ((ScrollView *)pNode)->setBounceable(pCheck);
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALSCROLLENABLED) == 0) {
        _verticalScrollEnabled = pCheck;
    } else if(strcmp(pPropertyName, PROPERTY_HORIZONTALSCROLLENABLED) == 0) {
        _horizontalScrollEnabled = pCheck;
    } else {
        NodeLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, ccbReader);
    }
}

void ScrollViewLoader::onHandlePropTypeCCBFile(Node * pNode, Node * pParent, const char * pPropertyName, Node * pCCBFileNode, CCBReader * ccbReader) {
    if((strcmp(pPropertyName, PROPERTY_CONTAINER) == 0) || (strcmp(pPropertyName, PROPERTY_CONTENTNODE) == 0)) {
        ((ScrollView *)pNode)->setContainer(pCCBFileNode);
        ((ScrollView *)pNode)->updateInset();
    } else {
        NodeLoader::onHandlePropTypeCCBFile(pNode, pParent, pPropertyName, pCCBFileNode, ccbReader);
    }
}

void ScrollViewLoader::onHandlePropTypeFloat(Node * pNode, Node * pParent, const char * pPropertyName, float pFloat, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_SCALE) == 0) {
        ((ScrollView *)pNode)->setScale(pFloat);
    } else {
        NodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, ccbReader);
    }
}

void ScrollViewLoader::onHandlePropTypeIntegerLabeled(Node * pNode, Node * pParent, const char * pPropertyName, int pIntegerLabeled, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_DIRECTION) == 0) {
        ((ScrollView *)pNode)->setDirection(ScrollView::Direction(pIntegerLabeled));
    } else {
        NodeLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pIntegerLabeled, ccbReader);
    }
}
*/

}
    
NS_CC_END
