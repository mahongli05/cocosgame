//
// Created by Administrator on 2017/3/12.
//

#include "GameContainer.h"

const int TAG_GAME_MAP = 99;

GameContainer *GameContainer::createContainer(int level) {

    GameContainer * container = GameContainer::create();
    GameMap * gameMap = GameMap::createGameMap("game4.tmx");
    gameMap->setAnchorPoint(Vec2(0, 0));
    container->addChild(gameMap, 0, TAG_GAME_MAP);
    container->mGameMap = gameMap;
    return container;
}

bool GameContainer::init() {
    return Layer::init();
}

Size GameContainer::getMapContentSize() {
    do {
        GameMap* pMap = dynamic_cast<GameMap*>(getChildByTag(TAG_GAME_MAP));
        CC_BREAK_IF(pMap==NULL);
        return pMap->getContentSize();
    } while (false);
}

void GameContainer::startGame() {
    this->schedule(schedule_selector(GameContainer::onFrameUpdate));
}

void GameContainer::onFrameUpdate(float dt) {
    do
    {
        GameMap* pMap = dynamic_cast<GameMap*>(getChildByTag(TAG_GAME_MAP));
        CC_BREAK_IF(pMap==NULL);
        pMap->onFrameUpdate(dt);

        //CCLog("TileType = %d",pMap->HeroPosToTileType(pHero->getPosition()));
        //CCLog("HeroPosX=%f    HeroPosY=%f",pHero->getPositionX(),pHero->getPositionY());
        return;
    } while (false);
//    CCLog("fun GameContainer::Update Error!");
}

void GameContainer::updateHeroSpeed(int percent) {
    if (mGameMap != NULL) {
        mGameMap->updatePercent(percent);
    }
}











