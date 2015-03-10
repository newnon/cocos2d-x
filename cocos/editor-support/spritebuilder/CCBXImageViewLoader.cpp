#include "CCBXImageViewLoader.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIImageView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_SPRITEFRAME("spriteFrame");
static const std::string PROPERTY_BLENDFUNC("blendFunc");
    
static const std::string PROPERTY_MARGIN_LEFT("marginLeft");
static const std::string PROPERTY_MARGIN_TOP("marginTop");
static const std::string PROPERTY_MARGIN_RIGHT("marginRight");
static const std::string PROPERTY_MARGIN_BOTTOM("marginBottom");

ImageViewLoader *ImageViewLoader::create()
{
    ImageViewLoader *ret = new ImageViewLoader();
    ret->autorelease();
    return ret;
}

Node *ImageViewLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    Rect margin(_margins.origin.x,_margins.origin.y,1.0-_margins.size.width-_margins.origin.x,1.0-_margins.size.height-_margins.origin.y);
    
    ui::ImageView *imageView = ui::ImageView::create();
    switch(_spriteFrame.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
            {
                Size size = _spriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                imageView->loadTexture(_spriteFrame.path, ui::Widget::TextureResType::LOCAL);
                imageView->setCapInsets(realMargins);
            }
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
            {
                Size size = _spriteFrame.spriteFrame->getOriginalSize();
                Rect realMargins(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height);
                imageView->loadTexture(_spriteFrame.path, ui::Widget::TextureResType::PLIST);
                imageView->setCapInsets(realMargins);
            }
            break;
        default:
            break;
    };
    return imageView;
}

void ImageViewLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
}

ImageViewLoader::ImageViewLoader()
{
    
}
ImageViewLoader::~ImageViewLoader(){
    
}
    
void ImageViewLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value) {
    if(propertyName == PROPERTY_BLENDFUNC) {
        //((LayerColor *)pNode)->setBlendFunc(pBlendFunc);
    } else {
        WidgetLoader::onHandlePropTypeBlendFunc(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_SPRITEFRAME) {
        _spriteFrame = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.origin.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.origin.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.size.width = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.size.height = value;
    } else {
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}

/*void Scale9SpriteLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _margins=Rect::ZERO;
    _size=Size::ZERO;
}

void Scale9SpriteLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
     ((Scale9Sprite *)pNode)->setPreferredSize(_size);
    if(ccbReader->getVersion()>5)
    {
        Rect margin(_margins.origin.x,_margins.origin.y,1.0-_margins.size.width-_margins.origin.x,1.0-_margins.size.height-_margins.origin.y);
        Size size = ((Scale9Sprite *)pNode)->getOriginalSize();
        ((Scale9Sprite *)pNode)->setCapInsets(Rect(margin.origin.x*size.width,margin.origin.y*size.height,margin.size.width*size.width,margin.size.height*size.height));
    }
}

void Scale9SpriteLoader::onHandlePropTypeSpriteFrame(Node * pNode, Node * pParent, const char * pPropertyName, SpriteFrame * pSpriteFrame, CCBReader * ccbReader) {
    if((strcmp(pPropertyName, PROPERTY_SPRITEFRAME) == 0)||(strcmp(pPropertyName, PROPERTY_DISPLAYFRAME) == 0)) {
        ((Scale9Sprite *)pNode)->setSpriteFrame(pSpriteFrame);
    } else {
        NodeLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pSpriteFrame, ccbReader);
    }
}

void Scale9SpriteLoader::onHandlePropTypeBlendFunc(Node * pNode, Node * pParent, const char * pPropertyName, BlendFunc pBlendFunc, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        // TODO Not exported by CocosBuilder yet!
        //((Scale9Sprite *)pNode)->setBlendFunc(pBlendFunc);
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pBlendFunc, ccbReader);
    }
}

void Scale9SpriteLoader::onHandlePropTypeSize(Node * pNode, Node * pParent, const char * pPropertyName, const Size &pSize, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_CONTENTSIZE) == 0) {
        _size = pSize;
    } else if(strcmp(pPropertyName, PROPERTY_PREFEREDSIZE) == 0) {
        _size = pSize;
    } else {
        NodeLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, ccbReader);
    }
}

void Scale9SpriteLoader::onHandlePropTypeFloat(Node * pNode, Node * pParent, const char * pPropertyName, float pFloat, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_INSETLEFT) == 0) {
        ((Scale9Sprite *)pNode)->setInsetLeft(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_INSETTOP) == 0) {
        ((Scale9Sprite *)pNode)->setInsetTop(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_INSETRIGHT) == 0) {
        ((Scale9Sprite *)pNode)->setInsetRight(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_INSETBOTTOM) == 0) {
        ((Scale9Sprite *)pNode)->setInsetBottom(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_LEFT) == 0) {
        _margins.origin.x = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_TOP) == 0) {
        _margins.origin.y = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_RIGHT) == 0) {
        _margins.size.width = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_BOTTOM) == 0) {
        _margins.size.height = pFloat;
    } else {
        NodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, ccbReader);
    }
}*/

}

NS_CC_END
