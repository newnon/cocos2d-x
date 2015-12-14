#include "CCBXScrollViewLoader.h"
#include "ui/UIScrollView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_BOUNCES("bounces");
static const std::string PROPERTY_CONTENTNODE("contentNode");
static const std::string PROPERTY_VERTICALSCROLLENABLED("verticalScrollEnabled");
static const std::string PROPERTY_HORIZONTALSCROLLENABLED("horizontalScrollEnabled");
static const std::string PROPERTY_PAGINGENABLED("pagingEnabled");
static const std::string PROPERTY_CLIPCONTENT("clipContent");
static const std::string PROPERTY_INERTIAL_SCROLL("inertialScroll");
static const std::string PROPERTY_SCROLL_BAR_ENABLED("scrollBarEnabled");
    
ScrollViewLoader *ScrollViewLoader::create()
{
    ScrollViewLoader *ret = new ScrollViewLoader();
    ret->autorelease();
    return ret;
}

Node *ScrollViewLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner)
{
    ui::ScrollView *scrollView = ui::ScrollView::create();
    scrollView->setAnchorPoint(Vec2(0.0f, 0.0f));
    scrollView->setClippingType(ui::Layout::ClippingType::SCISSOR);
    return scrollView;
}

void ScrollViewLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner)
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, rootOwner);
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
        Node *childNode = _file->createNode(parentSize, mainScale, additionalScale, owner);
        scrollView->setInnerContainerSize(childNode->getContentSize());
        scrollView->getInnerContainer()->addChild(childNode);
        scrollView->setEnabled(true);
    }
    scrollView->setInertiaScrollEnabled(_inertial);
    scrollView->setScrollBarEnabled(_scrollBar);
}

ScrollViewLoader::ScrollViewLoader():_clipping(true),_bounce(false),_file(nullptr), _verticalScrollEnabled(true), _horizontalScrollEnabled(true),_inertial(true),_scrollBar(false)
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
    } else if(propertyName == PROPERTY_INERTIAL_SCROLL) {
        _inertial = value;
    } else if(propertyName == PROPERTY_SCROLL_BAR_ENABLED) {
        _scrollBar = value;
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
