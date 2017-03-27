//
// Created by MHL on 2017/3/17.
//

#include "FloatingLand.h"
#include "PhysicsConfig.h"
#include "NodeUtil.h"

bool FloatingLand::init() {
    return Sprite::init();
}

FloatingLand::FloatingLand() {
    mActive = false;
}

FloatingLand::~FloatingLand() {

}

void FloatingLand::active() {
    mActive = true;
}

int FloatingLand::getGroup() {
    return mGroup;
}

void FloatingLand::setGroup(int group) {
    mGroup = group;
}

FloatingLand *FloatingLand::createFloatingLand(std::string name) {
    if (name == "floatingMove") {
        return FloatingMovingLand::create();
    } else if (name == "floatingDrop") {
        return FloatingDropLand::create();
    }
    return nullptr;
}

void FloatingLand::onFrameUpdate(float dt) {

}

bool FloatingMovingLand::init() {
    if (!FloatingLand::init()) {
        return false;
    }
    //TODO: create SpriteFrame
    setSpriteFrame(SpriteFrame::create("brokencoin.png", Rect(0, 0, 16, 16)));
    setScaleX(3);
    PhysicsBody* body = PhysicsBody::createBox(getContentSize(), PhysicsMaterial(FLOATING_DENSITY, 0.0f, 1.0f));
    body->setDynamic(false);
    body->setGravityEnable(false);
    body->setContactTestBitmask(LAND_CONTACT_TEST_MASK);
    body->setCategoryBitmask(LAND_CATEGORY_MASK);
    body->setCollisionBitmask(LAND_COLLECTION_MASK);
    setPhysicsBody(body);
    setTag(FLOAT_LAND_MOVE);
    return true;
}

void FloatingMovingLand::onHeroContact(Node* hero) {

}

void FloatingMovingLand::setDistance(float distance) {
    mMoveDistance = distance;
}

void FloatingMovingLand::setDirection(int direction) {
    mMoveDirection = direction;
}

void FloatingMovingLand::setSpeed(float speed) {
    mMoveSpeed = speed / 10;
}

void FloatingMovingLand::active() {
    FloatingLand::active();
    PhysicsBody* body = getPhysicsBody();
    body->setDynamic(true);
    switch (mMoveDirection) {
        case LEFT:
            mOriginalPosition = getPosition().x;
            body->setVelocity(Point(-mMoveSpeed, 0));
            break;

        case RIGHT:
            mOriginalPosition = getPosition().x;
            body->setVelocity(Point(mMoveSpeed, 0));
            break;

        case UP:
            mOriginalPosition = getPosition().y;
            body->setVelocity(Point(0, mMoveSpeed));
            break;

        case DOWN:
            mOriginalPosition = getPosition().y;
            body->setVelocity(Point(0, -mMoveSpeed));
            break;

        default:
            break;
    }
}

void FloatingMovingLand::onFrameUpdate(float dt) {
    if (mActive) {
        float currentPos = 0;
        float distance = 0;
        PhysicsBody* body = getPhysicsBody();
        switch (mMoveDirection) {
            case UP:
                currentPos = getPosition().y;
                distance = currentPos - mOriginalPosition;
                if (distance > mMoveDistance) {
                    mMoveDirection = DOWN;
                    body->setVelocity(Point(0, -mMoveSpeed));
                }
                break;

            case DOWN:
                currentPos = getPosition().y;
                distance = currentPos - mOriginalPosition;
                if (distance < -mMoveDistance) {
                    mMoveDirection = UP;
                    body->setVelocity(Point(0, mMoveSpeed));
                }
                break;

            case RIGHT:
                currentPos = getPosition().x;
                distance = currentPos - mOriginalPosition;
                if (distance > mMoveDistance) {
                    mMoveDirection = LEFT;
                    body->setVelocity(Point(-mMoveSpeed, 0));
                }
                break;

            case LEFT:
                currentPos = getPosition().x;
                distance = currentPos - mOriginalPosition;
                if (distance < -mMoveDistance) {
                    mMoveDirection = RIGHT;
                    body->setVelocity(Point(mMoveSpeed, 0));
                }
                break;

            case NONE:
                break;
        }
    }
}

FloatingMovingLand::FloatingMovingLand() {
    mMoveDirection = NONE;
    mMoveDistance = 0;
    mMoveSpeed = 0;
}

bool FloatingDropLand::init() {
    if (!FloatingLand::init()) {
        return false;
    }
    //TODO: create SpriteFrame
    setSpriteFrame(SpriteFrame::create("brokencoin.png", Rect(16, 16, 16, 16)));
    setScaleX(3);
    PhysicsBody* body = PhysicsBody::createBox(getContentSize(), PhysicsMaterial(FLOATING_DENSITY, 0.0f, 1.0f));
    body->setDynamic(false);
    body->setGravityEnable(false);
    body->setContactTestBitmask(LAND_CONTACT_TEST_MASK);
    body->setCategoryBitmask(LAND_CATEGORY_MASK);
    body->setCollisionBitmask(LAND_COLLECTION_MASK);
    setPhysicsBody(body);
    setTag(FLOAT_LAND_DROP);
    return true;
}

void FloatingDropLand::onFrameUpdate(float dt) {
    if (mActive && mEnableDrop) {
        if (mFloatingDuration > FLT_EPSILON) {
            mFloatingDuration -= dt;
        }
        if (mFloatingDuration < 0.0f) {
            PhysicsBody* body = getPhysicsBody();
            if (body != nullptr) {
                body->setDynamic(true);
                body->setGravityEnable(true);
                body->setCategoryBitmask(DROP_CATEGORY_MASK);
            }
        }
    }
}

void FloatingDropLand::onHeroContact(Node* hero) {

//    if (hero != nullptr) {
//        Point anchor = hero->getAnchorPoint();
//        float left = hero->getPosition().x - hero->getContentSize().width * anchor.x;
//        float right = hero->getPosition().x + hero->getContentSize().width * (1 - anchor.x);
//        float bottom = hero->getPosition().y - hero->getContentSize().height * anchor.y;
//
//        Rect rect = NodeUtil::getRectToParent(this);
//        if (left < rect.getMaxX() && right > rect.getMinX()) {
//            mEnableDrop = true;
//        }
//    }
}

void FloatingDropLand::setDuration(float duration) {
    mFloatingDuration = duration;
}

FloatingDropLand::FloatingDropLand() {
    mFloatingDuration = 3.0f;
    mEnableDrop = false;
}

void FloatingDropLand::setDropEnable(bool enable) {
    mEnableDrop = enable;
}












