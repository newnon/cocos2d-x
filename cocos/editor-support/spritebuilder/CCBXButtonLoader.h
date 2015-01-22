#ifndef _CCBX_CCBUTTONLOADER_H_
#define _CCBX_CCBUTTONLOADER_H_

#include "CCBXWidgetLoader.h"

NS_CC_BEGIN
namespace spritebuilder {

class ButtonLoader : public WidgetLoader {
public:
    
    static ButtonLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode) override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale) override;
    
CC_CONSTRUCTOR_ACCESS:
    ButtonLoader();
    ~ButtonLoader();

protected:
    virtual void onHandlePropTypeText(const std::string &propertyName, bool isExtraProp, const std::string &value) override;
    virtual void onHandlePropTypeFontTTF(const std::string &propertyName, bool isExtraProp, const std::string &value) override;
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value) override;
    virtual void onHandlePropTypeSize(const std::string &propertyName, bool isExtraProp, const SizeDescription &value) override;
    virtual void onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value) override;
    virtual void onHandlePropTypeColor3(const std::string &propertyName, bool isExtraProp, const Color3B &value) override;
    virtual void onHandlePropTypeColor4(const std::string &propertyName, bool isExtraProp, const Color4B &value) override;
    virtual void onHandlePropTypePosition(const std::string &propertyName, bool isExtraProp, const PositionDescription &value) override;
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;

private:
    cocos2d::Rect _margins;
    cocos2d::Rect _offset;
    cocos2d::Color4B _outlineColor;
    cocos2d::Color4B _shadowColor;
    float _outlineWidth;
    float _shadowBlurRadius;
    cocos2d::Size _shadowOffset;
    float _zoomOnClick;
    
    Color3B _normalLabelColor;
    Color3B _normalBackgroundColor;
    GLubyte _normalLabelOpacity;
    GLubyte _normalBackgroundOpacity;
    
    Color3B _selectedLabelColor;
    Color3B _selectedBackgroundColor;
    GLubyte _selectedLabelOpacity;
    GLubyte _selectedBackgroundOpacity;
    
    Color3B _disabledLabelColor;
    Color3B _disabledBackgroundColor;
    GLubyte _disabledLabelOpacity;
    GLubyte _disabledBackgroundOpacity;
    
    std::string _label;
    std::string _font;
    FloatScaleDescription _fontSize;
    FloatScaleDescription _horizPadding;
    FloatScaleDescription _vertPadding;
    TextHAlignment _textHAlignment;
    TextVAlignment _textVAlignment;
    
    SpriteFrameDescription _normalSpriteFrame;
    SpriteFrameDescription _selectedSpriteFrame;
    SpriteFrameDescription _disabledSpriteFrame;
    
};

}

NS_CC_END

#endif
