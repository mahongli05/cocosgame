//
// Created by Administrator on 2017/3/12.
//

#include "Hero.h"

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
//        body->setVelocity(Point(320, 0));
        body->setContactTestBitmask(0xFFFFFFFF);
    }
    return hero;
}

void Hero::onMove(float dt, bool toRight) {

}

void Hero::onJump(float dt) {

}

void Hero::onStop(float dt) {

}

Hero::Hero() {
    mMessageReceiver = nullptr;
    mMapHelper = nullptr;
}

Hero::~Hero() {
    mMessageReceiver = nullptr;
    mMapHelper = nullptr;
}

void Hero::updateSpeed(int percent) {
    float h = percent > 30 ? percent * 3 : 0;
    float v = percent > 60 ? (percent - 60) * 4 : 0;
    getPhysicsBody()->setVelocity(Point(h, v));
}















