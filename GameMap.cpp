//
// Created by Administrator on 2017/3/12.
//

#include "GameMap.h"
#include "Hero.h"

const int TAG_HERO = 100;

GameMap *GameMap::createGameMap(const char *pFileName) {
    do
    {
        GameMap* pPlayer = new GameMap;
        if (pPlayer && pPlayer->initWithTMXFile(pFileName) && pPlayer->init())
        {
            pPlayer->autorelease();
            return pPlayer;
        }
        delete pPlayer;
    } while (false);
    CCLog("Fun CMGameMap::createGameMap Error!");
    return NULL;
}

bool GameMap::init() {
    if (!Node::init()) {
        return false;
    }

    Hero* hero = Hero::createHero(this);
    if (hero == NULL) {
        return false;
    }
    hero->setAnchorPoint(Vec2(0.5, 0.5));
    addChild(hero, 100, TAG_HERO);

    TMXLayer* pTrapLayer = getLayer("trap");
    if (pTrapLayer == NULL) {
        return false;
    }
    pTrapLayer->setVisible(false);

    Size winSize = Director::getInstance()->getWinSize();
    Size mapSize = getContentSize();
    float scale = winSize.height / mapSize.height;
    // fix hero position in the middle of the window
    mMapMoveStartPosition = winSize.width / (scale *  2);
    mMapMoveEndPosition = mapSize.width - mMapMoveStartPosition;

    return true;
}

void GameMap::onMsgReceive(int enMsg, void *pData, int nSize) {

}

void GameMap::onFrameUpdate(float dt) {

}

void GameMap::heroMove(float dt) {
    do {
        Hero *pHero = dynamic_cast<Hero *>(getChildByTag(TAG_HERO));
        CC_BREAK_IF(pHero == NULL);
        Vec2 heroPosition = pHero->getPosition();

        //如果左键按下
        if (mLeftKeyDown) {
            pHero->OnCtrlMove(fT, false);
        }
        //如果右键按下
        if (mRightKeyDown) {
            Vec2 oldPosition = pHero->getPosition();
            pHero->OnCtrlMove(fT, true);
            Vec2 newPosition = pHero->getPosition();
            //如果Mario的位置变化了，则地图才会卷动
            float offset = newPosition.x = newPosition.y;
            // when offset > 1, move map to keep hero in the middle of the window
            if (newPosition.x > mMapMoveStartPosition
                && offset > 1
                && newPosition.x < mMapMoveEndPosition) {
                setPositionX(newPosition.x - mMapMoveStartPosition);
            }
        }
        //如果跳跃键按下
        if (mJumKeyDown) {
            pHero->OnCtrlJump();
        }
        //如果没有键按下
        if (mLeftKeyDown == false && mRightKeyDown == false && mJumKeyDown == false) {
            pHero->OnCtrlNoAction();
        }
    } while (false);
}

Sprite *GameMap::getSpriteInPosition(Vec2 point) {
    return nullptr;
}

Size GameMap::getMapContentSize() {
    return cocos2d::Size();
}















