#include "UIEditBoxImpl-emcc.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)

#include "UIEditBox.h"
#include "base/CCDirector.h"
#include "2d/CCLabel.h"

#include "base/CCEventDispatcher.h"
#include "base/CCEventListenerKeyboard.h"
#include "base/CCEventListenerCustom.h"
#include "base/CCScheduler.h"
#include "base/CCEventListenerTouch.h"

#include <emscripten/emscripten.h>

NS_CC_BEGIN

extern "C"
{
    int UIEditBox_init(bool isMultiLine);
    void UIEditBox_setContentSize(int id, int x, int y, int width, int height);
    void UIEditBox_setColor(int id, int r, int g, int b);
    void UIEditBox_setFont(int id, const char* fontName, int size);
    void UIEditBox_setText(int id, const char* str);
    void UIEditBox_setInputType(int id, int type);
    void UIEditBox_setMaxLength(int id, int length);
    void UIEditBox_setVisible(int id, bool visible);
    const char* UIEditBox_getText(int id);
    bool UIEditBox_isFocused(int id);
    void UIEditBox_close(int id);
    void UIEditBox_blur(int id);
    
    typedef void (*EventCallback)(void *userData);
    void UIEditBox_setCallback(int id, void *userData, EventCallback callback, EventCallback focusCallback);
}

namespace ui {

EditBoxImpl* __createSystemEditBox(EditBox* pEditBox)
{
    return new EditBoxImplEmcc(pEditBox);
}

EditBoxImplEmcc::EditBoxImplEmcc(EditBox* pEditText)
: EditBoxImplCommon(pEditText)
, _anchorPoint(Vec2(0.5f, 0.5f))
{
    _beforeDraw = Director::getInstance()->getEventDispatcher()->addCustomEventListener(Director::EVENT_BEFORE_DRAW, [this](EventCustom *event) {
        _drawedLastFrame = _drawedThisFrame;
        _drawedThisFrame = false;
    });
    
    _afterDraw = Director::getInstance()->getEventDispatcher()->addCustomEventListener(Director::EVENT_AFTER_DRAW, [this](EventCustom *event) {
        if(_drawedThisFrame != _drawedLastFrame)
        {
            UIEditBox_setVisible(_id, _drawedThisFrame);
        }
    });
    
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = [this](Touch*, Event*)
    {
        UIEditBox_blur(_id);
        return false;
    };

    
    Director::getInstance()->getEventDispatcher()->addEventListenerWithFixedPriority(_touchListener, -1);
}

EditBoxImplEmcc::~EditBoxImplEmcc()
{
    Director::getInstance()->getEventDispatcher()->removeEventListener(_beforeDraw);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_afterDraw);
    Director::getInstance()->getEventDispatcher()->removeEventListener(_touchListener);
}

void EditBoxImplEmcc::createNativeControl(const Rect& frame)
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    float factor = cocos2d::Director::getInstance()->getContentScaleFactor();

    _fontSize = ((_contentSize.height * glview->getScaleY()) / factor) * 2 / 3;
    _colText = Color4B(255, 255, 255, 255);
    _colPlaceHolder = Color4B(255, 255, 255, 255);
    _editBoxInputFlag = EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS;
    _editBoxInputMode = EditBox::InputMode::ANY;
}

void EditBoxImplEmcc::setFont(const char* pFontName, int fontSize)
{
    EditBoxImplCommon::setFont(pFontName, fontSize);
    
    if (fontSize == -1)
    {
        fontSize = _contentSize.height * 2 / 3;
    }
    else
    {
        auto glview = cocos2d::Director::getInstance()->getOpenGLView();
        float scaleFactor = glview->getScaleX();
        fontSize = fontSize * scaleFactor;
    }
    
    std::string fontName(pFontName);
    size_t pos = fontName.rfind(".");
    if (pos != std::string::npos)
    {
        fontName = fontName.substr(0, pos);
    }
    
    pos = fontName.rfind("/");
    if (pos != std::string::npos)
    {
        fontName = fontName.substr(pos + 1, fontName.length() - (pos + 1));
    }
    
    _fontName = fontName;
    _fontSize = fontSize;
    
    _label->setTTFConfig(TTFConfig(pFontName, _fontSize));
}

void EditBoxImplEmcc::setPlaceholderFont(const char* pFontName, int fontSize)
{
    _labelPlaceHolder->setTTFConfig(TTFConfig(pFontName, _fontSize));
}

void EditBoxImplEmcc::setNativeFontColor(const Color4B& color)
{
}

void EditBoxImplEmcc::setNativePlaceholderFont(const char* pFontName, int fontSize)
{
}

void EditBoxImplEmcc::setNativePlaceholderFontColor(const Color4B& color)
{
}

void EditBoxImplEmcc::setNativeInputMode(EditBox::InputMode inputMode)
{
}

void EditBoxImplEmcc::setNativeInputFlag(EditBox::InputFlag inputFlag)
{
}

void EditBoxImplEmcc::setNativeReturnType(EditBox::KeyboardReturnType returnType)
{
}

void EditBoxImplEmcc::setNativeTextHorizontalAlignment(cocos2d::TextHAlignment alignment)
{
}

void EditBoxImplEmcc::setNativeText(const char* pText)
{
    UIEditBox_setText(_id, pText);
}
    
void EditBoxImplEmcc::setNativePlaceHolder(const char* pText)
{
}

