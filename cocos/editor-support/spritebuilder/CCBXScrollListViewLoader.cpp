#include "CCBXScrollListViewLoader.h"
#include "ui/UIScrollView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static std::string PROPERTY_DIRECTION("direction");
static std::string PROPERTY_BOUNCES("bounces");
static std::string PROPERTY_TEMPLATE("template");
static std::string PROPERTY_VERTICALSCROLLENABLED("verticalScrollEnabled");
static std::string PROPERTY_HORIZONTALSCROLLENABLED("horizontalScrollEnabled");
static std::string PROPERTY_PAGINGENABLED("pagingEnabled");
static std::string PROPERTY_CLIPCONTENT("clipContent");
    
ScrollListViewLoader *ScrollListViewLoader::create()
{
    ScrollListViewLoader *ret = new ScrollListViewLoader();
    ret->autorelease();
    return ret;
}

Node *ScrollListViewLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    ui::ScrollView *scrollView = ui::ScrollView::create();
    scrollView->setClippingType(ui::Layout::ClippingType::SCISSOR);
    return scrollView;
}

void ScrollListViewLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
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

ScrollListViewLoader::ScrollListViewLoader():_clipping(false),_bounce(false),_file(nullptr)
{
    
}
    
ScrollListViewLoader::~ScrollListViewLoader()
{
    CC_SAFE_RELEASE(_file);
}
    
void ScrollListViewLoader::onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value)
{
    WidgetLoader::onHandlePropTypeSize(propertyName, isExtraProp, value);
}

void ScrollListViewLoader::onHandlePropTypeCCBFile(const std::string &propertyName, bool isExtraProp, NodeLoader *value)
{
    if(propertyName == PROPERTY_TEMPLATE) {
        _file = value;
        CC_SAFE_RETAIN(_file);
    } else {
        WidgetLoader::onHandlePropTypeCCBFile(propertyName, isExtraProp, value);
    }
}
    
void ScrollListViewLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
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
