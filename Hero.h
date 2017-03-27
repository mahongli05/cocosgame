//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_HERO_H
#define PROJ_ANDROID_STUDIO_HERO_H

#include "cocos2d.h"
#include "MapHelper.h"

using namespace cocos2d;

//enum State {
//    STATE_NONE,
//    STATE_NORMAL,
//    STATE_RUN_RIGHT,
//    STATE_JUMP,
//    STATE_DIE
//};

class Hero : public Sprite {

private:
    MapHelper* mMapHelper;

    SpriteFrame * mJumpFrame;
    SpriteFrame * mNormalFrame;
    SpriteFrame * mCrazyFrame;
    SpriteFrame * mCrazyJumpFrame;

    Action* mMoveRightAction;
    Action* mCrazyMoveRightAction;
    Action* mDieAction;

    int mStateFlag;
    float mCrazyDuration;

public:

    Hero();
    ~Hero();

    static Hero* createHero(MapHelper *mapHelper);

    CREATE_FUNC(Hero);

    virtual bool init();

    void updateSpeed(float xV, float yV);
    Point getSpeed();
    void setStateFlag(int flag);
    int getStateFlag();
    void setCrazy(float duration); // s
    void onFrameUpdate(float dt);
    bool isLive();
    bool isCrazy();
};


#endif //PROJ_ANDROID_STUDIO_HERO_H
