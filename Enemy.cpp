//
// Created by MHL on 2017/3/14.
//

#include "Enemy.h"
#include "PhysicsConfig.h"

Enemy::Enemy() {
    mDieFrame = nullptr;
    mNormalAction = nullptr;
}

Enemy::~Enemy() {
    mDieFrame->release();
    mNormalAction->release();
}

bool Enemy::init() {
    return Sprite::init();
}

Enemy *Enemy::createEnemy(std::string &name) {

    Enemy* enemy = create();
    Vector<SpriteFrame*> enemyVector;
    for (int i = 0; i < 2; i++) {
        enemyVector.pushBack(SpriteFrame::create("Mushroom0.png", Rect(16*i, 0, 16, 16)));
    }
    Animation * animation = Animation::createWithSpriteFrames(enemyVector, 0.1f);
    Animate * animate = Animate::create(animation);
    enemy->setSpriteFrame(SpriteFrame::create("Mushroom0.png", Rect(0, 0, 16, 16)));
    enemy->mNormalAction = RepeatForever::create(animate);
    enemy->mNormalAction->retain();
    enemy->runAction(enemy->mNormalAction);
    enemy->mDieFrame = SpriteFrame::create("Mushroom0.png", Rect(48, 0, 16, 16));
    enemy->mDieFrame->retain();

    PhysicsBody* body = PhysicsBody::createBox(enemy->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    body->setContactTestBitmask(ENEMY_CONTACT_TEST_MASK);
    body->setCategoryBitmask(ENEMY_CATEGORY_MASK);
    body->setCollisionBitmask(ENEMY_COLLECTION_MASK);
    body->setGravityEnable(true);
    // update to true when visible;
    body->setDynamic(false);
    body->setRotationEnable(false);
    body->setVelocity(Point(-200, 0));
    enemy->setPhysicsBody(body);

    return enemy;
}

void Enemy::die() {
    setSpriteFrame(mDieFrame);
    stopAction(mNormalAction);
    getPhysicsBody()->setVelocity(Point(0, 50));
    getPhysicsBody()->setCategoryBitmask(DROP_CATEGORY_MASK);
    // delay remove ?
//    getParent()->removeChild(this);
}











