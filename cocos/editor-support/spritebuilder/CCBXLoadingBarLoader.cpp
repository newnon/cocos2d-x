#include "CCBXLoadingBarLoader.h"

NS_CC_BEGIN
namespace spritebuilder {
    
static const std::string PROPERTY_FLIP("flip");
static const std::string PROPERTY_SPRITEFRAME("spriteFrame");
static const std::string PROPERTY_BLENDFUNC("blendFunc");
static const std::string PROPERTY_DIRECTION("direction");
static const std::string PROPERTY_PERCENTAGE("percentage");
    
LoadingBarLoader *LoadingBarLoader::create()
{
    LoadingBarLoader *ret = new LoadingBarLoader();
    ret->autorelease();
    return ret;
}

Node *LoadingBarLoader::createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode)
{
    ui::LoadingBar *loadingBar = ui::LoadingBar::create();
    switch(_spriteFrame.type)
    {
        case SpriteFrameDescription::TextureResType::LOCAL:
            loadingBar->loadTexture(_spriteFrame.path, ui::Widget::TextureResType::LOCAL);
            break;
        case SpriteFrameDescription::TextureResType::PLIST:
            loadingBar->loadTexture(_spriteFrame.path, ui::Widget::TextureResType::PLIST);
            break;
        default:
            break;
    };
    return loadingBar;
}

void LoadingBarLoader::setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale)
{
    ui::LoadingBar *loadingBar = static_cast<ui::LoadingBar*>(node);
    loadingBar->setDirection(_direction);
}
    
LoadingBarLoader::LoadingBarLoader()
    :_direction(ui::LoadingBar::Direction::LEFT)
{
    
}

LoadingBarLoader::~LoadingBarLoader()
{
    
}
    
void LoadingBarLoader::onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &value)
{
        
}
    
void LoadingBarLoader::onHandlePropTypeSpriteFrame(const std::string &propertyName, bool isExtraProp, const SpriteFrameDescription &value)
{
    if(propertyName == PROPERTY_SPRITEFRAME) {
        _spriteFrame = value;
    } else {
        WidgetLoader::onHandlePropTypeSpriteFrame(propertyName, isExtraProp, value);
    }
}
    
void LoadingBarLoader::onHandlePropTypeFlip(const std::string &propertyName, bool isExtraProp, const std::pair<bool,bool> &value)
{
    if(propertyName == PROPERTY_FLIP) {
        _flipped = value;
    } else {
        WidgetLoader::onHandlePropTypeFlip(propertyName, isExtraProp, value);
    }
}
    
void LoadingBarLoader::onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value)
{
    if (propertyName == PROPERTY_DIRECTION) {
        _direction =  static_cast<ui::LoadingBar::Direction>(value);
    } else {
        WidgetLoader::onHandlePropTypeIntegerLabeled(propertyName, isExtraProp, value);
    }
}

void LoadingBarLoader::onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value)
{
    if (propertyName == PROPERTY_PERCENTAGE) {
        _percentage = value;
    } else {
        WidgetLoader::onHandlePropTypeFloat(propertyName, isExtraProp, value);
    }
}
    
    
/*void ProgressTimerLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _percentage = 0.0f;
}
    
void ProgressTimerLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    ((ProgressTimer *)pNode)->setPercentage(_percentage);
}

void ProgressTimerLoader::onHandlePropTypeSpriteFrame(Node * pNode, Node * pParent, const char * pPropertyName, SpriteFrame * pCCSpriteFrame, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_DISPLAYFRAME) == 0) {
        if(pCCSpriteFrame != NULL) {
            Sprite *temp=((ProgressTimer *)pNode)->getSprite();
            temp->retain();
            ((ProgressTimer *)pNode)->setSprite(NULL);
			temp->setSpriteFrame(pCCSpriteFrame);
            ((ProgressTimer *)pNode)->setSprite(temp);
            temp->release();
        } else {
            CCLOG("ERROR: SpriteFrame NULL");
        }
    } else {
        NodeLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pCCSpriteFrame, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeFlip(Node * pNode, Node * pParent, const char * pPropertyName, bool * pFlip, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_FLIP) == 0) {
        ((ProgressTimer *)pNode)->getSprite()->setFlippedX(pFlip[0]);
        ((ProgressTimer *)pNode)->getSprite()->setFlippedY(pFlip[1]);
        ((ProgressTimer *)pNode)->setSprite(((ProgressTimer *)pNode)->getSprite());
    } else {
        NodeLoader::onHandlePropTypeFlip(pNode, pParent, pPropertyName, pFlip, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeBlendFunc(Node * pNode, Node * pParent, const char * pPropertyName, BlendFunc pCCBlendFunc, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_BLENDFUNC) == 0) {
        ((ProgressTimer *)pNode)->getSprite()->setBlendFunc(pCCBlendFunc);
    } else {
        NodeLoader::onHandlePropTypeBlendFunc(pNode, pParent, pPropertyName, pCCBlendFunc, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeIntegerLabeled(Node *pNode, Node *pParent, const char *pPropertyName, int pIntegerLabeled, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_TYPE) == 0) {
        ((ProgressTimer *)pNode)->setType((ProgressTimer::Type)pIntegerLabeled);
    } else {
        NodeLoader::onHandlePropTypeIntegerLabeled(pNode, pParent, pPropertyName, pIntegerLabeled, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypePoint(Node *pNode, Node *pParent, const char *pPropertyName, const Point &pPoint, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_MIDPOINT) == 0) {
        ((ProgressTimer *)pNode)->setMidpoint(pPoint);
    } else if (strcmp(pPropertyName, PROPERTY_BARCHANGERATE) == 0) {
        ((ProgressTimer *)pNode)->setBarChangeRate(pPoint);
    } else if (strcmp(pPropertyName, "anchorPoint") == 0) {
        ((ProgressTimer *)pNode)->setAnchorPoint(pPoint);
    }
    else {
        NodeLoader::onHandlePropTypePoint(pNode, pParent, pPropertyName, pPoint, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeFloat(Node *pNode, Node *pParent, const char *pPropertyName, float pFloat, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_PERCENTAGE) == 0) {
        _percentage = pFloat;
    } else {
        NodeLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, pCCBReader);
    }
}

void ProgressTimerLoader::onHandlePropTypeCheck(Node *pNode, Node *pParent, const char *pPropertyName, bool pCheck, CCBReader *pCCBReader)
{
    if (strcmp(pPropertyName, PROPERTY_REVERSEDIR) == 0) {
        ((ProgressTimer *)pNode)->setReverseDirection(pCheck);
    }
    else {
        NodeLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, pCCBReader);
    }
}*/

}

NS_CC_END
