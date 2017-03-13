//
// Created by Administrator on 2017/3/12.
//

#include "GameMap.h"

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
//    CCLog("Fun CMGameMap::createGameMap Error!");
    return NULL;
}

bool GameMap::init() {
    if (!Node::init()) {
        return false;
    }

    Hero* hero = Hero::createHero(this, this);
    if (hero == NULL) {
        return false;
    }
    hero->setAnchorPoint(Vec2(0.5, 0.5));
    hero->setPosition(Point(20, 100));
    addChild(hero, 100);
    mHero = hero;

    // add trap
    TMXLayer* pTrapLayer = getLayer("trap");
    if (pTrapLayer == NULL) {
        return false;
    }

    Size mapSize = getMapSize();
    for (int i = 0; i < mapSize.width; i++) {
        for (int j = 0; j < mapSize.height; j++) {
            Sprite* sprite = pTrapLayer->getTileAt(Point(i, j));
            if (sprite != NULL) {
                PhysicsBody* body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
                body->setDynamic(false);
                body->setContactTestBitmask(0xFFFFFFFF);
                sprite->setPhysicsBody(body);
            }
        }
    }

    // add land
    TMXLayer* pLandLayer = getLayer("land");
    if (pLandLayer == NULL) {
        return false;
    }

	for (int i = 0; i < mapSize.width; i++) {
		for (int j = 0; j < mapSize.height; j++) {
			Sprite* sprite = pLandLayer->getTileAt(Point(i, j));
			if (sprite != NULL) {
				PhysicsBody* body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
				body->setDynamic(false);
                body->setContactTestBitmask(0xFFFFFFFF);
				sprite->setPhysicsBody(body);
			}
		}
	}

    //TODO: add object

    PhysicsBody* body = PhysicsBody::createEdgeBox(getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    body->setDynamic(false);
    body->setContactTestBitmask(0xFFFFFFFF);
    setPhysicsBody(body);

    Size winSize = Director::getInstance()->getWinSize();
    Size mapContentSize = getContentSize();
    mMapScale = winSize.height / mapContentSize.height;
    // fix hero position in the middle of the window
    mMapMoveStartPosition = winSize.width / (mMapScale *  2);
    mMapMoveEndPosition = mapContentSize.width - mMapMoveStartPosition;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameMap::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void GameMap::onMsgReceive(int enMsg, void *pData, int nSize) {

}

void GameMap::onFrameUpdate(float dt) {
    if (mHero != NULL) {
        Vec2 newPosition = mHero->getPosition();
        //如果Mario的位置变化了，则地图才会卷动
        // when offset > 1, move map to keep hero in the middle of the window
        if (newPosition.x > mMapMoveStartPosition
            && newPosition.x < mMapMoveEndPosition) {
            setPositionX(mMapMoveStartPosition - newPosition.x);
        }
    }
}

void GameMap::heroMove(float dt) {

}

Sprite *GameMap::getSpriteInPosition(Vec2 point) {
    return nullptr;
}

Size GameMap::getMapContentSize() {
    return cocos2d::Size();
}

Hero *GameMap::getHero() {
    return mHero;
}

int GameMap::TileMapPosToTileType(Point HeroPos, float fMapMove)
{
	return 0;
}

bool GameMap::onContactBegin(PhysicsContact &contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    if (mHero == nodeA || nodeB == mHero) {
        mHero->updateSpeed(mPercent);
    }

    //bodies can collide
    return true;
}

void GameMap::updatePercent(int percent) {
    mPercent = percent;
    Point position = mHero->getPosition();
    TMXLayer* pLandLayer = getLayer("land");
    float left = position.x - mHero->getContentSize().width / 2;
    float right = position.x + mHero->getContentSize().width / 2;
    Size tileSize = getTileSize();
    Size mapSize = getMapSize();
    int leftIndex = left / tileSize.width;
    int rightIndex = right / tileSize.width;
    int index = mapSize.height - (int)(position.y / tileSize.height - 0.5);
    if (index >= 0 && index < mapSize.height - 1) {
        Node* nodeL = NULL;
        Node* nodeR = NULL;
        if (leftIndex >= 0 && leftIndex < mapSize.width - 1) {
            nodeL = pLandLayer->getTileAt(Point(leftIndex, index));
        }
        if (rightIndex >= 0 && rightIndex < mapSize.width - 1) {
            nodeR = pLandLayer->getTileAt(Point(rightIndex, index));
        }
        if (nodeL != NULL || nodeR != NULL) {
            mHero->updateSpeed(percent);
        }
    }

}























