#include "CCBXScrollViewLoader.h"
#include "ui/UIScrollView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_DIRECTION("direction");
static std::string PROPERTY_BOUNCES("bounces");
static std::string PROPERTY_CONTENTNODE("contentNode");
static std::string PROPERTY_VERTICALSCROLLENABLED("verticalScrollEnabled");
static std::string PROPERTY_HORIZONTALSCROLLENABLED("horizontalScrollEnabled");
static std::string PROPERTY_PAGINGENABLED("pagingEnabled");
static std::string PROPERTY_CLIPCONTENT("clipContent");
    
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
    ui::ScrollView::Direction direction = ui::ScrollView::Direction::NONE;
    if(_horizontalScrollEnabled && _verticalScrollEnabled)
        direction = ui::ScrollView::Direction::BOTH;
    else if(_horizontalScrollEnabled)
        direction = ui::ScrollView::Direction::HORIZONTAL;
    else if(_verticalScrollEnabled)
        direction = ui::ScrollView::Direction::VERTICAL;
    scrollView->setBounceEnabled(_bounce);
    scrollView->setClippingEnabled(_clipping);
    scrollView->setDirection(direction);
    if(_file)
    {
        Node *childNode = _file->createNode(parentSize, mainScale, additionalScale);
        scrollView->setInnerContainerSize(childNode->getContentSize());
        scrollView->getInnerContainer()->addChild(childNode);
        scrollView->setEnabled(true);
    }
}

ScrollViewLoader::ScrollViewLoader():_clipping(false),_bounce(false),_file(nullptr)
{
    
}
    
ScrollViewLoader::~ScrollViewLoader()
{
    CC_SAFE_RELEASE(_file);
}
    
void ScrollViewLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    WidgetLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
}

void ScrollViewLoader::onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value)
{
    if(propertyName == PROPERTY_CONTENTNODE) {
        _file = value;
        CC_SAFE_RETAIN(_file);
    } else {
        WidgetLoader::onHandlePropTypeCCBFile(propertyName, isExtraProp, value);
    }
}
    
void ScrollViewLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_CLIPCONTENT) {
        _clipping = value;
    } else if(propertyName == PROPERTY_BOUNCES) {
        _bounce = value;
    } else if(propertyName == PROPERTY_VERTICALSCROLLENABLED) {
        _verticalScrollEnabled = value;
    } else if(propertyName == PROPERTY_HORIZONTALSCROLLENABLED) {
        _horizontalScrollEnabled = value;
    } else if(propertyName == PROPERTY_PAGINGENABLED) {
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}

}
    
NS_CC_END
