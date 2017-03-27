//
// Created by MHL on 2017/3/14.
//

#include "Enemy.h"
#include "PhysicsConfig.h"

Enemy::Enemy() {
    mDieFrame = nullptr;
    mNormalAction = nullptr;
    mState = ENEMY_INACTIVE;
}

Enemy::~Enemy() {
    if (mDieFrame != nullptr) {
        mDieFrame->release();
    }
    mDieFrame = nullptr;
    if (mNormalAction != nullptr) {
        mNormalAction->release();
    }
    mNormalAction = nullptr;
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
    body->setRotationEnable(false);
    body->setVelocity(Point(-200, 0));
    body->setDynamic(false);
    enemy->setPhysicsBody(body);

    return enemy;
}

void Enemy::updateState(EnemyState state) {
    if (state == mState || mState == ENEMY_DIE) {
        return;
    }
    mState = state;
    switch (state) {
        case ENEMY_ACTIVE:
            active();
            break;

        case ENEMY_DIE:
            die();
            break;

        case ENEMY_INACTIVE:
            inactive();
            break;

        default:
            break;
    }
}

void Enemy::die() {
    stopAction(mNormalAction);
    setSpriteFrame(mDieFrame);
    PhysicsBody * body = getPhysicsBody();
    if (body != nullptr) {
        getPhysicsBody()->setVelocity(Point(0, 50));
        getPhysicsBody()->setCategoryBitmask(DROP_CATEGORY_MASK);
    }
    // delay remove ?
//    getParent()->removeChild(this);
}

void Enemy::active() {
    PhysicsBody* body = getPhysicsBody();
    bool isDynamic = body->isDynamic();
    if (!isDynamic ) {
        body->setDynamic(true);
        body->setVelocity(Point(-200, 0));
    }
}

void Enemy::inactive() {
    PhysicsBody* body = getPhysicsBody();
    if (body != nullptr) {
        body->setDynamic(false);
    }
}





















