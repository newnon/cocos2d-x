#include "CCButtonLoader.h"
#include "extensions/GUI/CCControlExtension/CCScale9Sprite.h"
#include "2d/CCLabel.h"

using namespace cocos2d;
using namespace cocos2d::extension;

namespace cocosbuilder {;

#define PROPERTY_ZOOMONTOUCHDOWN "zoomOnTouchDown"
#define PROPERTY_ZOOMWHENHIGHLIGHTED "zoomWhenHighlighted"
#define PROPERTY_LABELANCHORPOINT "labelAnchorPoint"
#define PROPERTY_PREFERREDSIZE "preferredSize"
#define PROPERTY_MAXSIZE "maxSize"
#define PROPERTY_BLOCK "block"
    
#define PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL "backgroundSpriteFrame|Normal"
#define PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED "backgroundSpriteFrame|Highlighted"
#define PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED "backgroundSpriteFrame|Disabled"
#define PROPERTY_BACKGROUNDSPRITEFRAME_SELECTED "backgroundSpriteFrame|Selected"
#define PROPERTY_BACKGROUNDSPRITEFRAME_HIHGLIGHTED_SELECTED "backgroundSpriteFrame|HighlightedSelected"
    
#define PROPERTY_BACKGROUNDOPACITY_NORMAL "backgroundOpacity|Normal"
#define PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED "backgroundOpacity|Highlighted"
#define PROPERTY_BACKGROUNDOPACITY_DISABLED "backgroundOpacity|Disabled"
#define PROPERTY_BACKGROUNDOPACITY_SELECTED "backgroundOpacity|Selected"
#define PROPERTY_BACKGROUNDOPACITY_HIHGLIGHTED_SELECTED "backgroundOpacity|HighlightedSelected"
    
#define PROPERTY_LABELOPACITY_NORMAL "labelOpacity|Normal"
#define PROPERTY_LABELOPACITY_HIGHLIGHTED "labelOpacity|Highlighted"
#define PROPERTY_LABELOPACITY_DISABLED "labelOpacity|Disabled"
#define PROPERTY_LABELOPACITY_SELECTED "labelOpacity|Selected"
#define PROPERTY_LABELOPACITY_HIHGLIGHTED_SELECTED "labelOpacity|HighlightedSelected"
    
#define PROPERTY_TITLE "title"
    
#define PROPERTY_TITLECOLOR_NORMAL "labelColor|Normal"
#define PROPERTY_TITLECOLOR_HIGHLIGHTED "labelColor|Highlighted"
#define PROPERTY_TITLECOLOR_DISABLED "labelColor|Disabled"
#define PROPERTY_TITLECOLOR_SELECTED "labelColor|Selected"
#define PROPERTY_TITLECOLOR_HIHGLIGHTED_SELECTED "labelColor|HighlightedSelected"
    
#define PROPERTY_BACKGROUNDCOLOR_NORMAL "backgroundColor|Normal"
#define PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED "backgroundColor|Highlighted"
#define PROPERTY_BACKGROUNDCOLOR_DISABLED "backgroundColor|Disabled"
#define PROPERTY_BACKGROUNDCOLOR_SELECTED "backgroundColor|Selected"
#define PROPERTY_BACKGROUNDCOLOR_HIHGLIGHTED_SELECTED "backgroundColor|HighlightedSelected"
    
#define PROPERTY_HORIZONTALPADDING "horizontalPadding"
#define PROPERTY_VERTICALPADDING "verticalPadding"
    
#define PROPERTY_HORIZONTALALIGMENT "horizontalAlignment"
#define PROPERTY_VERTICALALIGMENT "verticalAlignment"
    
#define PROPERTY_FONTNAME "fontName"
#define PROPERTY_FONTSIZE "fontSize"
    
#define PROPERTY_FONTCOLOR "fontColor"
#define PROPERTY_OUTLINECOLOR "outlineColor"
#define PROPERTY_OUTLINEWIDTH "outlineWidth"
    
#define PROPERTY_SHADOWBLURRADIUS "shadowBlurRadius"
#define PROPERTY_SHADOWWIDTH "shadowWidth"
#define PROPERTY_SHADOWCOLOR "shadowColor"
    
#define PROPERTY_SHADOWOFFSET "shadowOffset"
    
#define PROPERTY_MARGIN_LEFT "marginLeft"
#define PROPERTY_MARGIN_TOP "marginTop"
#define PROPERTY_MARGIN_RIGHT "marginRight"
#define PROPERTY_MARGIN_BOTTOM "marginBottom"
    
#define PROPERTY_OFFSET_LEFT "offsetLeft"
#define PROPERTY_OFFSET_TOP "offsetTop"
#define PROPERTY_OFFSET_RIGHT "offsetRight"
#define PROPERTY_OFFSET_BOTTOM "offsetBottom"

#define PROPERTY_TOGGLESSELECTEDSTATE "togglesSelectedState"

void ButtonLoader::onStarPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    _margins=Rect::ZERO;
    _outlineColor = Color4B(0,0,0,0);
    _shadowColor = Color4B(0,0,0,0);
    _outlineWidth = 0.0f;
    _shadowBlurRadius = 0.0f;
    _shadowOffset = Vec2(0,0);
}

