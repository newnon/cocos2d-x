#ifndef _CCB_CCPARTICLESYSTEMQUADLOADER_H_
#define _CCB_CCPARTICLESYSTEMQUADLOADER_H_

#include "CCBXNodeLoader.h"

NS_CC_BEGIN
namespace spritebuilder {


class CC_DLL ParticleSystemQuadLoader : public NodeLoader {
public:
    
    static ParticleSystemQuadLoader *create();
    virtual Node *createNodeInstance(const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, CCBXReaderOwner *rootOwner, const ValueMap &customProperties, const NodeParams& params) const override;
    virtual void setSpecialProperties(Node* node, const Size &parentSize, float mainScale, float additionalScale, CCBXReaderOwner *owner, Node *rootNode, const cocos2d::ValueMap &customProperties, const NodeParams& params) const override;
    
CC_CONSTRUCTOR_ACCESS:
    ParticleSystemQuadLoader();
    ~ParticleSystemQuadLoader();

protected:
    virtual void onHandlePropTypeIntegerLabeled(const std::string &propertyName, bool isExtraProp, int value) override;
    virtual void onHandlePropTypePoint(const std::string &propertyName, bool isExtraProp, const Point &value) override;
    virtual void onHandlePropTypeFloat(const std::string &propertyName, bool isExtraProp, float value) override;
    virtual void onHandlePropTypeInteger(const std::string &propertyName, bool isExtraProp, int value) override;
    virtual void onHandlePropTypeFloatVar(const std::string &propertyName, bool isExtraProp, const Vec2 &value) override;
    virtual void onHandlePropTypeColor4FVar(const std::string &propertyName, bool isExtraProp, const std::pair<Color4F, Color4F> &value) override;
    virtual void onHandlePropTypeBlendFunc(const std::string &propertyName, bool isExtraProp, const BlendFunc &pBlendFunc) override;
    virtual void onHandlePropTypeTexture(const std::string &propertyName, bool isExtraProp, const TextureDescription &texture) override;
    virtual void onHandlePropTypeCheck(const std::string &propertyName, bool isExtraProp, bool value) override;
    
private:
    int _emitterMode;
    int _positionType;
    Vec2 _posVar;
    Vec2 _gravity;
    float _emissionRate;
    float _duration;
    int _totalParticles;
    bool _resetOnVisible;
    Vec2 _life;
    Vec2 _startSize;
    Vec2 _endSize;
    Vec2 _startSpin;
    Vec2 _endSpin;
    Vec2 _angle;
    Vec2 _speed;
    Vec2 _tangentialAccel;
    Vec2 _radialAccel;
    Vec2 _startRadius;
    Vec2 _endRadius;
    Vec2 _rotatePerSecond;
    std::pair<Color4F, Color4F> _startColor;
    std::pair<Color4F, Color4F> _endColor;
    TextureDescription _texture;
    BlendFunc _blendFunc;
};

}

NS_CC_END

#endif
