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

class Hero : public Sprite, public MessageSender {

private:
    MessageReceiver* mMessageReceiver;
    MapHelper* mMapHelper;

public:

    Hero();
    ~Hero();

    static Hero* createHero(MessageReceiver *receiver, MapHelper *mapHelper);

    CREATE_FUNC(Hero);

    virtual bool init();

    void onMove(float dt, bool toRight);

    void onJump(float dt);

    void onStop(float dt);
};


#endif //PROJ_ANDROID_STUDIO_HERO_H
