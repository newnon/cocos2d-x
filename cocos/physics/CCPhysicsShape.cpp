/****************************************************************************
 Copyright (c) 2013 Chukong Technologies Inc.
 
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

#include "physics/CCPhysicsShape.h"
#if CC_USE_PHYSICS

#include <climits>

#include "chipmunk.h"
#include "chipmunk_unsafe.h"

#include "physics/CCPhysicsBody.h"
#include "physics/CCPhysicsWorld.h"

#include "chipmunk/CCPhysicsBodyInfo_chipmunk.h"
#include "chipmunk/CCPhysicsShapeInfo_chipmunk.h"
#include "chipmunk/CCPhysicsHelper_chipmunk.h"

NS_CC_BEGIN
extern const float PHYSICS_INFINITY;

static cpFloat
RadiusForTransform(AffineTransform t)
{
	// Return the magnitude of the longest basis vector.
	return cpfsqrt(MAX(t.a*t.a + t.b*t.b, t.c*t.c + t.d*t.d));
}

static cpFloat
ScaleXTransform(AffineTransform t)
{
	return cpfsqrt(t.a*t.a + t.b*t.b);
}

static cpFloat
ScaleYTransform(AffineTransform t)
{
	return cpfsqrt(t.c*t.c + t.d*t.d);
}

static inline cpVect
cpTransformPoint(AffineTransform t, Point p)
{
    return cpv(t.a*p.x + t.c*p.y, t.b*p.x + t.d*p.y);
}


PhysicsShape::PhysicsShape()
: _body(nullptr)
, _info(nullptr)
, _type(Type::UNKNOWN)
, _area(0)
, _mass(0)
, _moment(0)
, _tag(0)
, _categoryBitmask(UINT_MAX)
, _collisionBitmask(UINT_MAX)
, _contactTestBitmask(0)
, _group(0)
{
    
}

PhysicsShape::~PhysicsShape()
{
    CC_SAFE_DELETE(_info);
}

bool PhysicsShape::init(Type type)
{
    _info = new PhysicsShapeInfo(this);
    if (_info == nullptr) return false;
    
    _type = type;
    
    return true;
}

void PhysicsShape::setMass(float mass)
{
    if (mass < 0)
    {
        return;
    }
    
    if (_body)
    {
        _body->addMass(-_mass);
        _body->addMass(mass);
    };
    
    _mass = mass;
}

void PhysicsShape::setMoment(float moment)
{
    if (moment < 0)
    {
        return;
    }
    
    if (_body)
    {
        _body->addMoment(-_moment);
        _body->addMoment(moment);
    };
    
    _moment = moment;
}

void PhysicsShape::setMaterial(const PhysicsMaterial& material)
{
    setDensity(material.density);
    setRestitution(material.restitution);
    setFriction(material.friction);
}

PhysicsBodyInfo* PhysicsShape::bodyInfo() const
{
    if (_body != nullptr)
    {
        return _body->_info;
    }else
    {
        return nullptr;
    }
}

PhysicsShapeCircle::PhysicsShapeCircle()
{
    
}

PhysicsShapeCircle::~PhysicsShapeCircle()
{
    
}

PhysicsShapeBox::PhysicsShapeBox()
{
    
}

PhysicsShapeBox::~PhysicsShapeBox()
{
    
}

PhysicsShapePolygon::PhysicsShapePolygon()
{
    
}

PhysicsShapePolygon::~PhysicsShapePolygon()
{
    
}

PhysicsShapeEdgeBox::PhysicsShapeEdgeBox()
{
    
}

PhysicsShapeEdgeBox::~PhysicsShapeEdgeBox()
{
    
}

PhysicsShapeEdgeChain::PhysicsShapeEdgeChain()
{
    
}

PhysicsShapeEdgeChain::~PhysicsShapeEdgeChain()
{
    
}

PhysicsShapeEdgePolygon::PhysicsShapeEdgePolygon()
{
    
}

PhysicsShapeEdgePolygon::~PhysicsShapeEdgePolygon()
{
    
}

PhysicsShapeEdgeSegment::PhysicsShapeEdgeSegment()
{
    
}

PhysicsShapeEdgeSegment::~PhysicsShapeEdgeSegment()
{
    
}

void PhysicsShape::setDensity(float density)
{
    if (density < 0)
    {
        return;
    }
    
    _material.density = density;
    
    if (_material.density == PHYSICS_INFINITY)
    {
        setMass(PHYSICS_INFINITY);
    }else if (_area > 0)
    {
        setMass(PhysicsHelper::float2cpfloat(_material.density * _area));
    }
}

void PhysicsShape::setRestitution(float restitution)
{
    _material.restitution = restitution;
    
    for (cpShape* shape : _info->getShapes())
    {
        cpShapeSetElasticity(shape, PhysicsHelper::float2cpfloat(restitution));
    }
}

void PhysicsShape::setFriction(float friction)
{
    _material.friction = friction;
    
    for (cpShape* shape : _info->getShapes())
    {
        cpShapeSetFriction(shape, PhysicsHelper::float2cpfloat(friction));
    }
}


void PhysicsShape::recenterPoints(Vec2* points, int count, const Vec2& center)
{
    cpVect* cpvs = new cpVect[count];
    cpRecenterPoly(count, PhysicsHelper::points2cpvs(points, cpvs, count));
    PhysicsHelper::cpvs2points(cpvs, points, count);
    delete[] cpvs;
    
    if (center != Vec2::ZERO)
    {
        for (int i = 0; i < count; ++i)
        {
            points[i] += center;
        }
    }
}

Vec2 PhysicsShape::getPolyonCenter(const Vec2* points, int count)
{
    cpVect* cpvs = new cpVect[count];
    cpVect center = cpCentroidForPoly(count, PhysicsHelper::points2cpvs(points, cpvs, count));
    delete[] cpvs;
    
    return PhysicsHelper::cpv2point(center);
}

void PhysicsShape::setBody(PhysicsBody *body)
{
    // already added
    if (body != nullptr && _body == body)
    {
        return;
    }
    
    if (_body != nullptr)
    {
        _body->removeShape(this);
    }
    
    if (body == nullptr)
    {
        _info->setBody(nullptr);
        _body = nullptr;
    }else
    {
        _info->setBody(body->_info->getBody());
        _body = body;
    }
}

// PhysicsShapeCircle
PhysicsShapeCircle* PhysicsShapeCircle::create(float radius, const PhysicsMaterial& material/* = MaterialDefault*/, const Vec2& offset/* = Vec2(0, 0)*/)
{
    PhysicsShapeCircle* shape = new PhysicsShapeCircle();
    if (shape && shape->init(radius, material, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeCircle::init(float radius, const PhysicsMaterial& material/* = MaterialDefault*/, const Vec2& offset /*= Vec2(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::CIRCLE));
        
        cpShape* shape = cpCircleShapeNew(_info->getSharedBody(), radius, PhysicsHelper::point2cpv(offset));
        
        CC_BREAK_IF(shape == nullptr);
        
        _info->add(shape);
        
        _radius = radius;
        _center = offset;
        
        _area = calculateArea();
        _mass = material.density == PHYSICS_INFINITY ? PHYSICS_INFINITY : material.density * _area;
        _moment = calculateDefaultMoment();
        
        setMaterial(material);
        return true;
    } while (false);
    
    return false;
}

float PhysicsShapeCircle::calculateArea(float radius)
{
    return PhysicsHelper::cpfloat2float(cpAreaForCircle(0, radius));
}

float PhysicsShapeCircle::calculateMoment(float mass, float radius, const Vec2& offset)
{
    return mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForCircle(PhysicsHelper::float2cpfloat(mass),
                                                     0,
                                                     PhysicsHelper::float2cpfloat(radius),
                                                     PhysicsHelper::point2cpv(offset)));
}

float PhysicsShapeCircle::calculateArea()
{
    return PhysicsHelper::cpfloat2float(cpAreaForCircle(0, cpCircleShapeGetRadius(_info->getShapes().front())));
}

float PhysicsShapeCircle::calculateDefaultMoment()
{
    cpShape* shape = _info->getShapes().front();
    
    return _mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForCircle(PhysicsHelper::float2cpfloat(_mass),
                                                     0,
                                                     cpCircleShapeGetRadius(shape),
                                                     cpCircleShapeGetOffset(shape)));
}

float PhysicsShapeCircle::getRadius() const
{
    return PhysicsHelper::cpfloat2float(cpCircleShapeGetRadius(_info->getShapes().front()));
}

Vec2 PhysicsShapeCircle::getOffset()
{
    return PhysicsHelper::cpv2point(cpCircleShapeGetOffset(_info->getShapes().front()));
}

void PhysicsShapeCircle::rescale(const AffineTransform &transform)
{
    cpShape *shape = _info->getShapes().front();
    cpCircleShapeSetRadius(shape, _radius*RadiusForTransform(transform));
	cpCircleShapeSetOffset(shape, cpTransformPoint(transform, _center));
}

// PhysicsShapeEdgeSegment
PhysicsShapeEdgeSegment* PhysicsShapeEdgeSegment::create(const Vec2& a, const Vec2& b, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    PhysicsShapeEdgeSegment* shape = new PhysicsShapeEdgeSegment();
    if (shape && shape->init(a, b, material, border))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgeSegment::init(const Vec2& a, const Vec2& b, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGESEGMENT));
        
        cpShape* shape = cpSegmentShapeNew(_info->getSharedBody(),
                                           PhysicsHelper::point2cpv(a),
                                           PhysicsHelper::point2cpv(b),
                                           PhysicsHelper::float2cpfloat(border));
        
        CC_BREAK_IF(shape == nullptr);
        
        _info->add(shape);
        
        _a = a;
        _b = b;
        _radius = border;
        
        _mass = PHYSICS_INFINITY;
        _moment = PHYSICS_INFINITY;
        _center = a.getMidpoint(b);
        
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

Vec2 PhysicsShapeEdgeSegment::getPointA() const
{
    return PhysicsHelper::cpv2point(cpSegmentShapeGetA(_info->getShapes().front()));
}

Vec2 PhysicsShapeEdgeSegment::getPointB() const
{
    return PhysicsHelper::cpv2point(cpSegmentShapeGetB(_info->getShapes().front()));
}

Vec2 PhysicsShapeEdgeSegment::getCenter()
{
    return _center;
}

void PhysicsShapeEdgeSegment::rescale(const AffineTransform &transform)
{
    cpShape *shape = _info->getShapes().front();
    
    cpSegmentShapeSetEndpoints(shape, cpTransformPoint(transform, _a), cpTransformPoint(transform, _b));
    cpSegmentShapeSetRadius(shape, PhysicsHelper::float2cpfloat(_radius*RadiusForTransform(transform)));
}

// PhysicsShapeBox
PhysicsShapeBox* PhysicsShapeBox::create(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, const Vec2& offset/* = Vec2(0, 0)*/)
{
    PhysicsShapeBox* shape = new PhysicsShapeBox();
    if (shape && shape->init(size, material, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeBox::init(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, const Vec2& offset /*= Vec2(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::BOX));
        
        cpVect wh = PhysicsHelper::size2cpv(size);
        cpVect vec[4] =
        {
            {-wh.x/2.0f, -wh.y/2.0f}, {-wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, -wh.y/2.0f}
        };
        
        _points[0] = Point(-wh.x/2.0f, -wh.y/2.0f);
        _points[1] = Point(-wh.x/2.0f, wh.y/2.0f);
        _points[2] = Point(wh.x/2.0f, wh.y/2.0f);
        _points[3] = Point(wh.x/2.0f, -wh.y/2.0f);
        
        cpShape* shape = cpPolyShapeNew(_info->getSharedBody(), 4, vec, PhysicsHelper::point2cpv(offset));
        
        CC_BREAK_IF(shape == nullptr);
        
        _info->add(shape);
        
        _offset = offset;
        _area = calculateArea();
        _mass = material.density == PHYSICS_INFINITY ? PHYSICS_INFINITY : material.density * _area;
        _moment = calculateDefaultMoment();
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

float PhysicsShapeBox::calculateArea(const Size& size)
{
    cpVect wh = PhysicsHelper::size2cpv(size);
    cpVect vec[4] =
    {
        {-wh.x/2.0f, -wh.y/2.0f}, {-wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, -wh.y/2.0f}
    };
    return PhysicsHelper::cpfloat2float(cpAreaForPoly(4, vec));
}

float PhysicsShapeBox::calculateMoment(float mass, const Size& size, const Vec2& offset)
{
    cpVect wh = PhysicsHelper::size2cpv(size);
    cpVect vec[4] =
    {
        {-wh.x/2.0f, -wh.y/2.0f}, {-wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, wh.y/2.0f}, {wh.x/2.0f, -wh.y/2.0f}
    };
    
    return mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(PhysicsHelper::float2cpfloat(mass),
                                                   4,
                                                   vec,
                                                   PhysicsHelper::point2cpv(offset)));
}

float PhysicsShapeBox::calculateArea()
{
    cpShape* shape = _info->getShapes().front();
    return PhysicsHelper::cpfloat2float(cpAreaForPoly(((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts));
}

float PhysicsShapeBox::calculateDefaultMoment()
{
    cpShape* shape = _info->getShapes().front();
    return _mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(_mass, ((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts, cpvzero));
}

void PhysicsShapeBox::getPoints(Vec2* points) const
{
    cpShape* shape = _info->getShapes().front();
    PhysicsHelper::cpvs2points(((cpPolyShape*)shape)->verts, points, ((cpPolyShape*)shape)->numVerts);
}

Size PhysicsShapeBox::getSize() const
{
    cpShape* shape = _info->getShapes().front();
    return PhysicsHelper::cpv2size(cpv(cpvdist(cpPolyShapeGetVert(shape, 1), cpPolyShapeGetVert(shape, 2)),
                                       cpvdist(cpPolyShapeGetVert(shape, 0), cpPolyShapeGetVert(shape, 1))));
}

void PhysicsShapeBox::rescale(const AffineTransform &transform)
{
    cpShape *shape = _info->getShapes().front();
    
    cpVect hullVerts[4];
    for(int i=0; i<4; i++)
        hullVerts[i] =  cpTransformPoint(transform, _points[i]);
	
	unsigned int hullCount = cpConvexHull(4, hullVerts, hullVerts, NULL, 0.0);
    cpPolyShapeSetVerts(shape, hullCount, hullVerts, PhysicsHelper::point2cpv(_offset));
}

// PhysicsShapePolygon
PhysicsShapePolygon* PhysicsShapePolygon::create(const Vec2* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, const Vec2& offset/* = Vec2(0, 0)*/)
{
    PhysicsShapePolygon* shape = new PhysicsShapePolygon();
    if (shape && shape->init(points, count, material, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapePolygon::init(const Vec2* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, const Vec2& offset/* = Vec2(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::POLYGEN));
        
        cpVect* vecs = new cpVect[count];
        PhysicsHelper::points2cpvs(points, vecs, count);
        _points.assign(points, points+count);
        cpConvexHull((int)count, vecs, nullptr, nullptr, 0);
        cpShape* shape = cpPolyShapeNew(_info->getSharedBody(), count, vecs, PhysicsHelper::point2cpv(offset));
        CC_SAFE_DELETE_ARRAY(vecs);
        
        CC_BREAK_IF(shape == nullptr);
        
        _info->add(shape);
        
        _area = calculateArea();
        _offset = offset;
        _mass = material.density == PHYSICS_INFINITY ? PHYSICS_INFINITY : material.density * _area;
        _moment = calculateDefaultMoment();
        _center = PhysicsHelper::cpv2point(cpCentroidForPoly(((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts));
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

float PhysicsShapePolygon::calculateArea(const Vec2* points, int count)
{
    cpVect* vecs = new cpVect[count];
    PhysicsHelper::points2cpvs(points, vecs, count);
    float area = PhysicsHelper::cpfloat2float(cpAreaForPoly(count, vecs));
    CC_SAFE_DELETE_ARRAY(vecs);
    
    return area;
}

float PhysicsShapePolygon::calculateMoment(float mass, const Vec2* points, int count, const Vec2& offset)
{
    cpVect* vecs = new cpVect[count];
    PhysicsHelper::points2cpvs(points, vecs, count);
    float moment = mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(mass, count, vecs, PhysicsHelper::point2cpv(offset)));
    CC_SAFE_DELETE_ARRAY(vecs);
    
    return moment;
}

float PhysicsShapePolygon::calculateArea()
{
    cpShape* shape = _info->getShapes().front();
    return PhysicsHelper::cpfloat2float(cpAreaForPoly(((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts));
}

float PhysicsShapePolygon::calculateDefaultMoment()
{
    cpShape* shape = _info->getShapes().front();
    return _mass == PHYSICS_INFINITY ? PHYSICS_INFINITY
    : PhysicsHelper::cpfloat2float(cpMomentForPoly(_mass, ((cpPolyShape*)shape)->numVerts, ((cpPolyShape*)shape)->verts, cpvzero));
}

Vec2 PhysicsShapePolygon::getPoint(int i) const
{
    return PhysicsHelper::cpv2point(cpPolyShapeGetVert(_info->getShapes().front(), i));
}

void PhysicsShapePolygon::getPoints(Vec2* outPoints) const
{
    cpShape* shape = _info->getShapes().front();
    PhysicsHelper::cpvs2points(((cpPolyShape*)shape)->verts, outPoints, ((cpPolyShape*)shape)->numVerts);
}

int PhysicsShapePolygon::getPointsCount() const
{
    return ((cpPolyShape*)_info->getShapes().front())->numVerts;
}

Vec2 PhysicsShapePolygon::getCenter()
{
    return _center;
}

void PhysicsShapePolygon::rescale(const AffineTransform &transform)
{
    cpShape *shape = _info->getShapes().front();
    
    std::vector<cpVect> hullVerts(_points.size());
    for(int i=0; i<_points.size(); i++)
        hullVerts[i] =  cpTransformPoint(transform, _points[i]);
	
	unsigned int hullCount = cpConvexHull(static_cast<int>(_points.size()), &hullVerts.front(), &hullVerts.front(), NULL, 0.0);
    cpPolyShapeSetVerts(shape, hullCount, &hullVerts.front(), PhysicsHelper::point2cpv(_offset));
}

// PhysicsShapeEdgeBox
PhysicsShapeEdgeBox* PhysicsShapeEdgeBox::create(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/, const Vec2& offset/* = Vec2(0, 0)*/)
{
    PhysicsShapeEdgeBox* shape = new PhysicsShapeEdgeBox();
    if (shape && shape->init(size, material, border, offset))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgeBox::init(const Size& size, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/, const Vec2& offset/*= Vec2(0, 0)*/)
{
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGEBOX));
        
        cpVect vec[4] = {};
        vec[0] = PhysicsHelper::point2cpv(Vec2(-size.width/2+offset.x, -size.height/2+offset.y));
        vec[1] = PhysicsHelper::point2cpv(Vec2(+size.width/2+offset.x, -size.height/2+offset.y));
        vec[2] = PhysicsHelper::point2cpv(Vec2(+size.width/2+offset.x, +size.height/2+offset.y));
        vec[3] = PhysicsHelper::point2cpv(Vec2(-size.width/2+offset.x, +size.height/2+offset.y));
        
        int i = 0;
        for (; i < 4; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(_info->getSharedBody(), vec[i], vec[(i+1)%4],
                                               PhysicsHelper::float2cpfloat(border));
            CC_BREAK_IF(shape == nullptr);
            _info->add(shape);
        }
        CC_BREAK_IF(i < 4);
        
        _offset = offset;
        _mass = PHYSICS_INFINITY;
        _moment = PHYSICS_INFINITY;
        _border = border;
        _size = size;
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    return false;
}

void PhysicsShapeEdgeBox::getPoints(cocos2d::Vec2 *outPoints) const
{
    int i = 0;
    for(auto shape : _info->getShapes())
    {
        outPoints[i++] = PhysicsHelper::cpv2point(((cpSegmentShape*)shape)->a);
    }
}

void PhysicsShapeEdgeBox::rescale(const AffineTransform &transform)
{
    do
    {
        cpVect vec[4] = {};
        vec[0] = cpTransformPoint(transform, Point(-_size.width/2+_offset.x, -_size.height/2+_offset.y));
        vec[1] = cpTransformPoint(transform, Point(+_size.width/2+_offset.x, -_size.height/2+_offset.y));
        vec[2] = cpTransformPoint(transform, Point(+_size.width/2+_offset.x, +_size.height/2+_offset.y));
        vec[3] = cpTransformPoint(transform, Point(-_size.width/2+_offset.x, +_size.height/2+_offset.y));
        
        float xScale = ScaleXTransform(transform);
        float yScale = ScaleYTransform(transform);
        float border[4];
        border[0] = _border * xScale;
        border[1] = _border * yScale;
        border[2] = _border * xScale;
        border[3] = _border * yScale;
        
        std::vector<cpShape*>& shapes = _info->getShapes();
        
        for(int i=0;i<shapes.size();++i)
        {
            cpSegmentShapeSetEndpoints(shapes[i], vec[i], vec[(i+1)%4]);
            cpSegmentShapeSetRadius(shapes[i], border[i]);
        }
    } while (false);
}

// PhysicsShapeEdgeBox
PhysicsShapeEdgePolygon* PhysicsShapeEdgePolygon::create(const Vec2* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    PhysicsShapeEdgePolygon* shape = new PhysicsShapeEdgePolygon();
    if (shape && shape->init(points, count, material, border))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgePolygon::init(const Vec2* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    cpVect* vec = nullptr;
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGEPOLYGEN));
        
        vec = new cpVect[count];
        PhysicsHelper::points2cpvs(points, vec, count);
        _center = PhysicsHelper::cpv2point(cpCentroidForPoly(count, vec));
        
        int i = 0;
        for (; i < count; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(_info->getSharedBody(), vec[i], vec[(i+1)%count],
                                               PhysicsHelper::float2cpfloat(border));
            CC_BREAK_IF(shape == nullptr);
			cpShapeSetElasticity(shape, 1.0f);
			cpShapeSetFriction(shape, 1.0f);
            _info->add(shape);
        }
        CC_SAFE_DELETE_ARRAY(vec);
        
        CC_BREAK_IF(i < count);
        
        _mass = PHYSICS_INFINITY;
        _moment = PHYSICS_INFINITY;
        _border = border;
        _points.assign(points, points+count);
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    CC_SAFE_DELETE_ARRAY(vec);
    
    return false;
}

Vec2 PhysicsShapeEdgePolygon::getCenter()
{
    return _center;
}

void PhysicsShapeEdgePolygon::getPoints(cocos2d::Vec2 *outPoints) const
{
    int i = 0;
    for(auto shape : _info->getShapes())
    {
        outPoints[i++] = PhysicsHelper::cpv2point(((cpSegmentShape*)shape)->a);
    }
}

int PhysicsShapeEdgePolygon::getPointsCount() const
{
    return static_cast<int>(_info->getShapes().size() + 1);
}

void PhysicsShapeEdgePolygon::rescale(const AffineTransform &transform)
{
    cpVect* vec = nullptr;
    do
    {
        int count = static_cast<int>(_points.size());
        vec = new cpVect[_points.size()];
        for (int i = 0; i < count; ++i)
        {
            vec[i] = cpTransformPoint(transform, _points[i]);
        }
        
        _info->removeAll();
        int i = 0;
        for (; i < count; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(_info->getSharedBody(), vec[i], vec[(i+1)%count],
                                               PhysicsHelper::float2cpfloat(_border*RadiusForTransform(transform)));
            CC_BREAK_IF(shape == nullptr);
            cpShapeSetElasticity(shape, 1.0f);
            cpShapeSetFriction(shape, 1.0f);
            _info->add(shape);
        }
        CC_SAFE_DELETE_ARRAY(vec);
        
        CC_BREAK_IF(i < count);
    } while (false);
}

// PhysicsShapeEdgeChain
PhysicsShapeEdgeChain* PhysicsShapeEdgeChain::create(const Vec2* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    PhysicsShapeEdgeChain* shape = new PhysicsShapeEdgeChain();
    if (shape && shape->init(points, count, material, border))
    {
        shape->autorelease();
        return shape;
    }
    
    CC_SAFE_DELETE(shape);
    return nullptr;
}

bool PhysicsShapeEdgeChain::init(const Vec2* points, int count, const PhysicsMaterial& material/* = MaterialDefault*/, float border/* = 1*/)
{
    cpVect* vec = nullptr;
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGECHAIN));
        
        vec = new cpVect[count];
        PhysicsHelper::points2cpvs(points, vec, count);
        _center = PhysicsHelper::cpv2point(cpCentroidForPoly(count, vec));
        
        int i = 0;
        for (; i < count - 1; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(_info->getSharedBody(), vec[i], vec[i+1],
                                               PhysicsHelper::float2cpfloat(border));
            CC_BREAK_IF(shape == nullptr);
			cpShapeSetElasticity(shape, 1.0f);
			cpShapeSetFriction(shape, 1.0f);
            _info->add(shape);
        }
        CC_SAFE_DELETE_ARRAY(vec);
        CC_BREAK_IF(i < count - 1);
        
        _mass = PHYSICS_INFINITY;
        _moment = PHYSICS_INFINITY;
        _border = border;
        _points.assign(points, points+count);
        
        setMaterial(material);
        
        return true;
    } while (false);
    
    CC_SAFE_DELETE_ARRAY(vec);
    
    return false;
}

Vec2 PhysicsShapeEdgeChain::getCenter()
{
    return _center;
}

void PhysicsShapeEdgeChain::getPoints(Vec2* outPoints) const
{
    int i = 0;
    for(auto shape : _info->getShapes())
    {
        outPoints[i++] = PhysicsHelper::cpv2point(((cpSegmentShape*)shape)->a);
    }
    
    outPoints[i++] = PhysicsHelper::cpv2point(((cpSegmentShape*)_info->getShapes().back())->a);
}

int PhysicsShapeEdgeChain::getPointsCount() const
{
    return static_cast<int>(_info->getShapes().size() + 1);
}

void PhysicsShapeEdgeChain::rescale(const AffineTransform &transform)
{
    cpVect* vec = nullptr;
    int count = static_cast<int>(_points.size());
    do
    {
        CC_BREAK_IF(!PhysicsShape::init(Type::EDGECHAIN));
        
        vec = new cpVect[count];
        for (int i = 0; i < count; ++i)
        {
            vec[i] = cpTransformPoint(transform, _points[i]);
        }
        
        int i = 0;
        for (; i < count - 1; ++i)
        {
            cpShape* shape = cpSegmentShapeNew(_info->getSharedBody(), vec[i], vec[i+1],
                                               PhysicsHelper::float2cpfloat(_border*RadiusForTransform(transform)));
            CC_BREAK_IF(shape == nullptr);
			cpShapeSetElasticity(shape, 1.0f);
			cpShapeSetFriction(shape, 1.0f);
            _info->add(shape);
        }
        CC_SAFE_DELETE_ARRAY(vec);
        CC_BREAK_IF(i < count - 1);
    } while (false);
}

void PhysicsShape::setGroup(int group)
{
    if (group < 0)
    {
        for (auto shape : _info->getShapes())
        {
            cpShapeSetGroup(shape, (cpGroup)group);
        }
    }
    
    _group = group;
}

bool PhysicsShape::containsPoint(const Vec2& point) const
{
    for (auto shape : _info->getShapes())
    {
        if (cpShapePointQuery(shape, PhysicsHelper::point2cpv(point)))
        {
            return true;
        }
    }
    
    return false;
}

NS_CC_END

#endif // CC_USE_PHYSICS
