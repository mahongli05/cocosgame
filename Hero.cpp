//
// Created by Administrator on 2017/3/12.
//

#include "Hero.h"
#include "PhysicsConfig.h"
#include "StateFlag.h"

bool Hero::init() {
    return Sprite::init();
}

Hero *Hero::createHero(MapHelper *mapHelper) {
    Hero* hero = Hero::create();
    if (hero != nullptr) {
        TextureCache* textureCache =  Director::getInstance()->getTextureCache();
        Texture2D* texture2D = textureCache->addImage("walkRight.png");
        SpriteFrame* spriteFrame = SpriteFrame::createWithTexture(texture2D, Rect(0, 0, 18, 32));
        hero->setSpriteFrame(spriteFrame);
        hero->mMapHelper = mapHelper;

        PhysicsBody* body = PhysicsBody::createBox(hero->getContentSize(), PhysicsMaterial(1.0f, 0.0f, 1.0f));
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

        Vector<SpriteFrame*> crazyMoveRightVector;
        for (int i = 0; i < 9; i++) {
            crazyMoveRightVector.pushBack(SpriteFrame::create("allow_walkRight.png", Rect(18*i, 0, 18, 32)));
        }
        Animation * crazyMoveAnimation = Animation::createWithSpriteFrames(crazyMoveRightVector, 0.1f);
        Animate * crazyMoveAnimate = Animate::create(crazyMoveAnimation);
        hero->mCrazyMoveRightAction = RepeatForever::create(crazyMoveAnimate);
        hero->mCrazyMoveRightAction->retain();

        hero->mCrazyFrame = SpriteFrame::create("allow_walkRight.png", Rect(0, 0, 18, 32));
        hero->mCrazyJumpFrame = SpriteFrame::create("allow_walkRight.png", Rect(180, 0, 18, 32));
        hero->mCrazyFrame->retain();
        hero->mCrazyJumpFrame->retain();
        hero->setStateFlag(STATE_LIVE_MASK | STATE_DIRECTION_MASK);
    }
    return hero;
}

Hero::Hero() {
    mMapHelper = nullptr;
    mDieAction = nullptr;
    mMoveRightAction = nullptr;
    mJumpFrame = nullptr;
    mNormalFrame = nullptr;
    mCrazyFrame = nullptr;
    mCrazyJumpFrame = nullptr;
    mCrazyMoveRightAction = nullptr;
    mCrazyDuration = 0;
    mStateFlag = 0;
}

Hero::~Hero() {
    mMapHelper = nullptr;
    mDieAction->release();
    mMoveRightAction->release();
    mJumpFrame->release();
    mNormalFrame->release();
    mCrazyFrame->release();
    mCrazyJumpFrame->release();
    mCrazyMoveRightAction->release();
}

Point Hero::getSpeed() {
    PhysicsBody* body = getPhysicsBody();
    if (body != nullptr) {
        return body->getVelocity();
    }
    return Point(0, 0);
}

void Hero::updateSpeed(float xV, float yV) {

    if (!isLive()) {
        return;
    }

    PhysicsBody* body = getPhysicsBody();
    if (body != nullptr) {
        body->setVelocity(Point(xV, yV));
    }

    int tempFlag = getStateFlag();
    if (abs(yV) > FLT_EPSILON) {
        tempFlag |= STATE_JUMP_MASK;
    }

    if (abs(xV) > FLT_EPSILON) {
        tempFlag |= STATE_RUN_MASK;
    }

    setStateFlag(tempFlag);
}

void Hero::setStateFlag(int flag) {

    if (mStateFlag == flag) {
        return;
    }

    // test live
    if (mStateFlag != 0 && !StateFlag::containFlag(mStateFlag, STATE_LIVE_MASK)) {
        return;
    }

    if (!StateFlag::containFlag(flag, STATE_LIVE_MASK)) {
        mStateFlag = flag;
        runAction(mDieAction);
        getPhysicsBody()->setVelocity(Point(0, 250));
        getPhysicsBody()->setCategoryBitmask(DROP_CATEGORY_MASK);
        return;
    }

    int oldCrazyJumpRunDirFlag = mStateFlag & CRAZY_JUMP_RUN_DIRECTION;
    int newCrazyJumpRunDirFlag = flag & CRAZY_JUMP_RUN_DIRECTION;
    if (oldCrazyJumpRunDirFlag == newCrazyJumpRunDirFlag) {
        mStateFlag = flag;
        return;
    }

    stopAction(mMoveRightAction);
    stopAction(mCrazyMoveRightAction);

    switch (newCrazyJumpRunDirFlag) {
        case CRAZY_JUMP:
        case CRAZY_JUMP_RIGHT:
        case CRAZY_JUMP_RUN_RIGHT:
            setSpriteFrame(mCrazyJumpFrame);
            break;

        case CRAZY_RUN_RIGHT:
            runAction(mCrazyMoveRightAction);
            break;

        case JUMP:
        case JUMP_RUN_RIGHT:
        case JUMP_RIGHT:
            setSpriteFrame(mJumpFrame);
            break;

        case RUN_RIGHT:
            runAction(mMoveRightAction);
            break;

        case CRAZY_RIGHT:
        case STATE_CRAZY_MASK:
            setSpriteFrame(mCrazyFrame);
            break;

        default:
            setSpriteFrame(mNormalFrame);
            break;
    }

    mStateFlag = flag;
}

int Hero::getStateFlag() {
    return mStateFlag;
}

void Hero::setCrazy(float duration) {
    mCrazyDuration = duration;
    if (duration > 0) {
        int flag = getStateFlag();
        setStateFlag(flag | STATE_CRAZY_MASK);
    }
}

void Hero::onFrameUpdate(float dt) {

    if (!isLive()) {
        return;
    }

    int removeFlag = 0;

    if (mCrazyDuration > 0.0f) {
        mCrazyDuration -= dt;
    }
    if (mCrazyDuration < 0.0f && isCrazy()) {
        removeFlag |= STATE_CRAZY_MASK;
    }

    PhysicsBody* body = getPhysicsBody();
    Point velocity = body->getVelocity();
    if (velocity.x < 0.0f) {
        body->setVelocity(Point(0.0f, velocity.y));
    }

    if (StateFlag::containFlag(mStateFlag, STATE_JUMP_MASK) && abs(velocity.y) < FLT_EPSILON) {
        removeFlag |= STATE_JUMP_MASK;
    }
    if (StateFlag::containFlag(mStateFlag, STATE_RUN_MASK) && abs(velocity.x) < FLT_EPSILON) {
        removeFlag |= STATE_RUN_MASK;
    }

    if (removeFlag != 0) {
        int flag = StateFlag::removeFlag(mStateFlag, removeFlag);
        setStateFlag(flag);
    }
}

bool Hero::isLive() {
    return StateFlag::containFlag(mStateFlag, STATE_LIVE_MASK);
}

bool Hero::isCrazy() {
    return StateFlag::containFlag(mStateFlag, STATE_CRAZY_MASK);
}

