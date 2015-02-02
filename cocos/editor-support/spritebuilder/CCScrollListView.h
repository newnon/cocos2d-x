#ifndef _CCB_CCSCROLLLISTVIEW_H_
#define _CCB_CCSCROLLLISTVIEW_H_

#include "ui/UIScrollView.h"
#include "SpriteBuilder.h"
#include "base/CCRef.h"

NS_CC_BEGIN
namespace spritebuilder {

//#include "cocos-ext.h"

typedef std::function<void(const std::string&, const std::string&, Ref*)> ListViewDelegate;

class ScrollListView : public ui::ScrollView, public spritebuilder::CCBXReaderOwner
{
public:
    struct ItemStruct
    {
        cocos2d::Node* item;
        std::map<std::string,cocos2d::Node*> variables;
    };
    void setDelegate(const ListViewDelegate& delegate);
    static ScrollListView *create();
    virtual bool init();
    ScrollListView();
    ~ScrollListView();
    ItemStruct *addElement(const std::string &name);
    ItemStruct *getElement(const std::string &name);
    bool removeElement(const std::string &name);
    void clear();
    void move(int elements);
    void prepare();
private:
    struct IndexItem
    {
        std::string item;
        std::string selector;
    };
    std::list<int> _movement;
    bool _inMove;
    std::map<std::string, ItemStruct> _items;
    std::map<std::string, ItemStruct>::iterator _curitem;
    
    cocos2d::Data _data;
    
    void moveContent(cocos2d::Point offset, float dt);
    void stoppedMove();
    void internalMove(int elements);

    
    ListViewDelegate _delegate;
    
    virtual bool onAssignCCBMemberVariable(const std::string &memberVariableName, Node* node) override;
    virtual ccReaderClickCallback onResolveCCBClickSelector(const std::string &selectorName, Node* node) override;
    
    virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
    
    void RecalcPositions();
    
    NodeLoader * _template;
    CC_PROPERTY(bool, _horizontal, Horizontal);
    CC_PROPERTY(cocos2d::TextVAlignment, _verticalTextAlignment, VerticalTextAlignment);
    CC_PROPERTY(cocos2d::TextHAlignment, _textAlignment, TextAlignment);
};

}

NS_CC_END

#endif