void EditBoxImplEmcc::setNativeVisible(bool visible)
{
    UIEditBox_setVisible(_id, visible);
}

void EditBoxImplEmcc::updateNativeFrame(const Rect& rect)
{
    _position = rect.origin;
    _contentSize = rect.size;
    adjustTextFieldPositionAndSize();
}

const char* EditBoxImplEmcc::getNativeDefaultFontName()
{
    return "";
}

void EditBoxImplEmcc::nativeOpenKeyboard()
{
    createFromJS();
}

void EditBoxImplEmcc::nativeCloseKeyboard()
{
    _text = getNativeText();
    UIEditBox_close(_id);
}

void EditBoxImplEmcc::setNativeMaxLength(int maxLength)
{
}
    
bool EditBoxImplEmcc::isEditing()
{
    return UIEditBox_isFocused(_id);
}

void EditBoxImplEmcc::setPosition(const Vec2& pos)
{
    _position = pos;
    adjustTextFieldPositionAndSize();
}

void EditBoxImplEmcc::setContentSize(const Size& size)
{
    EditBoxImplCommon::setContentSize(size);
    adjustTextFieldPositionAndSize();
}

void EditBoxImplEmcc::setAnchorPoint(const Vec2& anchorPoint)
{
    _anchorPoint = anchorPoint;
    setPosition(_position);
}

const char* EditBoxImplEmcc::getNativeText(void)
{
    const char *textPtr = UIEditBox_getText(_id);
    if (textPtr)
    {
        _text = textPtr;
        free((void*)textPtr);
    }
    else
    {
        _text.clear();
    }
    return _text.c_str();
}

    
void EditBoxImplEmcc::updatePosition(float dt)
{
    adjustTextFieldPositionAndSize();
}
 
void EditBoxImplEmcc::onEnter(void)
{
    adjustTextFieldPositionAndSize();
}
    
void EditBoxImplEmcc::onEnterCallback(void *userData)
{
    EditBoxImplEmcc *editBoxEmcc = static_cast<EditBoxImplEmcc*>(userData);
    editBoxEmcc->editBoxEditingDidEnd(editBoxEmcc->getNativeText(), EditBoxDelegate::EditBoxEndAction::RETURN);
    
    UIEditBox_close(editBoxEmcc->_id);
}

void EditBoxImplEmcc::onFocusCallback(void *userData)
{
    EditBoxImplEmcc *editBoxEmcc = static_cast<EditBoxImplEmcc*>(userData);
    editBoxEmcc->editBoxEditingDidEnd(editBoxEmcc->getNativeText(), EditBoxDelegate::EditBoxEndAction::UNKNOWN);
    
    UIEditBox_close(editBoxEmcc->_id);
}

    
void EditBoxImplEmcc::createFromJS()
{
    bool isMultiLine = _editBoxInputMode == EditBox::InputMode::ANY;
    _id = UIEditBox_init(isMultiLine);
    UIEditBox_setColor(_id, _colText.r, _colText.g, _colText.b);
    UIEditBox_setFont(_id, _fontName.c_str(), _fontSize);
    UIEditBox_setMaxLength(_id, _maxLength);
    UIEditBox_setCallback(_id, static_cast<void*>(this), &EditBoxImplEmcc::onEnterCallback, &EditBoxImplEmcc::onFocusCallback);
    
    switch (_editBoxInputFlag)
    {
        case EditBox::InputFlag::PASSWORD:
            UIEditBox_setInputType(_id, 1);
            break;
        case EditBox::InputFlag::INITIAL_CAPS_WORD:
            UIEditBox_setInputType(_id, 2);
            break;
        case EditBox::InputFlag::INITIAL_CAPS_SENTENCE:
            CCLOG("INITIAL_CAPS_SENTENCE not implemented");
            break;
        case EditBox::InputFlag::INITIAL_CAPS_ALL_CHARACTERS:
            UIEditBox_setInputType(_id, 3);
            break;
        case EditBox::InputFlag::SENSITIVE:
            CCLOG("SENSITIVE not implemented");
            break;
        default:
            break;
    }
    
    adjustTextFieldPositionAndSize();

    if (!_text.empty())
        UIEditBox_setText(_id, _text.c_str());
}

void EditBoxImplEmcc::adjustTextFieldPositionAndSize()
{
    auto glview = cocos2d::Director::getInstance()->getOpenGLView();
    float factor = cocos2d::Director::getInstance()->getContentScaleFactor();
    const Size &winSize = cocos2d::Director::getInstance()->getWinSize();
    
    Size contentSize = _editBox->getContentSize();
    Rect rect = Rect(0, 0, contentSize.width, contentSize.height);
    rect = RectApplyAffineTransform(rect, _editBox->getNodeToWorldAffineTransform());

    Vec2 designCoord = Vec2(rect.origin.x, rect.origin.y + rect.size.height);
    Vec2 visiblePos = Vec2(designCoord.x * glview->getScaleX(), designCoord.y * glview->getScaleY());
    Vec2 screenGLPos = visiblePos + glview->getViewPortRect().origin;

    rect.size = Size(rect.size.width * glview->getScaleX(), rect.size.height * glview->getScaleY());
    float newPositionY = (winSize.height * glview->getScaleX()) - screenGLPos.y;
    UIEditBox_setContentSize(_id, screenGLPos.x, newPositionY, rect.size.width, rect.size.height);
}
}

NS_CC_END

#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) */


 
