#include "CCBXImageViewLoader.h"
#include "ui/UIScale9Sprite.h"
#include "ui/UIImageView.h"
#include "ui/UITileImageView.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_SPRITEFRAME("spriteFrame");
static const std::string PROPERTY_BLENDFUNC("blendFunc");
    
static const std::string PROPERTY_MARGIN("margin");
static const std::string PROPERTY_MARGIN_LEFT("marginLeft");
static const std::string PROPERTY_MARGIN_TOP("marginTop");
static const std::string PROPERTY_MARGIN_RIGHT("marginRight");
static const std::string PROPERTY_MARGIN_BOTTOM("marginBottom");
static const std::string PROPERTY_IMAGESCALE("imageScale");
static const std::string PROPERTY_FLIP("flip");
static const std::string PROPERTY_RENDERINGTYPE("renderingType");

ImageViewLoader *ImageViewLoader::create()
{
    ImageViewLoader *ret = new ImageViewLoader();
    ret->autorelease();
    return ret;
}

Node *ImageViewLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const
{
    if(_renderingType == RenderingType::TILED)
    {
        ui::TileImageView *imageView = ui::TileImageView::create();
        imageView->setAnchorPoint(Vec2(0.0f, 0.0f));
        return imageView;
    }
    else
    {
        ui::ImageView *imageView = ui::ImageView::create();
        imageView->setAnchorPoint(Vec2(0.0f, 0.0f));
        return imageView;
    }
}
    
inline ui::Widget::TextureResType convertTextureResType(SpriteFrameDescription::TextureResType value)
{
    return static_cast<ui::Widget::TextureResType>(static_cast<int>(value) - 1);
}

void ImageViewLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const
{
    WidgetLoader::setSpecialProperties(node, parentSize, mainScale, additionalScale, owner, rootNode, customProperties, params);
    
    const SpriteFrameDescription &spriteFrameDesc = getNodeParamValue(params, PROPERTY_SPRITEFRAME, _spriteFrameDesc);
    
    if(_renderingType == RenderingType::TILED)
    {
        ui::TileImageView *imageView = static_cast<ui::TileImageView*>(node);
        
        imageView->ignoreContentAdaptWithSize(false);
        if(spriteFrameDesc.type != SpriteFrameDescription::TextureResType::NONE)
            imageView->loadTexture(spriteFrameDesc.path, convertTextureResType(spriteFrameDesc.type));
        imageView->setImageScale(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_IMAGESCALE, _imageScale)) / CCBXReader::getResolutionScale());
        const std::pair<bool,bool> &flipped = getNodeParamValue(params, PROPERTY_FLIP, _flipped);
        imageView->setFlippedX(flipped.first);
        imageView->setFlippedY(flipped.second);
    }
    else
    {
        ui::ImageView *imageView = static_cast<ui::ImageView*>(node);
        
        imageView->ignoreContentAdaptWithSize(false);
        const Vec4 &margins = getNodeParamValue(params, PROPERTY_MARGIN, _margins);
        if(spriteFrameDesc.type != SpriteFrameDescription::TextureResType::NONE)
        {
            imageView->loadTexture(spriteFrameDesc.path, convertTextureResType(spriteFrameDesc.type));
            imageView->setCapInsets(calcMargins(margins, spriteFrameDesc.spriteFrame->getOriginalSize()));
        }
        imageView->setScale9Enabled(_renderingType == RenderingType::SIMPLE ? false : margins != Vec4::ZERO);
        imageView->setImageScale(getAbsoluteScale(mainScale, additionalScale, getNodeParamValue(params, PROPERTY_IMAGESCALE, _imageScale)) / CCBXReader::getResolutionScale());
        imageView->setBlendFunc(getNodeParamValue(params, PROPERTY_BLENDFUNC, _blendFunc));
        const std::pair<bool,bool> &flipped = getNodeParamValue(params, PROPERTY_FLIP, _flipped);
        imageView->setFlippedX(flipped.first);
        imageView->setFlippedY(flipped.second);
    }
}

ImageViewLoader::ImageViewLoader(RenderingType renderingType)
    :_imageScale{0,1.f}
    ,_blendFunc(BlendFunc::ALPHA_PREMULTIPLIED)
    ,_flipped(false, false)
    ,_renderingType(renderingType)
{
    
}
ImageViewLoader::~ImageViewLoader(){
    
}
    
void ImageViewLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value) {
    if(propertyName == PROPERTY_BLENDFUNC) {
        _blendFunc = value;
    } else {
        WidgetLoader::onHandlePropTypeBlendFunc(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_SPRITEFRAME) {
        _spriteFrameDesc = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeOffsets(const std::string &propertyName, bool isExtraProp, const Vec4 &value)
{
    if(propertyName == PROPERTY_MARGIN) {
        _margins = value;
    } else {
        NodeLoader::onHandlePropTypeOffsets(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if(propertyName == PROPERTY_MARGIN_LEFT) {
        _margins.x = value;
    } else if(propertyName == PROPERTY_MARGIN_TOP) {
        _margins.y = value;
    } else if(propertyName == PROPERTY_MARGIN_RIGHT) {
        _margins.z = value;
    } else if(propertyName == PROPERTY_MARGIN_BOTTOM) {
        _margins.w = value;
    } else {
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeFloatScale(const std::string &propertyName, bool isExtraProp, const FloatScaleDescription &value)
{
    if(propertyName == PROPERTY_IMAGESCALE) {
        _imageScale = value;
    } else {
        WidgetLoader::onHandlePropTypeFloatScale(propertyName, isExtraProp, value);
    }
}
 
void ImageViewLoader::onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &value)
{
    if(propertyName == PROPERTY_FLIP) {
        _flipped = value;
    } else {
        NodeLoader::onHandlePropTypeFlip(propertyName, isExtraProp, value);
    }
}
    
void ImageViewLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_RENDERINGTYPE) {
        _renderingType = static_cast<RenderingType>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}
   
TileImageViewLoader *TileImageViewLoader::create()
{
    TileImageViewLoader *ret = new TileImageViewLoader();
    ret->autorelease();
    return ret;
}

TileImageViewLoader::TileImageViewLoader():ImageViewLoader(ImageViewLoader::RenderingType::TILED)
{
    
}
    
void TileImageViewLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if(propertyName == PROPERTY_RENDERINGTYPE) {
        assert(false);
    } else {
        ImageViewLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

}

NS_CC_END
