/*
 * Copyright (c) 2014 cocos2d-x.org
 * http://www.cocos2d-x.org
 *
 * cocos2d for iPhone: http://www.cocos2d-iphone.org
 *
 * Copyright (c) 2013 Apportable Inc.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

#include "CCButton.h"
#include "2d/CCLabel.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/CCAction.h"
#include "2d/CCActionInterval.h"

NS_CC_EXT_BEGIN

ButtonControl::ButtonControl():_background(Scale9Sprite::create()),_label(Label::create()),_zoomWhenHighlighted(false),_horizontalPadding(0),_verticalPadding(0),_togglesSelectedState(false),_preferredSize(0,0),_maxSize(0,0),_needLaout(true), _leftOffsets(0.0f), _topOffsets(0.0f), _rightOffsets(0.0f), _bottomOffsets(0)
{
    _ignoreAnchorPointForPosition = false;
    Control::addChild(_background);
    Control::addChild(_label);
    _label->setHorizontalAlignment(cocos2d::TextHAlignment::CENTER);
    _label->setVerticalAlignment(cocos2d::TextVAlignment::CENTER);
    Control::setCascadeColorEnabled(true);
    Control::setCascadeOpacityEnabled(true);
}
ButtonControl::~ButtonControl()
{
    
}

ButtonControl *ButtonControl::create()
{
    ButtonControl *ret = new ButtonControl();
    
    if (!ret)
        return nullptr;
    
    if(!ret->init())
    {
        delete ret;
        return nullptr;
    }
    
    ret->autorelease();
    
    return ret;
}
ButtonControl *ButtonControl::create(const std::string &title, const std::string &fontName, float size, SpriteFrame* normal, SpriteFrame* highlighted, SpriteFrame* disabled, SpriteFrame* selected)
{
    ButtonControl *ret = new ButtonControl();
    
    if (ret)
    {
        if(!ret->init())
        {
            delete ret;
            return nullptr;
        }
        ret->_title = title;
        ret->setFontName(fontName);
        ret->setFontSize(size);
        ret->_backgroundSpriteFrames.insert(static_cast<int>(Control::State::NORMAL), normal);
        ret->_backgroundSpriteFrames.insert(static_cast<int>(Control::State::HIGH_LIGHTED), highlighted);
        ret->_backgroundSpriteFrames.insert(static_cast<int>(Control::State::DISABLED), disabled);
        ret->_backgroundSpriteFrames.insert(static_cast<int>(Control::State::SELECTED), selected);
        ret->autorelease();
    }
    
    return ret;
}

bool ButtonControl::isTouchInside(Touch * touch)
{
    Vec2 touchLocation = touch->getLocation(); // Get the touch position
    touchLocation = this->getParent()->convertToNodeSpace(touchLocation);
    Rect rect = Rect(0, 0, _contentSize.width, _contentSize.height);
    rect.origin.x += _leftOffsets * getContentSize().width;
    rect.origin.y += _bottomOffsets * getContentSize().height;
    rect.size.width -= _rightOffsets * getContentSize().width - rect.origin.x;
    rect.size.height -= _topOffsets * getContentSize().height - rect.origin.y;
    Rect bBox = RectApplyAffineTransform(rect, getNodeToParentAffineTransform());
    return bBox.containsPoint(touchLocation);
}

bool ButtonControl::onTouchBegan(Touch *touch, Event *event)
{
    if (!isTouchInside(touch) || !isEnabled() || !isVisible() || !hasVisibleParents() )
    {
        return false;
    }
    this->setHighlighted(true);
    sendActionsForControlEvents(Control::EventType::TOUCH_DOWN);
    return true;
}
void ButtonControl::onTouchMoved(Touch *touch, Event *event)
{
    if (!isEnabled())
    {
        return;
    }
    
    bool isTouchMoveInside = isTouchInside(touch);
    if (isTouchMoveInside && !isHighlighted())
    {
        setHighlighted(true);
        sendActionsForControlEvents(Control::EventType::DRAG_ENTER);
    }
    else if (isTouchMoveInside && isHighlighted())
    {
        sendActionsForControlEvents(Control::EventType::DRAG_INSIDE);
    }
    else if (!isTouchMoveInside && isHighlighted())
    {
        setHighlighted(false);
        
        sendActionsForControlEvents(Control::EventType::DRAG_EXIT);
    }
    else if (!isTouchMoveInside && !isHighlighted())
    {
        sendActionsForControlEvents(Control::EventType::DRAG_OUTSIDE);
    }

}
void ButtonControl::onTouchEnded(Touch *touch, Event *event)
{
    setHighlighted(false);
    if (isTouchInside(touch))
    {
        if (_togglesSelectedState)
        {
            setSelected(!_selected);
        }
        sendActionsForControlEvents(Control::EventType::TOUCH_UP_INSIDE);
        if(_callBack)
        {
            retain();
            _callBack(this);
            release();
        }
    }
    else
    {
        sendActionsForControlEvents(Control::EventType::TOUCH_UP_OUTSIDE);
    }
}
void ButtonControl::onTouchCancelled(Touch *touch, Event *event)
{
    setHighlighted(false);
    sendActionsForControlEvents(Control::EventType::TOUCH_CANCEL);
}

void ButtonControl::setEnabled(bool enabled)
{
    Control::setEnabled(enabled);
    stateChanged();
}

void ButtonControl::setSelected(bool enabled)
{
    Control::setSelected(enabled);
    stateChanged();
}

void ButtonControl::setHighlighted(bool enabled)
{
    Control::setHighlighted(enabled);
    stateChanged();
}

void ButtonControl::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    if (!_visible)
        return;
    
    if(_needLaout)
        layout();
    
    Control::visit(renderer, parentTransform, parentFlags);
}

const Size& ButtonControl::getContentSize() const
{
    if (_needLaout)
    {
        const_cast<ButtonControl*>(this)->layout();
    }
    return Node::getContentSize();
}

void ButtonControl::setHorizontalAlignment(TextHAlignment hAlignment)
{
    _label->setHorizontalAlignment(hAlignment);
}

TextHAlignment ButtonControl::getHorizontalAlignment() const
{
    return _label->getHorizontalAlignment();
}

void ButtonControl::setVerticalAlignment(TextVAlignment vAlignment)
{
    _label->setVerticalAlignment(vAlignment);
}

TextVAlignment ButtonControl::getVerticalAlignment() const
{
    return _label->getVerticalAlignment();
}

void ButtonControl::updatePropertiesForState(Control::State state)
{
    // Update background
    _background->setColor(getBackgroundColorForState(state));
    _background->setOpacity(getBackgroundOpacityForState(state));
    
    SpriteFrame* spriteFrame = getBackgroundSpriteFrameForState(state);
    if (!spriteFrame)
    {
        if(state == Control::State::SELECTED_HIGH_LIGHTED)
        {
            spriteFrame = getBackgroundSpriteFrameForState(Control::State::HIGH_LIGHTED);
            if(spriteFrame)
                spriteFrame = getBackgroundSpriteFrameForState(Control::State::NORMAL);
        }
        else
        {
            spriteFrame = getBackgroundSpriteFrameForState(Control::State::NORMAL);
        }
    }
    if (spriteFrame)
    {
        Size size = spriteFrame->getOriginalSize();
        _background->setSpriteFrame(spriteFrame,Rect(_margin.origin.x*size.width,_margin.origin.y*size.height,_margin.size.width*size.width,_margin.size.height*size.height));
    }
    
    // Update label
    _label->setColor(getLabelColorForState(state));
    _label->setOpacity(getLabelOpacityForState(state));
    
    needsLayout();
}

void ButtonControl::stateChanged()
{
    if (isEnabled())
    {
        // Button is enabled
        if (isHighlighted())
        {
            if(isSelected())
                updatePropertiesForState(Control::State::SELECTED_HIGH_LIGHTED);
            else
                updatePropertiesForState(Control::State::HIGH_LIGHTED);
            
            if (_zoomWhenHighlighted)
            {
                _label->runAction(ScaleTo::create(0.1, 1.2));
                _background->runAction(ScaleTo::create(0.1, 1.2));
            }
        }
        else
        {
            if (isSelected())
            {
                updatePropertiesForState(Control::State::SELECTED);
            }
            else
            {
                updatePropertiesForState(Control::State::NORMAL);
            }
            
            _label->stopAllActions();
            _background->stopAllActions();
            if (_zoomWhenHighlighted)
            {
                _label->setScaleX(1.0f);
                _label->setScaleY(1.0f);
                
                _background->setScaleX(1.0f);
                _background->setScaleY(1.0f);
            }
        }
    }
    else
    {
        // Button is disabled
        updatePropertiesForState(Control::State::DISABLED);
    }
}

void ButtonControl::needsLayout()
{
    _needLaout = true;
    Control::needsLayout();
}

void ButtonControl::layout()
{
    _label->setDimensions(0,0);
    Size originalLabelSize = _label->getContentSize();
    Size paddedLabelSize = originalLabelSize;
    paddedLabelSize.width += _horizontalPadding * 2;
    paddedLabelSize.height += _verticalPadding * 2;
    
    bool shrunkSize = false;
    Size size = _preferredSize;
    
    Size maxSize = _maxSize;
    
    if (size.width < paddedLabelSize.width)
        size.width = paddedLabelSize.width;
    if (size.height < paddedLabelSize.height)
        size.height = paddedLabelSize.height;
    
    if (maxSize.width > 0 && maxSize.width < size.width)
    {
        size.width = maxSize.width;
        shrunkSize = true;
    }
    if (maxSize.height > 0 && maxSize.height < size.height)
    {
        size.height = maxSize.height;
        shrunkSize = true;
    }
    
    Size labelSize = Size(clampf(size.width - _horizontalPadding * 2, 0, size.width - _horizontalPadding * 2),
                                      clampf(size.height - _verticalPadding * 2, 0, size.height - _verticalPadding * 2));
    _label->setDimensions(labelSize.width, labelSize.height);
    
    _background->setContentSize(size);
    _background->setAnchorPoint(Point(0.5f,0.5f));
    _background->setPosition(Point(size.width/2,size.height/2));
    
    _label->setPosition(Point(size.width/2,size.height/2));
    
    setContentSize(size);
    
    _needLaout = false;
}

void ButtonControl::setPreferredSize(const Size& value)
{
    _preferredSize = value;
    needsLayout();
}

const Size& ButtonControl::getPreferredSize() const
{
    return _preferredSize;
}

void ButtonControl::setMaxSize(const Size& value)
{
    _maxSize = value;
    needsLayout();
}

const Size& ButtonControl::getMaxSize() const
{
    return _maxSize;
}

Scale9Sprite* ButtonControl::getBackground() const
{
    return _background;
}
Label* ButtonControl::getLabel() const
{
    return _label;
}

void ButtonControl::setZoomWhenHighlighted(bool value)
{
    _zoomWhenHighlighted = value;
}
const bool ButtonControl::getZoomWhenHighlighted() const
{
    return _zoomWhenHighlighted;
}

void ButtonControl::setHorizontalPadding(float value)
{
    _horizontalPadding = value;
    needsLayout();
}
const float ButtonControl::getHorizontalPadding() const
{
    return _horizontalPadding;
}

void ButtonControl::setVerticalPadding(float value)
{
    _verticalPadding = value;
    needsLayout();
}

const float ButtonControl::getVerticalPadding() const
{
    return _verticalPadding;
}

void ButtonControl::setTogglesSelectedState(bool value)
{
    _togglesSelectedState = true;
}

const bool ButtonControl::getTogglesSelectedState() const
{
    return _togglesSelectedState;
}

void ButtonControl::setTitle(const std::string& value)
{
    _title = value;
    _label->setString(value);
    needsLayout();
}
const std::string& ButtonControl::getTitle() const
{
    return _title;
}

void ButtonControl::setFontName(const std::string& value)
{
    _label->setSystemFontName(value);
    needsLayout();
}

const std::string& ButtonControl::getFontName() const
{
    return _label->getSystemFontName();
}

void ButtonControl::setFontSize(float value)
{
    _label->setSystemFontSize(value);
    needsLayout();
}
const float ButtonControl::getFontSize() const
{
    return _label->getSystemFontSize();
}

void ButtonControl::setCapInsets(const Rect& value)
{
    _margin = value;
    stateChanged();
}

const Rect& ButtonControl::getCapInsets() const
{
    return _margin;
}

void ButtonControl::setBackgroundColor(const Color3B& color, Control::State state)
{
    _backgroundColors[state] = color;
    stateChanged();
}

const Color3B& ButtonControl::getBackgroundColorForState(Control::State state) const
{
    auto it = _backgroundColors.find(state);
    if(it != _backgroundColors.end())
        return it->second;
    else
        return Color3B::WHITE;
}

void ButtonControl::setBackgroundOpacity(GLubyte opacity, Control::State state)
{
    _backgroundOpacities[state] = opacity;
    stateChanged();
}

GLubyte ButtonControl::getBackgroundOpacityForState(Control::State state) const
{
    auto it = _backgroundOpacities.find(state);
    if(it != _backgroundOpacities.end())
        return it->second;
    else
        return 255;
}


void ButtonControl::setLabelColor(const Color3B& color, Control::State state)
{
    _labelColors[state] = color;
    stateChanged();
}


const Color3B& ButtonControl::getLabelColorForState(Control::State state) const
{
    auto it = _labelColors.find(state);
    if(it != _labelColors.end())
        return it->second;
    else
        return Color3B::WHITE;
}


void ButtonControl::setLabelOpacity(GLubyte opacity, Control::State state)
{
    _labelOpacities[state] = opacity;
    stateChanged();
}


GLubyte ButtonControl::getLabelOpacityForState(Control::State state) const
{
    auto it = _labelOpacities.find(state);
    if(it != _labelOpacities.end())
        return it->second;
    else
        return 255;
}


void ButtonControl::setBackgroundSpriteFrame(SpriteFrame* spriteFrame, Control::State state)
{
    auto it=_backgroundSpriteFrames.find(static_cast<int>(state));
    if(it!=_backgroundSpriteFrames.end())
        _backgroundSpriteFrames.erase(it);
    _backgroundSpriteFrames.insert(static_cast<int>(state), spriteFrame);
    stateChanged();
}


SpriteFrame* ButtonControl::getBackgroundSpriteFrameForState(Control::State state) const
{
    auto it=_backgroundSpriteFrames.find(static_cast<int>(state));
    if(it!=_backgroundSpriteFrames.end())
        return it->second;
    else
        return nullptr;
}

void ButtonControl::setCallback(const ccButtonCallback& callback)
{
    _callBack = callback;
}

void ButtonControl::addChild(Node * child, int zOrder/* =0 */, int tag/* =0 */)
{
    CCASSERT(0, "addChild: is not supported on Label.");
}

void ButtonControl::setCascadeOpacityEnabled(bool cascadeOpacityEnabled)
{
    
}

void ButtonControl::setCascadeColorEnabled(bool cascadeColorEnabled)
{
    
}

void ButtonControl::setOffsets(float left, float top, float right, float bottom)
{
    _leftOffsets = left;
    _topOffsets = top;
    _rightOffsets = right;
    _bottomOffsets = bottom;
}

void ButtonControl::setLeftOffset(float left)
{
    _leftOffsets = left;
}

float ButtonControl::getLeftOffset() const
{
    return _leftOffsets;
}

void ButtonControl::setTopOffset(float top)
{
    _topOffsets = top;
}

float ButtonControl::getTopOffset() const
{
    return _topOffsets;
}

void ButtonControl::setRightOffset(float right)
{
    _rightOffsets = right;
}

float ButtonControl::getRightOffset() const
{
    return _rightOffsets;
}

void ButtonControl::setBottomOffset(float bottom)
{
    _bottomOffsets = bottom;
}

float ButtonControl::getBottomOffset() const
{
    return _bottomOffsets;
}

NS_CC_EXT_END
