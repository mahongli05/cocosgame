//
// Created by MHL on 2017/3/17.
//

#ifndef PROJ_ANDROID_STUDIO_FLOATINGLAND_H
#define PROJ_ANDROID_STUDIO_FLOATINGLAND_H

#include <Box2D/Collision/b2Distance.h>
#include "cocos2d.h"

using namespace cocos2d;

enum DIRECTION {
    NONE = 0,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

static float FLOATING_DENSITY = 1000000000.0f;

class FloatingLand : public Sprite {

public:
    static FloatingLand* createFloatingLand(std::string name);

    FloatingLand();
    ~FloatingLand();


    int getGroup();
    void setGroup(int group);

    virtual void active();
    virtual void onFrameUpdate(float dt);
    virtual void onHeroContact(Node* hero) = 0;

protected:
    virtual bool init();

    bool mActive;
    int mGroup;
};

class FloatingMovingLand : public FloatingLand {
public:
    CREATE_FUNC(FloatingMovingLand);

    void setDistance(float distance);
    void setDirection(int direction);
    void setSpeed(float speed);

    FloatingMovingLand();

    virtual void active();
    virtual void onFrameUpdate(float dt);
    virtual void onHeroContact(Node* hero);

protected:
    virtual bool init();

private:
    float mMoveDistance;
    int mMoveDirection;
    float mMoveSpeed;
    float mOriginalPosition;
};

class FloatingDropLand : public FloatingLand {
public:
    CREATE_FUNC(FloatingDropLand);

    void setDuration(float duration);
    void setDropEnable(bool enable);

    FloatingDropLand();
    virtual void onFrameUpdate(float dt);
    virtual void onHeroContact(Node* hero);

protected:
    virtual bool init();

private:
    float mFloatingDuration;
    bool mEnableDrop;
};


#endif //PROJ_ANDROID_STUDIO_FLOATINGLAND_H
