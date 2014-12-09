/*
 * Copyright (c) 2014      Perepelitsa Sergey
 * Copyright (c) 2014      cocos2d-x.org
 *
 * cocos2d-x: http://www.cocos2d-x.org
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
 *
 */


#include "CCLayoutBox.h"

NS_CC_BEGIN

static float roundUpToEven(float f)
{
    return ceilf(f/2.0f) * 2.0f;
}

LayoutBox::LayoutBox():_direction(LayoutBoxDirection::Horizontal),_spacing(0.0f)
{
    
}
LayoutBox::~LayoutBox()
{
    
}

LayoutBox* LayoutBox::create()
{
    LayoutBox* ret = new LayoutBox;
    if (ret && ret->init())
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    
    return ret;
}

void LayoutBox::layout()
{
    LayoutNode::layout();
    if (_direction == LayoutBoxDirection::Horizontal)
    {
        // Get the maximum height
        float maxHeight = 0;
        for (Node* child : this->_children)
        {
            float height = child->getContentSize().height;
            if (height > maxHeight) maxHeight = height;
        }
        
        // Position the nodes
        float width = 0;
        for (Node* child : this->_children)
        {
            if(child->isVisible())
            {
                Size childSize = child->getContentSize();
                
                Point offset = child->getAnchorPointInPoints();
                Point localPos = Point(roundf(width), roundf((maxHeight-childSize.height)/2.0f));
                Point position = localPos + offset;
                
                child->setPosition(position);
                
                width += childSize.width;
                width += _spacing;
            }
        }
        
        // Account for last added increment
        width -= _spacing;
        if (width < 0) width = 0;
        
        setContentSize(Size(roundUpToEven(width), roundUpToEven(maxHeight)));
    }
    else
    {
        // Get the maximum width
        float maxWidth = 0;
        for (Node* child : this->_children)
        {
            float width = child->getContentSize().width;
            if (width > maxWidth) maxWidth = width;
        }
        
        // Position the nodes
        float height = 0;
        for (Node* child : this->_children)
        {
            if(child->isVisible())
            {
                Size childSize = child->getContentSize();
                
                Point offset = child->getAnchorPointInPoints();
                Point localPos = Point(roundf((maxWidth-childSize.width)/2.0f), roundf(height));
                Point position = localPos + offset;
                
                child->setPosition(position);
                
                height += childSize.height;
                height += _spacing;
            }
        }
        
        // Account for last added increment
        height -= _spacing;
        if (height < 0) height = 0;
        
        setContentSize(Size(roundUpToEven(maxWidth), roundUpToEven(height)));
    }
}

LayoutBoxDirection LayoutBox::getDirection() const
{
    return  _direction;
}

void LayoutBox::setDirection(LayoutBoxDirection direction)
{
    _direction = direction;
}

float LayoutBox::getSpacing() const
{
    return  _spacing;
}
void LayoutBox::setSpacing(float spacing)
{
    _spacing = spacing;
}

NS_CC_END
