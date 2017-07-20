#ifndef __UIEditBoxIMPLEMCC_H__
#define __UIEditBoxIMPLEMCC_H__

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)

#include "UIEditBoxImpl-common.h"


NS_CC_BEGIN

class EventListenerTouchOneByOne;
namespace ui {

class EditBox;

class EditBoxImplEmcc : public EditBoxImplCommon
{
public:
    /**
     * @js NA
     */
    EditBoxImplEmcc(EditBox* pEditText);
    /**
     * @js NA
     * @lua NA
     */
    virtual ~EditBoxImplEmcc();
    
    virtual void setFont(const char* pFontName, int fontSize) override;
    virtual void setPlaceholderFont(const char* pFontName, int fontSize) override;
    
    virtual bool isEditing() override;
    virtual void createNativeControl(const Rect& frame) override;
    virtual void setNativeFont(const char* pFontName, int fontSize) override {};
    virtual void setNativeFontColor(const Color4B& color) override;
    virtual void setNativePlaceholderFont(const char* pFontName, int fontSize) override;
    virtual void setNativePlaceholderFontColor(const Color4B& color) override;
    virtual void setNativeInputMode(EditBox::InputMode inputMode) override;
    virtual void setNativeInputFlag(EditBox::InputFlag inputFlag) override;
    virtual void setNativeReturnType(EditBox::KeyboardReturnType returnType) override;
    virtual void setNativeTextHorizontalAlignment(cocos2d::TextHAlignment alignment) override;
    virtual void setNativeText(const char* pText) override;
    virtual void setNativePlaceHolder(const char* pText) override;
    virtual void setNativeVisible(bool visible) override;
    virtual void updateNativeFrame(const Rect& rect) override;
    virtual const char* getNativeDefaultFontName() override;
    virtual void nativeOpenKeyboard() override;
    virtual void nativeCloseKeyboard() override;
    virtual void setNativeMaxLength(int maxLength) override;
    
    virtual void setContentSize(const Size& size) override;
    virtual void setPosition(const Vec2& pos) override;
    virtual void setAnchorPoint(const Vec2& anchorPoint) override;
    
    virtual void updatePosition(float dt) override;
    virtual void onEnter(void) override;

protected:
    void adjustTextFieldPositionAndSize();
    void createFromJS();
    
    const char *getNativeText();
    
    static void onEnterCallback(void *userData);
    static void onFocusCallback(void *userData);
    
private:
    Vec2 _position;
    Vec2 _anchorPoint;
    Size _contentSize;
    
    EventListener* _beforeDraw;
    EventListener* _afterDraw;
    EventListenerTouchOneByOne* _touchListener;
    
    int _id = 0;
    bool _drawedLastFrame = true;
    bool _drawedThisFrame = true;
};


}

NS_CC_END


#endif /* #if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN) */

#endif /* __UIEditBoxIMPLEMCC_H__ */

