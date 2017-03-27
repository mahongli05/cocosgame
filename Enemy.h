//
// Created by MHL on 2017/3/14.
//

#ifndef PROJ_ANDROID_STUDIO_ENEMY_H
#define PROJ_ANDROID_STUDIO_ENEMY_H

#include "cocos2d.h"

using namespace cocos2d;

enum  EnemyState {
    ENEMY_INACTIVE,
    ENEMY_ACTIVE,
    ENEMY_DIE
};

class Enemy : public Sprite {

public:
    Enemy();
    ~Enemy();

    static Enemy* createEnemy(std::string &name);

    CREATE_FUNC(Enemy);

    void updateState(EnemyState state);
    virtual void active();
    virtual void die();
    virtual void inactive();

protected:
    virtual bool init();

private:
    SpriteFrame* mDieFrame;
    Action* mNormalAction;
    EnemyState mState;
};


#endif //PROJ_ANDROID_STUDIO_ENEMY_H
