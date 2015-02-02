//
//  RjScrollListView.cpp
//  inferno
//
//  Created by user-i121 on 03.07.13.
//
//

#include "CCScrollListView.h"
#include "platform/CCFileUtils.h"
#include "2d/CCActionInterval.h"
#include "2d/CCActionInstant.h"
#include "base/CCDirector.h"
#include "CCBXNodeLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

void ScrollListView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
{
    for (auto node : getInnerContainer()->getChildren())
    {
        if((node->getPosition().x+node->getContentSize().width)<-getInnerContainer()->getPosition().x)
            node->setVisible(false);
        else if((node->getPosition().x)>(getContentSize().width-getInnerContainer()->getPosition().x))
            node->setVisible(false);
        else
            node->setVisible(true);
    }
    ScrollView::visit(renderer, parentTransform, parentFlags);
}

void ScrollListView::prepare()
{
    //ScrollView::visit();
}

void ScrollListView::RecalcPositions()
{
    if(getInnerContainer()->getChildren().empty())
        return;
    size_t num=0;
    size_t count = getInnerContainer()->getChildren().size();
    Node *node =getInnerContainer()->getChildren().at(0);
    Size contentsize = node->getContentSize();
    Size viewsize = getContentSize();
    
    float yoffset = 0;
    switch (_verticalTextAlignment) {
        case TextVAlignment::TOP:
        {
            yoffset = 0;
            break;
        }
        case TextVAlignment::BOTTOM:
        {
            if (_horizontal) {
                yoffset = viewsize.height - contentsize.height;
            }
            else {
                yoffset = contentsize.height*count/2;
            }
            break;
        }
        case TextVAlignment::CENTER:
        {
            if (_horizontal) {
                yoffset = viewsize.height/2 - contentsize.height/2;
            }
            else {
                yoffset = viewsize.height/2 - contentsize.height*count/2;
            }
            break;
        }
        default:
            break;
    }
    
    if(_horizontal)
    {
        float containerSize = 0;
        for (auto pChild : getInnerContainer()->getChildren())
        {
            pChild->setPosition(Point(pChild->getContentSize().width*num,0));
            containerSize += pChild->getContentSize().width;
            ++num;
        }
        
        float xoffset = 0;
        switch (_textAlignment) {
            case TextHAlignment::CENTER:
            {
                xoffset = viewsize.width/2 - containerSize/2;
                break;
            }
            case TextHAlignment::LEFT:
            {
                xoffset = 0;
                break;
            }
            case TextHAlignment::RIGHT:
            {
                xoffset = viewsize.width - contentsize.width;
                break;
            }
            default:
                break;
        }
        
        //setContentOffset(Point(xoffset, (viewsize.height-contentsize.height) - yoffset), false);
        setContentSize(Size(std::max(contentsize.width*count,viewsize.width),std::max(contentsize.height,viewsize.height)));
        if((contentsize.width*count)<viewsize.width)
            setDirection(ScrollView::Direction::NONE);
        else
            setDirection(ScrollView::Direction::HORIZONTAL);
    }
    else
    {
        for (auto pChild : getInnerContainer()->getChildren())
        {
            pChild->setPosition(Point(0,pChild->getContentSize().height*(count-num-1)));
            ++num;
        }
        
        float xoffset = 0;
        switch (_textAlignment) {
            case TextHAlignment::CENTER:
            {
                xoffset = viewsize.width/2 - contentsize.width/2;
                break;
            }
            case TextHAlignment::LEFT:
            {
                xoffset = 0;
                break;
            }
            case TextHAlignment::RIGHT:
            {
                xoffset = viewsize.width - contentsize.width;
                break;
            }
            default:
                break;
        }
        
        //setContentOffset(Point(xoffset, (viewsize.height-contentsize.height*count) - yoffset), false);
        setContentSize(Size(std::max(contentsize.width,viewsize.width),std::max(contentsize.height*count,viewsize.height)));
        if((contentsize.height*count)<viewsize.height)
            setDirection(ScrollView::Direction::NONE);
        else
            setDirection(ScrollView::Direction::VERTICAL);
    }
}

void ScrollListView::setDelegate(const ListViewDelegate& delegate)
{
    _delegate = delegate;
}
    
bool ScrollListView::onAssignCCBMemberVariable(const std::string &memberVariableName, Node* node)
{
    if(_curitem==_items.end())
        return false;
    _curitem->second.variables[memberVariableName] = node;
    return true;
}

ccReaderClickCallback ScrollListView::onResolveCCBClickSelector(const std::string &selectorName, Node* node)
{
    if(_curitem==_items.end())
        return nullptr;
    return [this, selectorName](Ref* ref)
    {
        if(_delegate)
            _delegate(_curitem->first, selectorName, ref);
            
    };
}

