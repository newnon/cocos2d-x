#ifndef _CCB_CCSCROLLLISTVIEW_H_
#define _CCB_CCSCROLLLISTVIEW_H_

#include "ui/UIListView.h"
#include "SpriteBuilder.h"
#include "base/CCRef.h"

NS_CC_BEGIN
namespace spritebuilder {

class CC_DLL ScrollListView : public ui::ListView
{
public:
    static ScrollListView *create(NodeLoader *templateLoader);
    
    /**
     * Push back custom item into listview.
     */
    ui::Widget* pushBackElement(spritebuilder::CCBXReaderOwner * owner);
    
    /**
     * Insert custom item into listview.
     */
    ui::Widget* insertElement(ssize_t index, spritebuilder::CCBXReaderOwner * owner);
    
CC_CONSTRUCTOR_ACCESS:
    ScrollListView(NodeLoader *templateLoader);
    ~ScrollListView();
    
private:
    NodeLoader * _template;
};

}

NS_CC_END

#endif
