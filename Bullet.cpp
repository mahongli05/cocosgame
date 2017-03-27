//
// Created by MHL on 2017/3/15.
//

#include "Bullet.h"
#include "PhysicsConfig.h"

Bullet::Bullet() {

}

Bullet::~Bullet() {

}

Bullet *Bullet::createBullet(std::string name) {

    if (name == "fire") {
        return EnemyBullet::create();
    } else if (name == "ax") {
        return HeroBullet::create();
    }
    return nullptr;
}

void Bullet::active() {

}

bool Bullet::init() {
    return Sprite::init();
}

bool HeroBullet::init() {
    if (!Bullet::init()) {
        return false;
    }

    return true;
}

void HeroBullet::active() {
    Bullet::active();
}

bool EnemyBullet::init() {

    if (!Bullet::init()) {
        return false;
    }

    Vector<SpriteFrame*> bulletVector;
    for (int i = 0; i < 11; i++) {
        bulletVector.pushBack(SpriteFrame::create("fireRight.png", Rect(11*i, 0, 11, 16)));
    }
    Animation * animation = Animation::createWithSpriteFrames(bulletVector, 0.1f);
    Animate * animate = Animate::create(animation);
    runAction(RepeatForever::create(animate));

    PhysicsBody* body = PhysicsBody::createBox(getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    body->setContactTestBitmask(BULLET_E_CONTACT_TEST_MASK);
    body->setCategoryBitmask(BULLET_E_CATEGORY_MASK);
    body->setCollisionBitmask(BULLET_E_COLLECTION_MASK);
    body->setGravityEnable(true);
    // update to true when visible;
    body->setDynamic(false);
    body->setRotationEnable(false);
    setPhysicsBody(body);
    return true;
}

void EnemyBullet::active() {
    Bullet::active();
    PhysicsBody* body = getPhysicsBody();
    if (body != nullptr) {
        body->setDynamic(true);
        body->setVelocity(Point(-200, 0));
    }
}



