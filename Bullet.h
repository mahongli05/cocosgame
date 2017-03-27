//
// Created by MHL on 2017/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_BULLET_H
#define PROJ_ANDROID_STUDIO_BULLET_H

#include "cocos2d.h"

using namespace cocos2d;

class Bullet : public Sprite {

public:
    Bullet();
    ~Bullet();
    static Bullet* createBullet(std::string name);
    virtual void active();
protected:
    virtual bool init();
private:
};

class HeroBullet : public Bullet {
public:
    CREATE_FUNC(HeroBullet);
    virtual void active();
protected:
    virtual bool init();

};

class EnemyBullet : public Bullet {
public:
    CREATE_FUNC(EnemyBullet);
    virtual void active();
protected:
    virtual bool init();
};


#endif //PROJ_ANDROID_STUDIO_BULLET_H
