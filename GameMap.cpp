//
// Created by Administrator on 2017/3/12.
//

#include "GameMap.h"
#include "PhysicsConfig.h"
#include "Enemy.h"

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
                body->setCollisionBitmask(0x01);
                body->setContactTestBitmask(TRAP_CONTACT_TEST_MASK);
                body->setCategoryBitmask(TRAP_CATEGORY_MASK);
                body->setCollisionBitmask(TRAP_COLLECTION_MASK);
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
                body->setContactTestBitmask(LAND_CONTACT_TEST_MASK);
                body->setCategoryBitmask(LAND_CATEGORY_MASK);
                body->setCollisionBitmask(LAND_COLLECTION_MASK);
				sprite->setPhysicsBody(body);
			}
		}
	}

    //TODO: add object
    loadObjects();

    PhysicsBody* body = PhysicsBody::createEdgeBox(getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
    body->setDynamic(false);
    body->setContactTestBitmask(BOX_CONTACT_TEST_MASK);
    body->setCategoryBitmask(BOX_CATEGORY_MASK);
    body->setCollisionBitmask(BOX_COLLECTION_MASK);
    setPhysicsBody(body);

    Size winSize = Director::getInstance()->getWinSize();
    Size mapContentSize = getContentSize();
    mMapScale = winSize.height / mapContentSize.height;
    // fix hero position in the middle of the window
    mMapVisibleWidth = winSize.width / mMapScale;
    mMapMoveStartPosition = mMapVisibleWidth / 2;
    mMapMoveEndPosition = mapContentSize.width - mMapMoveStartPosition;

    auto contactListener = EventListenerPhysicsContact::create();
    contactListener->onContactBegin = CC_CALLBACK_1(GameMap::onContactBegin, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void GameMap::onMsgReceive(int enMsg, void *pData, int nSize) {

}

void GameMap::onFrameUpdate(float dt) {
    if (mHero != NULL && mHero->getState() != STATE_DIE) {
        Vec2 newPosition = mHero->getPosition();
        //如果Mario的位置变化了，则地图才会卷动
        // when offset > 1, move map to keep hero in the middle of the window
        if (newPosition.x > mMapMoveStartPosition
            && newPosition.x < mMapMoveEndPosition) {
            setPositionX(mMapMoveStartPosition - newPosition.x);
        }
        Point newVelocity = mHero->getPhysicsBody()->getVelocity();
        if (abs(newVelocity.x - mHorizontalVelocity) > FLT_EPSILON
            || abs(newVelocity.y - mVerticalVelocity) > FLT_EPSILON) {
            log("HERO: onFrameUpdate: %f, %f", newVelocity.x, newVelocity.y);
            mHorizontalVelocity = newVelocity.x;
            mVerticalVelocity = newVelocity.y;
        }
    }

    // When enemy become visible, set to dynamic.
    float mapVisibleRight = mMapVisibleWidth - getPosition().x;
    for (Enemy * enemy : mEnemyVector) {
        PhysicsBody* body = enemy->getPhysicsBody();
        bool isDynamic = body->isDynamic();
        float x = enemy->getPosition().x;
        if (!isDynamic && x < mapVisibleRight) {
            body->setDynamic(true);
        }
    }
}

Sprite *GameMap::getSpriteInPosition(Vec2 point) {
    return nullptr;
}

Size GameMap::getMapContentSize() {
    return cocos2d::Size();
}

bool GameMap::onContactBegin(PhysicsContact &contact) {
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    int mask = nodeA->getPhysicsBody()->getCategoryBitmask()
               | nodeB->getPhysicsBody()->getCategoryBitmask();

    switch (mask) {
        case HERO_WIN:
            // win;
            log("GameMap: contactBegin: hero win");
            break;

        case HERO_LAND:
            // hit land;
            log("GameMap: contactBegin: hero land");
            break;

        case HERO_TRAP:
            // hit trap, hero die
            log("GameMap: contactBegin: hero trap");
            onHeroDied();
            break;

        case HERO_ENEMY:
            // hero die
            log("GameMap: contactBegin: hero enemy");
//            onHeroDied();
            onEnemyDied(nodeA, nodeB);
            break;

        case ENEMY_LAND:
            // enemy land;
            log("GameMap: contactBegin: enemy land");
            break;

        case ENEMY_TRAP:
            // enemy die
            log("GameMap: contactBegin: enemy trap");
            onEnemyDied(nodeA, nodeB);
            break;

        case ENEMY_ENEMY:
            // enemy hit, change velocity;
            log("GameMap: contactBegin: enemy enemy");
            break;

        default:
            break;

    }

    log("GameMap: contactBegin: %d, %d", nodeA->getPhysicsBody()->getCategoryBitmask(),
        nodeB->getPhysicsBody()->getCategoryBitmask());

    return true;
}

void GameMap::updatePercent(int percent) {
    mPercent = percent;
    Point position = mHero->getPosition();
    TMXLayer* pLandLayer = getLayer("land");
    float left = position.x - mHero->getContentSize().width / 2;
    float right = position.x + mHero->getContentSize().width / 2;
    float height = position.y - mHero->getContentSize().height / 2;
    Size tileSize = getTileSize();
    Size mapSize = getMapSize();
    int leftIndex = left / tileSize.width;
    int rightIndex = right / tileSize.width;
    int heightIndexToBottom = height / tileSize.height;
    int heightIndexToTop = mapSize.height - heightIndexToBottom - 1;
    float offset = height - (heightIndexToBottom + 1) * tileSize.height;
    if (heightIndexToTop >= 0 && offset < 1 && heightIndexToTop < mapSize.height - 1) {
        Node* nodeL = NULL;
        Node* nodeR = NULL;
        if (leftIndex >= 0 && leftIndex < mapSize.width - 1) {
            nodeL = pLandLayer->getTileAt(Point(leftIndex, heightIndexToTop));
        }
        if (rightIndex >= 0 && rightIndex < mapSize.width - 1) {
            nodeR = pLandLayer->getTileAt(Point(rightIndex, heightIndexToTop));
        }
        if (nodeL != NULL || nodeR != NULL) {
            mHero->updateSpeed(percent);
        }
    }
}

GameMap::GameMap() {
    mHorizontalVelocity = 0;
    mVerticalVelocity = 0;
}

GameMap::~GameMap() {

}

void GameMap::onHeroDied() {
    if (mHero != nullptr) {
        mHero->updateState(State::STATE_DIE);
    }
}

void GameMap::loadObjects() {

    TMXObjectGroup* group = getObjectGroup("objects");
    ValueVector vector = group->getObjects();
    for (Value value : vector) {
        if (value.getType() == Value::Type::MAP) {
            ValueMap valueMap = value.asValueMap();
            std::string name = valueMap.at("name").asString();
            float x = valueMap.at("x").asFloat();
            float y = valueMap.at("y").asFloat();
            if (name == "others") {
                // hero born;
            } else {
                Enemy* enemy = Enemy::createEnemy(name);
                if (enemy != NULL) {
                    enemy->setAnchorPoint(Point(0.5, 0));
                    enemy->setPosition(Point(x, y));
                    addChild(enemy, 100);
                    mEnemyVector.pushBack(enemy);
                }
            }
        }
    }
}

void GameMap::onEnemyDied(Node *nodeA, Node *nodeB) {
    log("GameMap: onEnemyDied: %d, %d", nodeA->getPhysicsBody()->getCategoryBitmask(),
        nodeB->getPhysicsBody()->getCategoryBitmask());
    if (nodeA->getPhysicsBody()->getCategoryBitmask() == ENEMY_CATEGORY_MASK) {
        ((Enemy*)nodeA)->die();
    }
    if (nodeB->getPhysicsBody()->getCategoryBitmask() == ENEMY_CATEGORY_MASK) {
        ((Enemy*)nodeA)->die();
    }
}

































