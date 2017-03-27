//
// Created by Administrator on 2017/3/12.
//

#include "GameContainer.h"

const int TAG_GAME_MAP = 99;

GameContainer *GameContainer::createContainer(int level) {

    GameContainer * container = GameContainer::create();
    container->loadMap(level);
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

GameContainer::GameContainer() {
    mGameMap = NULL;
    mCurrentLevel = 0;
}

GameContainer::~GameContainer() {
    mGameMap = NULL;
}

GameMap *GameContainer::getGameMap() {
    return mGameMap;
}

bool GameContainer::loadMap(int level) {
    if (mGameMap != NULL) {
        removeChild(mGameMap);
    }
    GameMap * gameMap = GameMap::createGameMap("game5.tmx");
    if (gameMap != nullptr) {
        gameMap->setAnchorPoint(Vec2(0, 0));
        gameMap->setGameEventCallback(mGameEventCallback);
        addChild(gameMap, 0, TAG_GAME_MAP);
        setPosition(Point(0, 0));
        mGameMap = gameMap;
        mCurrentLevel = level;
        return true;
    }
    return false;
}

int GameContainer::getCurrentLevel() {
    return mCurrentLevel;
}

void GameContainer::setGameEventCallback(GameEventCallback *callback) {
    mGameEventCallback = callback;
    if (mGameMap != nullptr) {
        mGameMap->setGameEventCallback(callback);
    }
}























