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


#include "CCLayoutNode.h"

NS_CC_BEGIN

LayoutNode::LayoutNode():_needsLayout(true)
{
    
}

LayoutNode::~LayoutNode()
{
    
}

bool LayoutNode::init()
{
    if(!Node::init())
        return false;
    
    needsLayout();
    
    return true;
}

void LayoutNode::needsLayout()
{
    _needsLayout = true;
}

void LayoutNode::layout()
{
    _needsLayout = false;
}

void LayoutNode::visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags)
{
    if (_needsLayout)
        layout();
    Node::visit(renderer, parentTransform, parentFlags);
}

void LayoutNode::addChild(Node *child, int zOrder, int tag)
{
    Node::addChild(child, zOrder, tag);
    needsLayout();
}

void LayoutNode::removeChild(Node* child, bool cleanup)
{
    Node::removeChild(child, cleanup);
    layout();
}

void LayoutNode::removeAllChildrenWithCleanup(bool cleanup)
{
    Node::removeAllChildrenWithCleanup(cleanup);
}

NS_CC_END
