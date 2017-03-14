//
// Created by Administrator on 2017/3/12.
//

#include "Hero.h"
#include "PhysicsConfig.h"

bool Hero::init() {
    return Sprite::init();
}

Hero *Hero::createHero(MessageReceiver *receiver, MapHelper *mapHelper) {
    Hero* hero = Hero::create();
    if (hero != nullptr) {
        TextureCache* textureCache =  Director::getInstance()->getTextureCache();
        Texture2D* texture2D = textureCache->addImage("walkRight.png");
        SpriteFrame* spriteFrame = SpriteFrame::createWithTexture(texture2D, Rect(0, 0, 18, 32));
        hero->setSpriteFrame(spriteFrame);
        hero->setReceiver(receiver);
        hero->mMapHelper = mapHelper;

        PhysicsBody* body = PhysicsBody::createBox(hero->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 0.0f));
        hero->setPhysicsBody(body);
        body->setDynamic(true);
        body->setRotationEnable(false);
        body->setGravityEnable(true);
        body->setContactTestBitmask(HERO_CONTACT_TEST_MASK);
        body->setCategoryBitmask(HERO_CATEGORY_MASK);
        body->setCollisionBitmask(HERO_COLLECTION_MASK);

        Vector<SpriteFrame*> moveRightVector;
        for (int i = 0; i < 9; i++) {
            moveRightVector.pushBack(SpriteFrame::create("walkRight.png", Rect(18*i, 0, 18, 32)));
        }
        Animation * moveAnimation = Animation::createWithSpriteFrames(moveRightVector, 0.1f);
        Animate * moveAnimate = Animate::create(moveAnimation);
        hero->mMoveRightAction = RepeatForever::create(moveAnimate);
        hero->mMoveRightAction->retain();

        Vector<SpriteFrame*> dieVector;
        for (int i = 0; i < 6; i++) {
            dieVector.pushBack(SpriteFrame::create("normal_die.png", Rect(24*i, 0, 24, 34)));
        }
        Animation * dieAnimation = Animation::createWithSpriteFrames(dieVector, 0.1f);
        Animate * dieAnimate = Animate::create(dieAnimation);
        hero->mDieAction = dieAnimate;
        hero->mDieAction->retain();

        hero->mNormalFrame = SpriteFrame::create("walkRight.png", Rect(0, 0, 18, 32));
        hero->mJumpFrame = SpriteFrame::create("walkRight.png", Rect(180, 0, 18, 32));
        hero->mNormalFrame->retain();
        hero->mJumpFrame->retain();

        hero->updateState(State::STATE_NORMAL);
    }
    return hero;
}

Hero::Hero() {
    mMessageReceiver = nullptr;
    mMapHelper = nullptr;
    mState = State::STATE_NONE;
    mDieAction = nullptr;
    mMoveRightAction = nullptr;
    mJumpFrame = nullptr;
    mNormalFrame = nullptr;
}

Hero::~Hero() {
    mMessageReceiver = nullptr;
    mMapHelper = nullptr;
    mDieAction->release();
    mMoveRightAction->release();
    mJumpFrame->release();
    mNormalFrame->release();
}

void Hero::updateSpeed(int percent) {

    if (mState == STATE_DIE) {
        return;
    }

    float h = percent > 30 ? percent * 3 : 0;
    float v = percent > 60 ? (percent - 50) * 5 : 0;
    getPhysicsBody()->setVelocity(Point(h, v));
    log("HERO: updateSpeed: %d, %f, %f", percent, h, v);

    if (abs(v) > FLT_EPSILON) {
        updateState(STATE_JUMP);
    } else if (abs(h) > 0) {
        updateState(STATE_RUN_RIGHT);
    } else {
        updateState(STATE_NORMAL);
    }
}

void Hero::updateState(State state) {
    if (mState == state || mState == STATE_DIE) {
        return;
    }
    mState = state;
    stopAction(mMoveRightAction);
    stopAction(mDieAction);
    switch (mState) {
        case STATE_NORMAL:
            setSpriteFrame(mNormalFrame);
            break;

        case STATE_JUMP:
            setSpriteFrame(mJumpFrame);
            break;

        case STATE_RUN_RIGHT:
            runAction(mMoveRightAction);
            break;

        case STATE_DIE:
            runAction(mDieAction);
            getPhysicsBody()->setVelocity(Point(0, 250));
            getPhysicsBody()->setCategoryBitmask(DROP_CATEGORY_MASK);
            break;

        default:
            break;
    }
}

State Hero::getState() {
    return mState;
}



















