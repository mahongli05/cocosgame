//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_HERO_H
#define PROJ_ANDROID_STUDIO_HERO_H

#include "cocos2d.h"
#include "MessageSender.h"
#include "MessageReceiver.h"
#include "MapHelper.h"

using namespace cocos2d;

enum State {
    STATE_NONE,
    STATE_NORMAL,
    STATE_RUN_RIGHT,
    STATE_JUMP,
    STATE_DIE
};

class Hero : public Sprite, public MessageSender {

private:
    MessageReceiver* mMessageReceiver;
    MapHelper* mMapHelper;

    SpriteFrame * mJumpFrame;
    SpriteFrame * mNormalFrame;

    Action* mMoveRightAction;
    Action* mDieAction;

    State mState;

public:

    Hero();
    ~Hero();

    static Hero* createHero(MessageReceiver *receiver, MapHelper *mapHelper);

    CREATE_FUNC(Hero);

    virtual bool init();

    void updateSpeed(int percent);
    void updateState(State state);
    State getState();
};


#endif //PROJ_ANDROID_STUDIO_HERO_H