void ButtonLoader::onEndPropertiesParsing(cocos2d::Node * pNode, CCBReader * ccbReader)
{
    ((ButtonControl *)pNode)->setCapInsets(Rect(_margins.origin.x,_margins.origin.y,1.0-_margins.size.width-_margins.origin.x,1.0-_margins.size.height-_margins.origin.y));
    if (_outlineColor.a > 0 && _outlineWidth > 0)
        ((ButtonControl *)pNode)->getLabel()->enableOutline(_outlineColor, _outlineWidth);
    if (_shadowColor.a > 0)
        ((ButtonControl *)pNode)->getLabel()->enableShadow(_shadowColor, _shadowOffset, _shadowBlurRadius);
}
    
void ButtonLoader::onHandlePropTypeIntegerLabeled(cocos2d::Node * pNode, cocos2d::Node * pParent, const char* pPropertyName, int pIntegerLabeled, CCBReader * ccbReader)
{
    if(strcmp(pPropertyName, PROPERTY_HORIZONTALALIGMENT) == 0){
        ((ButtonControl *)pNode)->setHorizontalAlignment(static_cast<TextHAlignment>(pIntegerLabeled));
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALALIGMENT) == 0) {
        ((ButtonControl *)pNode)->setVerticalAlignment(static_cast<TextVAlignment>(pIntegerLabeled));
    } else {
        ControlLoader::onHandlePropTypeIntegerLabeled(pNode, pParent, pPropertyName, pIntegerLabeled, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeCheck(Node * pNode, Node * pParent, const char * pPropertyName, bool pCheck, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_ZOOMWHENHIGHLIGHTED) == 0){
        ((ButtonControl *)pNode)->setZoomWhenHighlighted(pCheck);
    } else if(strcmp(pPropertyName, PROPERTY_TOGGLESSELECTEDSTATE) == 0) {
        ((ButtonControl *)pNode)->setTogglesSelectedState(pCheck);
    } else {
        ControlLoader::onHandlePropTypeCheck(pNode, pParent, pPropertyName, pCheck, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeString(Node * pNode, Node * pParent, const char * pPropertyName, const char * pString, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLE) == 0) {
        ((ButtonControl *)pNode)->setTitle(pString);

    } else {
        ControlLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeText(Node * pNode, Node * pParent, const char * pPropertyName, const char * pString, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLE) == 0) {
        ((ButtonControl *)pNode)->setTitle(pString);
    } else {
        ControlLoader::onHandlePropTypeString(pNode, pParent, pPropertyName, pString, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeFontTTF(Node * pNode, Node * pParent, const char * pPropertyName, const char * pFontTTF, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTNAME) == 0) {
        ((ButtonControl *)pNode)->setFontName(pFontTTF);
    } else {
        ControlLoader::onHandlePropTypeFontTTF(pNode, pParent, pPropertyName, pFontTTF, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypeFloat(cocos2d::Node * pNode, cocos2d::Node * pParent, const char* pPropertyName, float pFloat, CCBReader * ccbReader)
{
    if(strcmp(pPropertyName, PROPERTY_MARGIN_LEFT) == 0) {
        _margins.origin.x = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_TOP) == 0) {
        _margins.origin.y = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_RIGHT) == 0) {
        _margins.size.width = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_MARGIN_BOTTOM) == 0) {
        _margins.size.height = pFloat;
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_LEFT) == 0) {
        ((ButtonControl *)pNode)->setLeftOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_TOP) == 0) {
        ((ButtonControl *)pNode)->setTopOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_RIGHT) == 0) {
        ((ButtonControl *)pNode)->setRightOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_OFFSET_BOTTOM) == 0) {
        ((ButtonControl *)pNode)->setBottomOffset(pFloat);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDOPACITY_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundOpacity(pFloat*255, Control::State::SELECTED_HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_LABELOPACITY_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelOpacity(pFloat*255, Control::State::SELECTED_HIGH_LIGHTED);
    } else {
        ControlLoader::onHandlePropTypeFloat(pNode, pParent, pPropertyName, pFloat, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeFloatScale(Node * pNode, Node * pParent, const char * pPropertyName, float pFloatScale, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTSIZE) == 0) {
        ((ButtonControl *)pNode)->setFontSize(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_HORIZONTALPADDING) == 0) {
        ((ButtonControl *)pNode)->setHorizontalPadding(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_VERTICALPADDING) == 0) {
        ((ButtonControl *)pNode)->setVerticalPadding(pFloatScale);
    } else if(strcmp(pPropertyName, PROPERTY_OUTLINEWIDTH) == 0) {
        _outlineWidth = pFloatScale;
    } else if(strcmp(pPropertyName, PROPERTY_SHADOWBLURRADIUS) == 0) {
        _shadowBlurRadius = pFloatScale;
    } else {
        ControlLoader::onHandlePropTypeFloatScale(pNode, pParent, pPropertyName, pFloatScale, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypePoint(Node * pNode, Node * pParent, const char * pPropertyName, const Point &pPoint, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_LABELANCHORPOINT) == 0) {
        //((Button *)pNode)->setLabelAnchorPoint(pPoint);
    } else {
        ControlLoader::onHandlePropTypePoint(pNode, pParent, pPropertyName, pPoint, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeSize(Node * pNode, Node * pParent, const char * pPropertyName, const Size &pSize, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_PREFERREDSIZE) == 0) {
        ((ButtonControl *)pNode)->setPreferredSize(pSize);
    } else if(strcmp(pPropertyName, PROPERTY_MAXSIZE) == 0) {
        ((ButtonControl *)pNode)->setMaxSize(pSize);
    } else {
        ControlLoader::onHandlePropTypeSize(pNode, pParent, pPropertyName, pSize, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeSpriteFrame(Node * pNode, Node * pParent, const char * pPropertyName, SpriteFrame * pSpriteFrame, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_NORMAL) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::NORMAL);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIGHLIGHTED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::HIGH_LIGHTED);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_DISABLED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::DISABLED);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_SELECTED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::SELECTED);
        }
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDSPRITEFRAME_HIHGLIGHTED_SELECTED) == 0) {
        if(pSpriteFrame != NULL) {
            ((ButtonControl *)pNode)->setBackgroundSpriteFrame(pSpriteFrame, Control::State::SELECTED_HIGH_LIGHTED);
        }
    } else {
        ControlLoader::onHandlePropTypeSpriteFrame(pNode, pParent, pPropertyName, pSpriteFrame, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypeBlock(cocos2d::Node * pNode, cocos2d::Node * pParent, const char * pPropertyName, BlockData * pBlockData, CCBReader * ccbReader)
{
    if(strcmp(pPropertyName, PROPERTY_BLOCK) == 0) {
        if(pBlockData)
            ((ButtonControl *)pNode)->setCallback( std::bind( pBlockData->mSELMenuHandler, pBlockData->_target, std::placeholders::_1) );
    } else {
        NodeLoader::onHandlePropTypeBlock(pNode, pParent, pPropertyName, pBlockData, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypeColor4(Node * pNode, Node * pParent, const char * pPropertyName, const Color4B &pColor4B, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_FONTCOLOR) == 0){
		((ButtonControl *)pNode)->getLabel()->setTextColor(pColor4B);
    } else if(strcmp(pPropertyName, PROPERTY_OUTLINECOLOR) == 0){
        _outlineColor = pColor4B;
    } else if(strcmp(pPropertyName, PROPERTY_SHADOWCOLOR) == 0){
        _shadowColor = pColor4B;
    } else {
        ControlLoader::onHandlePropTypeColor4(pNode, pParent, pPropertyName, pColor4B, ccbReader);
    }
}

void ButtonLoader::onHandlePropTypeColor3(Node * pNode, Node * pParent, const char * pPropertyName, const Color3B &pColor3B, CCBReader * ccbReader) {
    if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_TITLECOLOR_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setLabelColor(pColor3B, Control::State::SELECTED_HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_NORMAL) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::NORMAL);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_HIGHLIGHTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::HIGH_LIGHTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_DISABLED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::DISABLED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::SELECTED);
    } else if(strcmp(pPropertyName, PROPERTY_BACKGROUNDCOLOR_HIHGLIGHTED_SELECTED) == 0) {
        ((ButtonControl *)pNode)->setBackgroundColor(pColor3B, Control::State::SELECTED_HIGH_LIGHTED);
    } else {
        ControlLoader::onHandlePropTypeColor3(pNode, pParent, pPropertyName, pColor3B, ccbReader);
    }
}
    
void ButtonLoader::onHandlePropTypePosition(Node * pNode, Node * pParent, const char* pPropertyName, const Point &pPosition, CCBReader * pCCBReader) {
    if(strcmp(pPropertyName, PROPERTY_SHADOWOFFSET) == 0) {
        _shadowOffset = pPosition;
    } else {
        NodeLoader::onHandlePropTypePosition(pNode, pParent, pPropertyName, pPosition, pCCBReader);
    }
}

};