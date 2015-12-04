#include "CCBXScrollListViewLoader.h"
#include "CCScrollListView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_BOUNCES("bounces");
static const std::string PROPERTY_TEMPLATE("template");
static const std::string PROPERTY_GRAVITY("gravity");
static const std::string PROPERTY_MAGNETIC("magnetic");
static const std::string PROPERTY_CLIPCONTENT("clipContent");
static const std::string PROPERTY_HORIZONRAL("horizontal");
static const std::string PROPERTY_INERTIAL_SCROLL("inertialScroll");
static const std::string PROPERTY_SCROLL_BAR_ENABLED("scrollBarEnabled");
    
ScrollListViewLoader *ScrollListViewLoader::create()
{
    ScrollListViewLoader *ret = new ScrollListViewLoader();
    ret->autorelease();
    return ret;
}

Node *ScrollListViewLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner)
{
    ScrollListView *scrollListView = ScrollListView::create(_file, mainScale, additionalScale);
    scrollListView->setAnchorPoint(Vec2(0.0f, 0.0f));
    scrollListView->setClippingType(ui::Layout::ClippingType::SCISSOR);
    return scrollListView;
}

void ScrollListViewLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner)
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, rootOwner);
    ScrollListView *scrollView = static_cast<ScrollListView*>(node);
    scrollView->setBounceEnabled(_bounce);
    scrollView->setClippingEnabled(_clipping);
    scrollView->setDirection(_horizontal?ScrollListView::Direction::HORIZONTAL:ScrollListView::Direction::VERTICAL);
    scrollView->setGravity(static_cast<cocos2d::ui::ListView::Gravity>(_gravity));
    scrollView->setMagneticType(static_cast<cocos2d::ui::ListView::MagneticType>(_magnetic));
    scrollView->setInertiaScrollEnabled(_inertial);
    scrollView->setScrollBarEnabled(_scrollBar);
}

ScrollListViewLoader::ScrollListViewLoader():_horizontal(false),_clipping(true),_bounce(false),_file(nullptr),_gravity(0), _magnetic(0),_inertial(true),_scrollBar(false)
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
    
void ScrollListViewLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_GRAVITY) {
        _gravity = value;
    } else if(propertyName == PROPERTY_MAGNETIC) {
        _magnetic = value;
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}
    
void ScrollListViewLoader::onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value)
{
    if(propertyName == PROPERTY_CLIPCONTENT) {
        _clipping = value;
    } else if(propertyName == PROPERTY_INERTIAL_SCROLL) {
        _inertial = value;
    } else if(propertyName == PROPERTY_SCROLL_BAR_ENABLED) {
        _scrollBar = value;
    } else if(propertyName == PROPERTY_BOUNCES) {
        _bounce = value;
    } else if(propertyName == PROPERTY_HORIZONRAL) {
        _horizontal = value;
    } else {
        WidgetLoader::onHandlePropTypeCheck(propertyName, isExtraProp, value);
    }
}

}
    
NS_CC_END