ScrollListView::ScrollListView():_curitem(_items.end()),_inMove(false)
,_textAlignment(cocos2d::TextHAlignment::LEFT)
,_horizontal(false)
{
}
ScrollListView::~ScrollListView()
{
}
bool ScrollListView::init()
{
    if(!ScrollView::init())
        return false;
    setDirection(ScrollView::Direction::VERTICAL);
    return true;
}
ScrollListView::ItemStruct *ScrollListView::addElement(const std::string &name)
{
    if(_items.find(name)!=_items.end())
        return NULL;
    _curitem = _items.insert(std::pair<std::string, ItemStruct>(name,ItemStruct())).first;

    Node *node = _template->createNode(getContentSize(), 1.0, 1.0, this);
    
    if(!node)
        return NULL;
    node->retain();
    _curitem->second.item=node;
    getInnerContainer()->addChild(node);
    RecalcPositions();
    return &_curitem->second;
}
    
ScrollListView::ItemStruct * ScrollListView::getElement(const std::string &name)
{
    auto elemIt=_items.find(name);
    return elemIt==_items.end() ? nullptr : &elemIt->second;
}
    
bool ScrollListView::removeElement(const std::string &name)
{
    auto elemIt=_items.find(name);
    if(elemIt==_items.end())
        return false;
    elemIt->second.item->removeFromParent();
    elemIt->second.item->release();
    _items.erase(elemIt);
    return true;
}

void ScrollListView::clear()
{
    for(auto it=_items.begin();it!=_items.end();++it)
    {
        it->second.item->removeFromParent();
        it->second.item->release();
    }
    _items.clear();
}

void ScrollListView::setVerticalTextAlignment(cocos2d::TextVAlignment alignment)
{
    _verticalTextAlignment = alignment;
}

TextVAlignment ScrollListView::getVerticalTextAlignment()
{
    return _verticalTextAlignment;
}

void ScrollListView::setTextAlignment(cocos2d::TextHAlignment alignment)
{
    _textAlignment = alignment;
}

TextHAlignment ScrollListView::getTextAlignment()
{
    return _textAlignment;
}

bool ScrollListView::getHorizontal()
{
    return _horizontal;
}

void ScrollListView::setHorizontal(bool value)
{
    if(value)
        this->setDirection(ScrollView::Direction::HORIZONTAL);
    else
        this->setDirection(ScrollView::Direction::VERTICAL);
    _horizontal = value;
}

void ScrollListView::moveContent(cocos2d::Point offset, float dt)
{
    //_scrollDistance = Point::ZERO;
    FiniteTimeAction *scroll, *expire;
    scroll = MoveBy::create(dt, offset);
    expire = CallFunc::create(std::bind(&ScrollListView::stoppedMove,this));
    //_container->runAction(Sequence::create(scroll, expire, NULL));
}

void ScrollListView::stoppedMove()
{
    if(_movement.empty())
    {
        _inMove=false;
        return;
    }
    internalMove(_movement.front());
    _movement.pop_front();
}

void ScrollListView::internalMove(int elements)
{
    /*size_t count = getContainer()->getChildren().size();
    if(getContainer()->getChildren().empty())
        return;
    Node * node= getContainer()->getChildren().at(0);
    
    float offsetSize = node->getContentSize().width;
    if (!_horizontal)
        offsetSize = node->getContentSize().height;
    
    if(_horizontal && ((offsetSize*count) < getViewSize().width))
        return;
    
    if(!_horizontal && ((offsetSize*count) < getViewSize().height))
        return;
    
    Point offset = Point::ZERO;
    if (_horizontal)
        offset = Point(offsetSize, 0);
    else
        offset = Point(0, offsetSize);
    
    offset = offset * elements;
    offset = offset * -1.0f;
    
    if (-getContentOffset().x + getViewSize().width - offset.x > getContentSize().width)
        offset.x = -(getContentSize().width-getViewSize().width+getContentOffset().x);
    if (-getContentOffset().x -offset.x < 0)
        offset.x=-getContentOffset().x;
    
    int div = (int)(getContentOffset().x + offset.x) % (int) node->getContentSize().width;
    
    if (div) {
        if (offset.x < 0)
            offset.x -= div;
        if (offset.x > 0)
            offset.x -= div + node->getContentSize().width;
    }
    
    _inMove=true;
    
    this->moveContent(offset, 0.25f);*/
}

void ScrollListView::move(int elements)
{
    /*if(_inMove)
    {
        _movement.push_back(elements);
        return;
    }
    internalMove(elements);*/
}
    
}

NS_CC_END
