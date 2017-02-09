/****************************************************************************
Copyright (c) 2013-2014 Chukong Technologies Inc.

http://www.cocos2d-x.org

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
****************************************************************************/

#include "ui/UIScrollBar.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIHelper.h"
#include "2d/CCSprite.h"
#include "2d/CCCamera.h"
#include "editor-support/cocostudio/CocosStudioExtension.h"

NS_CC_BEGIN

namespace ui {
    
static const int BASEBAR_RENDERER_Z = (-2);
static const int PROGRESSBAR_RENDERER_Z = (-2);
static const int SLIDBALL_RENDERER_Z = (-1);
    
IMPLEMENT_CLASS_GUI_INFO(ScrollBar)
    
ScrollBar::ScrollBar():
_barRenderer(nullptr),
_progressBarRenderer(nullptr),
_barTextureSize(Size::ZERO),
_progressBarTextureSize(Size::ZERO),
_slidBallNormalRenderer(nullptr),
_slidBallPressedRenderer(nullptr),
_slidBallDisabledRenderer(nullptr),
_slidBallMouseOverRenderer(nullptr),
_slidBallRenderer(nullptr),
_barLength(0.0),
_percent(0),
_maxPercent(100),
_scale9Enabled(false),
_prevIgnoreSize(true),
_zoomScale(0.1f),
_sliderBallNormalTextureScaleX(1.0),
_sliderBallNormalTextureScaleY(1.0),
_isSliderBallPressedTextureLoaded(false),
_isSliderBallDisabledTexturedLoaded(false),
_isSliderBallMouseOverTexturedLoaded(false),
_capInsetsBarRenderer(Rect::ZERO),
_capInsetsProgressBarRenderer(Rect::ZERO),
_sliderEventListener(nullptr),
_sliderEventSelector(nullptr),
_eventCallback(nullptr),
_barTexType(TextureResType::LOCAL),
_progressBarTexType(TextureResType::LOCAL),
_ballNTexType(TextureResType::LOCAL),
_ballPTexType(TextureResType::LOCAL),
_ballDTexType(TextureResType::LOCAL),
_ballMTexType(TextureResType::LOCAL),
_barRendererAdaptDirty(true),
_progressBarRendererDirty(true),
_textureFile(""),
_progressBarTextureFile(""),
_slidBallNormalTextureFile(""),
_slidBallPressedTextureFile(""),
_slidBallDisabledTextureFile(""),
_slidBallMouseOverTextureFile(""),
_imageScale(1.0f)
{
    setTouchEnabled(true);
}

ScrollBar::~ScrollBar()
{
    _sliderEventListener = nullptr;
    _sliderEventSelector = nullptr;
}

ScrollBar* ScrollBar::create()
{
    ScrollBar* widget = new (std::nothrow) ScrollBar();
    if (widget && widget->init())
    {
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}
    
ScrollBar* ScrollBar::create(const std::string& barTextureName,
                      const std::string& normalBallTextureName,
                      TextureResType resType)
{
    ScrollBar* widget = new (std::nothrow) ScrollBar();
    if (widget && widget->init())
    {
        widget->loadBarTexture(barTextureName, resType);
        widget->loadSlidBallTextureNormal(normalBallTextureName, resType);
        widget->autorelease();
        return widget;
    }
    CC_SAFE_DELETE(widget);
    return nullptr;
}

bool ScrollBar::init()
{
    if (Widget::init())
    {
        return true;
    }
    return false;
}

void ScrollBar::initRenderer()
{
    _barRenderer = Scale9Sprite::create();
    _progressBarRenderer = Scale9Sprite::create();
    _barRenderer->setScale9Enabled(false);
    _progressBarRenderer->setScale9Enabled(false);
    
    _progressBarRenderer->setAnchorPoint(Vec2(0.0f, 0.5f));
    
    addProtectedChild(_barRenderer, BASEBAR_RENDERER_Z, -1);
    addProtectedChild(_progressBarRenderer, PROGRESSBAR_RENDERER_Z, -1);
    
    _slidBallNormalRenderer = Scale9Sprite::create();
    _slidBallPressedRenderer = Scale9Sprite::create();
    _slidBallPressedRenderer->setVisible(false);
    _slidBallDisabledRenderer = Scale9Sprite::create();
    _slidBallDisabledRenderer->setVisible(false);
    _slidBallMouseOverRenderer = Scale9Sprite::create();
    _slidBallMouseOverRenderer->setVisible(false);
    
    _slidBallRenderer = Node::create();
    
    _slidBallRenderer->addChild(_slidBallNormalRenderer);
    _slidBallRenderer->addChild(_slidBallPressedRenderer);
    _slidBallRenderer->addChild(_slidBallDisabledRenderer);
    _slidBallRenderer->addChild(_slidBallMouseOverRenderer);
    _slidBallRenderer->setCascadeColorEnabled(true);
    _slidBallRenderer->setCascadeOpacityEnabled(true);
    
    addProtectedChild(_slidBallRenderer, SLIDBALL_RENDERER_Z, -1);
}

void ScrollBar::loadBarTexture(const std::string& fileName, TextureResType texType)
{
    _textureFile = fileName;
    _barTexType = texType;
    if (fileName.empty())
    {
        _barRenderer->resetRender();
    }
    else
    {
        switch (_barTexType)
        {
        case TextureResType::LOCAL:
            _barRenderer->initWithFile(fileName);
            break;
        case TextureResType::PLIST:
            _barRenderer->initWithSpriteFrameName(fileName);
            break;
        default:
            break;
        }
    }
    //FIXME: https://github.com/cocos2d/cocos2d-x/issues/12249
    if (!_ignoreSize && _customSize.equals(Size::ZERO)) {
        _customSize = _barRenderer->getContentSize();
    }
    this->setupBarTexture();
}
void ScrollBar::loadBarTexture(SpriteFrame* spriteframe)
{
    _barRenderer->initWithSpriteFrame(spriteframe);
    this->setupBarTexture();
}

void ScrollBar::setupBarTexture()
{
    this->updateChildrenDisplayedRGBA();
    _barRendererAdaptDirty = true;
    _progressBarRendererDirty = true;
    updateContentSizeWithTextureSize(_barRenderer->getContentSize());
    _barTextureSize = _barRenderer->getContentSize();
}

void ScrollBar::loadProgressBarTexture(const std::string& fileName, TextureResType texType)
{
    _progressBarTextureFile = fileName;
    _progressBarTexType = texType;
    if (fileName.empty())
    {
        _progressBarRenderer->resetRender();
    }
    else
    {
        switch (_progressBarTexType)
        {
        case TextureResType::LOCAL:
            _progressBarRenderer->initWithFile(fileName);
            break;
        case TextureResType::PLIST:
            _progressBarRenderer->initWithSpriteFrameName(fileName);
            break;
        default:
            break;
        }
    }
    this->setupProgressBarTexture();
}

void ScrollBar::loadProgressBarTexture(SpriteFrame* spriteframe)
{
    _progressBarRenderer->initWithSpriteFrame(spriteframe);
    this->setupProgressBarTexture();
}

void ScrollBar::setupProgressBarTexture()
{
    this->updateChildrenDisplayedRGBA();
    _progressBarRenderer->setAnchorPoint(Vec2(0.0f, 0.5f));
    _progressBarTextureSize = _progressBarRenderer->getContentSize();
    _progressBarRendererDirty = true;
}

void ScrollBar::setScale9Enabled(bool able)
{
    if (_scale9Enabled == able)
    {
        return;
    }
    
    _scale9Enabled = able;
    _barRenderer->setScale9Enabled(_scale9Enabled);
    _progressBarRenderer->setScale9Enabled(_scale9Enabled);
    
    if (_scale9Enabled)
    {
        bool ignoreBefore = _ignoreSize;
        ignoreContentAdaptWithSize(false);
        _prevIgnoreSize = ignoreBefore;
    }
    else
    {
        ignoreContentAdaptWithSize(_prevIgnoreSize);
    }
    setCapInsetsBarRenderer(_capInsetsBarRenderer);
    setCapInsetProgressBarRenderer(_capInsetsProgressBarRenderer);
    _barRendererAdaptDirty = true;
    _progressBarRendererDirty = true;
}
    
bool ScrollBar::isScale9Enabled()const
{
    return _scale9Enabled;
}

void ScrollBar::ignoreContentAdaptWithSize(bool ignore)
{
    if (!_scale9Enabled || (_scale9Enabled && !ignore))
    {
        Widget::ignoreContentAdaptWithSize(ignore);
        _prevIgnoreSize = ignore;
    }
}

void ScrollBar::setCapInsets(const Rect &capInsets)
{
    setCapInsetsBarRenderer(capInsets);
    setCapInsetProgressBarRenderer(capInsets);
}

void ScrollBar::setCapInsetsBarRenderer(const Rect &capInsets)
{
    _capInsetsBarRenderer = ui::Helper::restrictCapInsetRect(capInsets, _barRenderer->getContentSize());
    if (!_scale9Enabled)
    {
        return;
    }
    _barRenderer->setCapInsets(_capInsetsBarRenderer);
}
    
const Rect& ScrollBar::getCapInsetsBarRenderer()const
{
    return _capInsetsBarRenderer;
}

void ScrollBar::setCapInsetProgressBarRenderer(const Rect &capInsets)
{
    _capInsetsProgressBarRenderer = ui::Helper::restrictCapInsetRect(capInsets, _progressBarRenderer->getContentSize());

    if (!_scale9Enabled)
    {
        return;
    }
    _progressBarRenderer->setCapInsets(_capInsetsProgressBarRenderer);
}
    
const Rect& ScrollBar::getCapInsetsProgressBarRenderer()const
{
    return _capInsetsProgressBarRenderer;
}

void ScrollBar::loadSlidBallTextures(const std::string& normal,
                                      const std::string& pressed,
                                      const std::string& disabled,
                                      const std::string& mouseOver,
                                      TextureResType texType)
{
    loadSlidBallTextureNormal(normal, texType);
    loadSlidBallTexturePressed(pressed,texType);
    loadSlidBallTextureDisabled(disabled,texType);
    loadSlidBallTextureMouseOver(mouseOver,texType);
}

void ScrollBar::loadSlidBallTextureNormal(const std::string& normal,TextureResType texType)
{
    _slidBallNormalTextureFile = normal;
    _ballNTexType = texType;
    if (normal.empty())
    {
        _slidBallNormalRenderer->resetRender();
    }
    else
    {
        switch (_ballNTexType)
        {
        case TextureResType::LOCAL:
            _slidBallNormalRenderer->initWithFile(normal);
            break;
        case TextureResType::PLIST:
            _slidBallNormalRenderer->initWithSpriteFrameName(normal);
            break;
        default:
            break;
        }
    }
    
    this->updateChildrenDisplayedRGBA();
}
void ScrollBar::loadSlidBallTextureNormal(SpriteFrame* spriteframe)
{
    _slidBallNormalRenderer->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}

void ScrollBar::loadSlidBallTexturePressed(const std::string& pressed,TextureResType texType)
{
    _slidBallPressedTextureFile = pressed;
    _isSliderBallPressedTextureLoaded = !pressed.empty();
    _ballPTexType = texType;
    if (pressed.empty())
    {
        _slidBallPressedRenderer->resetRender();
    }
    else
    {
        switch (_ballPTexType)
        {
        case TextureResType::LOCAL:
            _slidBallPressedRenderer->initWithFile(pressed);
            break;
        case TextureResType::PLIST:
            _slidBallPressedRenderer->initWithSpriteFrameName(pressed);
            break;
        default:
            break;
        }
    }
    this->updateChildrenDisplayedRGBA();
}


void ScrollBar::loadSlidBallTexturePressed(SpriteFrame* spriteframe)
{
    _slidBallPressedRenderer->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}

void ScrollBar::loadSlidBallTextureDisabled(const std::string& disabled,TextureResType texType)
{
    _slidBallDisabledTextureFile = disabled;
    _isSliderBallDisabledTexturedLoaded = !disabled.empty();
    _ballDTexType = texType;
    if (disabled.empty())
    {
        _slidBallDisabledRenderer->init();
    }
    else
    {
        switch (_ballDTexType)
        {
        case TextureResType::LOCAL:
            _slidBallDisabledRenderer->initWithFile(disabled);
            break;
        case TextureResType::PLIST:
            _slidBallDisabledRenderer->initWithSpriteFrameName(disabled);
            break;
        default:
            break;
        }
    }
    this->updateChildrenDisplayedRGBA();
}

void ScrollBar::loadSlidBallTextureDisabled(SpriteFrame* spriteframe)
{
    _slidBallDisabledRenderer->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}
    
void ScrollBar::loadSlidBallTextureMouseOver(const std::string& mouseOver,TextureResType texType)
{
    _slidBallMouseOverTextureFile = mouseOver;
    _isSliderBallMouseOverTexturedLoaded = !mouseOver.empty();
    _ballMTexType = texType;
    if (mouseOver.empty())
    {
        _slidBallMouseOverRenderer->resetRender();
    }
    else
    {
        switch (_ballMTexType)
        {
            case TextureResType::LOCAL:
                _slidBallMouseOverRenderer->initWithFile(mouseOver);
                break;
            case TextureResType::PLIST:
                _slidBallMouseOverRenderer->initWithSpriteFrameName(mouseOver);
                break;
            default:
                break;
        }
    }
    this->updateChildrenDisplayedRGBA();
}

void ScrollBar::loadSlidBallTextureMouseOver(SpriteFrame* spriteframe)
{
    _slidBallMouseOverRenderer->setSpriteFrame(spriteframe);
    this->updateChildrenDisplayedRGBA();
}

void ScrollBar::setMaxPercent(int percent)
{
    _maxPercent = percent;
}

int ScrollBar::getMaxPercent()const
{
    return _maxPercent;
}

void ScrollBar::setPercent(int percent)
{
    if (percent > _maxPercent)
    {
        percent = _maxPercent;
    }
    if (percent < 0)
    {
        percent = 0;
    }
    _percent = percent;
    float res = 1.0 * percent / _maxPercent;
    float dis = _barLength * res;
    _slidBallRenderer->setPosition(dis, _contentSize.height / 2.0f);
    if (_scale9Enabled)
    {
        _progressBarRenderer->setPreferredSize(Size(dis / _imageScale, _contentSize.height));
    }
    else
    {
        Sprite* spriteRenderer = _progressBarRenderer->getSprite();
        
        if (nullptr != spriteRenderer) {
            Rect rect = spriteRenderer->getTextureRect();
            rect.size.width = _progressBarTextureSize.width * res;
            spriteRenderer->setTextureRect(rect, spriteRenderer->isTextureRectRotated(), rect.size);
        }
    }
}
    
void ScrollBar::setImageScale(float scale)
{
    if(_imageScale != scale)
    {
        _imageScale = scale;
        _barRendererAdaptDirty = true;
        _progressBarRendererDirty = true;
        _slidBallRenderer->setScale(scale);
    }
}

float ScrollBar::getImageScale() const
{
    return _imageScale;
}
    
bool ScrollBar::hitTest(const cocos2d::Vec2 &pt, const Camera *camera, Vec3 *p) const
{
    Rect rect;
    rect.size = _slidBallNormalRenderer->getContentSize();
    auto w2l = _slidBallNormalRenderer->getWorldToNodeTransform();

    Rect sliderBarRect;
    sliderBarRect.size = this->_barRenderer->getContentSize();
    auto barW2l = this->_barRenderer->getWorldToNodeTransform();
    return isScreenPointInRect(pt, camera, w2l, rect, nullptr) || isScreenPointInRect(pt, camera, barW2l, sliderBarRect, nullptr);
}

bool ScrollBar::onTouchBegan(Touch *touch, Event *unusedEvent)
{
    bool pass = Widget::onTouchBegan(touch, unusedEvent);
    if (_hitted)
    {
        setPercent(getPercentWithBallPos(_touchBeganPosition));
        percentChangedEvent(EventType::ON_SLIDEBALL_DOWN);
    }
    return pass;
}

void ScrollBar::onTouchMoved(Touch *touch, Event *unusedEvent)
{
    _touchMovePosition = touch->getLocation();
    setPercent(getPercentWithBallPos(_touchMovePosition));
    percentChangedEvent(EventType::ON_PERCENTAGE_CHANGED);
}

void ScrollBar::onTouchEnded(Touch *touch, Event *unusedEvent)
{
    Widget::onTouchEnded(touch, unusedEvent);
    percentChangedEvent(EventType::ON_PERCENTAGE_CHANGED);
    percentChangedEvent(EventType::ON_SLIDEBALL_UP);
}

void ScrollBar::onTouchCancelled(Touch *touch, Event *unusedEvent)
{
    Widget::onTouchCancelled(touch, unusedEvent);
    percentChangedEvent(EventType::ON_SLIDEBALL_CANCEL);
}

float ScrollBar::getPercentWithBallPos(const Vec2 &pt) const
{
    Vec3 p;
    Widget::hitTest(pt, _hittedByCamera, &p);
    return ((p.x/_barLength) * static_cast<float>(_maxPercent));
}

void ScrollBar::addEventListenerSlider(Ref *target, SEL_ScrollBarPercentChangedEvent selector)
{
    _sliderEventListener = target;
    _sliderEventSelector = selector;
}
    
void ScrollBar::addEventListener(const ccSliderCallback& callback)
{
    _eventCallback = callback;
}

void ScrollBar::percentChangedEvent(EventType event)
{
    this->retain();
    if (_sliderEventListener && _sliderEventSelector)
    {
        (_sliderEventListener->*_sliderEventSelector)(this,SCOLBAR_PERCENTCHANGED);
    }
    if (_eventCallback)
    {
        _eventCallback(this,event);
    }
    if (_ccEventCallback)
    {
        _ccEventCallback(this, static_cast<int>(EventType::ON_PERCENTAGE_CHANGED));
    }
    this->release();
}

int ScrollBar::getPercent()const
{
    return _percent;
}

void ScrollBar::onSizeChanged()
{
    Widget::onSizeChanged();
    _barRendererAdaptDirty = true;
    _progressBarRendererDirty = true;
}
    
void ScrollBar::adaptRenderers()
{
    if (_barRendererAdaptDirty)
    {
        barRendererScaleChangedWithSize();
        _barRendererAdaptDirty = false;
    }
    if (_progressBarRendererDirty)
    {
        progressBarRendererScaleChangedWithSize();
        _progressBarRendererDirty = false;
    }
}

Size ScrollBar::getVirtualRendererSize() const
{
    return _barRenderer->getContentSize();
}

Node* ScrollBar::getVirtualRenderer()
{
    return _barRenderer;
}

void ScrollBar::barRendererScaleChangedWithSize()
{
    if (_unifySize)
    {
        _barLength = _contentSize.width;
        _barRenderer->setPreferredSize(_contentSize);
    }
    else if (_ignoreSize)
    {
        
        _barRenderer->setScale(1.0f);
        _barLength = _contentSize.width;
    }
    else
    {
        _barLength = _contentSize.width;
        if (_scale9Enabled)
        {
            _barRenderer->setPreferredSize(_contentSize / _imageScale);
            _barRenderer->setScale(_imageScale);
        }
        else
        {
            Size btextureSize = _barTextureSize;
            if (btextureSize.width <= 0.0f || btextureSize.height <= 0.0f)
            {
                _barRenderer->setScale(1.0f);
            }
            else
            {
                float bscaleX = _contentSize.width / btextureSize.width;
                float bscaleY = _contentSize.height / btextureSize.height;
                _barRenderer->setScaleX(bscaleX);
                _barRenderer->setScaleY(bscaleY);
            }
        }
    }
    _barRenderer->setPosition(_contentSize.width / 2.0f, _contentSize.height / 2.0f);
    setPercent(_percent);
}

void ScrollBar::progressBarRendererScaleChangedWithSize()
{
    if (_unifySize)
    {
        _progressBarRenderer->setPreferredSize(_contentSize);
    }
    else if (_ignoreSize)
    {
        if (!_scale9Enabled)
        {
            Size ptextureSize = _progressBarTextureSize;
            float pscaleX = _contentSize.width / ptextureSize.width;
            float pscaleY = _contentSize.height / ptextureSize.height;
            _progressBarRenderer->setScaleX(pscaleX);
            _progressBarRenderer->setScaleY(pscaleY);
        }
    }
    else
    {
        if (_scale9Enabled)
        {
            _progressBarRenderer->setPreferredSize(_contentSize / _imageScale);
            _progressBarRenderer->setScale(_imageScale);
        }
        else
        {
            Size ptextureSize = _progressBarTextureSize;
            if (ptextureSize.width <= 0.0f || ptextureSize.height <= 0.0f)
            {
                _progressBarRenderer->setScale(1.0f);
                return;
            }
            float pscaleX = _contentSize.width / ptextureSize.width;
            float pscaleY = _contentSize.height / ptextureSize.height;
            _progressBarRenderer->setScaleX(pscaleX);
            _progressBarRenderer->setScaleY(pscaleY);
        }
    }
    _progressBarRenderer->setPosition(0.0f, _contentSize.height / 2.0f);
    setPercent(_percent);
}

void ScrollBar::onPressStateChangedToNormal()
{
    _slidBallNormalRenderer->setVisible(true);
    _slidBallPressedRenderer->setVisible(false);
    _slidBallDisabledRenderer->setVisible(false);
    _slidBallMouseOverRenderer->setVisible(false);
    
    _slidBallNormalRenderer->setGLProgramState(this->getNormalGLProgramState(_slidBallNormalRenderer->getSprite()->getTexture()));
    _slidBallNormalRenderer->setScale(_sliderBallNormalTextureScaleX, _sliderBallNormalTextureScaleY);
}

void ScrollBar::onPressStateChangedToPressed()
{
    _slidBallNormalRenderer->setGLProgramState(this->getNormalGLProgramState(_slidBallNormalRenderer->getSprite()->getTexture()));

    
    if (!_isSliderBallPressedTextureLoaded)
    {
        _slidBallNormalRenderer->setScale(_sliderBallNormalTextureScaleX + _zoomScale,
                                          _sliderBallNormalTextureScaleY + _zoomScale);
    }
    else
    {
        _slidBallNormalRenderer->setVisible(false);
        _slidBallPressedRenderer->setVisible(true);
        _slidBallDisabledRenderer->setVisible(false);
    }
    _slidBallMouseOverRenderer->setVisible(false);
}

void ScrollBar::onPressStateChangedToDisabled()
{
    if (!_isSliderBallDisabledTexturedLoaded)
    {
        _slidBallNormalRenderer->setGLProgramState(this->getGrayGLProgramState(_slidBallNormalRenderer->getSprite()->getTexture()));
        _slidBallNormalRenderer->setVisible(true);
    }
    else
    {
        _slidBallNormalRenderer->setVisible(false);
        _slidBallDisabledRenderer->setVisible(true);
    }
    
    _slidBallNormalRenderer->setScale(_sliderBallNormalTextureScaleX, _sliderBallNormalTextureScaleY);
    
    _slidBallPressedRenderer->setVisible(false);
    _slidBallMouseOverRenderer->setVisible(false);
}
    
void ScrollBar::onPressStateChangedToMouseOver()
{
    if (!_isSliderBallMouseOverTexturedLoaded)
    {
        _slidBallNormalRenderer->setVisible(true);
    }
    else
    {
        _slidBallNormalRenderer->setVisible(false);
        _slidBallMouseOverRenderer->setVisible(true);
    }
    
    _slidBallNormalRenderer->setScale(_sliderBallNormalTextureScaleX, _sliderBallNormalTextureScaleY);
    
    _slidBallPressedRenderer->setVisible(false);
    _slidBallDisabledRenderer->setVisible(false);
}
    
    
void ScrollBar::setZoomScale(float scale)
{
    _zoomScale = scale;
}

float ScrollBar::getZoomScale()const
{
    return _zoomScale;
}


std::string ScrollBar::getDescription() const
{
    return "Slider";
}

Widget* ScrollBar::createCloneInstance()
{
    return ScrollBar::create();
}

void ScrollBar::copySpecialProperties(Widget *widget)
{
    ScrollBar* slider = dynamic_cast<ScrollBar*>(widget);
    if (slider)
    {
        _prevIgnoreSize = slider->_prevIgnoreSize;
        setScale9Enabled(slider->_scale9Enabled);
        auto barSprite = slider->_barRenderer->getSprite();
        if (nullptr != barSprite)
        {
            loadBarTexture(barSprite->getSpriteFrame());
        }
        auto progressSprite = slider->_progressBarRenderer->getSprite();
        if (nullptr != progressSprite)
        {
            loadProgressBarTexture(progressSprite->getSpriteFrame());
        }
        loadSlidBallTextureNormal(slider->_slidBallNormalRenderer->getSprite()->getSpriteFrame());
        loadSlidBallTexturePressed(slider->_slidBallPressedRenderer->getSprite()->getSpriteFrame());
        loadSlidBallTextureDisabled(slider->_slidBallDisabledRenderer->getSprite()->getSpriteFrame());
        loadSlidBallTextureMouseOver(slider->_slidBallMouseOverRenderer->getSprite()->getSpriteFrame());
        setPercent(slider->getPercent());
        setMaxPercent(slider->getMaxPercent());
        _isSliderBallPressedTextureLoaded = slider->_isSliderBallPressedTextureLoaded;
        _isSliderBallDisabledTexturedLoaded = slider->_isSliderBallDisabledTexturedLoaded;
        _isSliderBallMouseOverTexturedLoaded = slider->_isSliderBallMouseOverTexturedLoaded;
        _sliderEventListener = slider->_sliderEventListener;
        _sliderEventSelector = slider->_sliderEventSelector;
        _eventCallback = slider->_eventCallback;
        _ccEventCallback = slider->_ccEventCallback;
    }
}

ResourceData ScrollBar::getBackFile()
{
    ResourceData rData;
    rData.type = (int)_barTexType;
    rData.file = _textureFile;
    return rData;
}
ResourceData ScrollBar::getProgressBarFile()
{
    ResourceData rData;
    rData.type = (int)_progressBarTexType;
    rData.file = _progressBarTextureFile;
    return rData;
}
ResourceData ScrollBar::getBallNormalFile()
{
    ResourceData rData;
    rData.type = (int)_ballNTexType;
    rData.file = _slidBallNormalTextureFile;
    return rData;
}
ResourceData ScrollBar::getBallPressedFile()
{
    ResourceData rData;
    rData.type = (int)_ballPTexType;
    rData.file = _slidBallPressedTextureFile;
    return rData;
}
ResourceData ScrollBar::getBallDisabledFile()
{
    ResourceData rData;
    rData.type = (int)_ballDTexType;
    rData.file = _slidBallDisabledTextureFile;
    return rData;
}
ResourceData ScrollBar::getBallMouseOverFile()
{
    ResourceData rData;
    rData.type = (int)_ballMTexType;
    rData.file = _slidBallMouseOverTextureFile;
    return rData;
}

Node *ScrollBar::getSlidBarNode() const
{
    return _slidBallRenderer;
}
    
}

NS_CC_END
