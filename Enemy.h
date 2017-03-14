//
// Created by MHL on 2017/3/14.
//

#ifndef PROJ_ANDROID_STUDIO_ENEMY_H
#define PROJ_ANDROID_STUDIO_ENEMY_H

#include "cocos2d.h"

using namespace cocos2d;

class Enemy : public Sprite {

public:
    Enemy();
    ~Enemy();

    static Enemy* createEnemy(std::string &name);

    CREATE_FUNC(Enemy);

    void die();

protected:
    virtual bool init();

private:
    SpriteFrame* mDieFrame;
    Action* mNormalAction;
};


#endif //PROJ_ANDROID_STUDIO_ENEMY_H
