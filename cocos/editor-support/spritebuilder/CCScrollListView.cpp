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
    
ScrollListView* ScrollListView::create(NodeLoader *templateLoader)
{
    ScrollListView* widget = new (std::nothrow) ScrollListView(templateLoader);
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

ScrollListView::ScrollListView(NodeLoader *templateLoader):_template(templateLoader)
{
    CC_SAFE_RETAIN(_template);
}
    
ScrollListView::~ScrollListView()
{
    CC_SAFE_RELEASE(_template);
}
    
ui::Widget* ScrollListView::pushBackElement(spritebuilder::CCBXReaderOwner * owner)
{
    ui::Widget *ret = ui::Widget::create();
    _template->loadNode(ret, getContentSize(), 1.0f, 1.0f, owner);
    pushBackCustomItem(ret);
    return ret;
}

ui::Widget* ScrollListView::insertElement(ssize_t index, spritebuilder::CCBXReaderOwner * owner)
{
    ui::Widget *ret = ui::Widget::create();
    _template->loadNode(ret, getContentSize(), 1.0f, 1.0f, owner);
    insertCustomItem(ret, index);
    return ret;
}
    
}

NS_CC_END
