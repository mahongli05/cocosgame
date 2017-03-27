//
// Created by Administrator on 2017/3/12.
//

#include "GameMap.h"
#include "PhysicsConfig.h"
#include "Enemy.h"
#include "Bullet.h"
#include "StateFlag.h"
#include "NodeUtil.h"
#include "FloatingLand.h"

static const int LIVE_LAND_COUNT = 4;
static const int LIVE_LAND_OFFSET = 2;

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
    return NULL;
}

bool GameMap::init() {

    if (!Node::init()) {
        return false;
    }

    mHero = createHero(Point(20, 100));
    if (mHero == NULL) {
        return false;
    }

    // add trap
    if (!loadTrap()) {
        return false;
    }

    // add land
    if (!loadLand()) {
        return false;
    }

    if (!loadObjects()) {
        return false;
    }

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
    contactListener->onContactSeparate = CC_CALLBACK_1(GameMap::onContactSeparate, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);

    return true;
}

void GameMap::onFrameUpdate(float dt) {

    if (mHero != NULL && mHero->isLive()) {
        Vec2 newPosition = mHero->getPosition();
        //如果Mario的位置变化了，则地图才会卷动
        // when offset > 1, move map to keep hero in the middle of the window
        if (newPosition.x > mMapMoveStartPosition
            && newPosition.x < mMapMoveEndPosition) {
            float newMapX = mMapMoveStartPosition - newPosition.x;
            if (getPosition().x > newMapX) {
                setPositionX(newMapX);
            }
        }
        mHero->onFrameUpdate(dt);
    }

    // When enemy become visible, set to dynamic.
    float mapVisibleRight = mMapVisibleWidth - getPosition().x;

    for (Vector<Enemy*>::iterator it = mInactiveEnemyVector.begin();
         it != mInactiveEnemyVector.end();) {
        Enemy* enemy = *it;
        float x = enemy->getPosition().x;
        if (x < mapVisibleRight) {
            enemy->active();
            it = mInactiveEnemyVector.erase(it);
        } else {
            ++it;
        }
    }

    for (Vector<Bullet*>::iterator it = mInactiveBulletE.begin();
         it != mInactiveBulletE.end();) {
        Bullet* bullet = *it;
        float x = bullet->getPosition().x;
        if (x < mapVisibleRight) {
            bullet->active();
            it = mInactiveBulletE.erase(it);
        } else {
            ++it;
        }
    }

    int activeGroup = -1;
    for ( Vector<FloatingLand*>::iterator it = mInactiveMoveLand.begin();
          it != mInactiveMoveLand.end();) {
        FloatingLand* land = *it;
        float x = land->getPosition().x;
        if (x < mapVisibleRight) {
            land->active();
            activeGroup = land->getGroup();
            mActiveLand.pushBack(land);
            it = mInactiveMoveLand.erase(it);
        } else if (land->getGroup() == activeGroup) {
            land->active();
            mActiveLand.pushBack(land);
            it = mInactiveMoveLand.erase(it);
        } else {
            ++it;
        }
    }

    for ( Vector<FloatingLand*>::iterator it = mInactiveDropLand.begin();
          it != mInactiveDropLand.end();) {
        FloatingLand* land = *it;
        float x = land->getPosition().x;
        if (x < mapVisibleRight) {
            land->active();
            mActiveLand.pushBack(land);
            it = mInactiveDropLand.erase(it);
        } else {
            ++it;
        }
    }

    for (FloatingLand* land : mActiveLand) {
        land->onFrameUpdate(dt);
    }

    if (mHero != nullptr && mHero->isLive()) {
        Point anchor = mHero->getAnchorPoint();
        float left = mHero->getPosition().x - mHero->getContentSize().width * anchor.x;
        float right = mHero->getPosition().x + mHero->getContentSize().width * (1 - anchor.x);
        float bottom = mHero->getPosition().y - mHero->getContentSize().height * anchor.y;
        for (Vector<FloatingLand*>::iterator it = mContactDropLand.begin();
             it != mContactDropLand.end();) {
            FloatingDropLand* land = (FloatingDropLand*)*it;
            Rect rect = NodeUtil::getRectToParent(land);
            if (bottom - rect.getMaxY() < 0.0f
                && bottom > rect.getMinY()
                && left < rect.getMaxX()
                && right > rect.getMinX()) {
                land->setDropEnable(true);
                it = mContactDropLand.erase(it);
            } else {
                it++;
            }
        }
    }

    if (mWin != NULL) {
        if (mWin->getPosition().x < mapVisibleRight) {
            PhysicsBody* body = mWin->getPhysicsBody();
            if (body != NULL) {
                body->setDynamic(true);
            }
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
            onHeroWin();
            break;

        case HERO_LAND:
            onHeroLand(nodeA, nodeB);
            break;

        case HERO_TRAP:
            onHeroTrap(nodeA, nodeB);
            break;

        case HERO_ENEMY:
            onHeroHit(nodeA, nodeB);
            break;

        case ENEMY_LAND:
            break;

        case ENEMY_TRAP:
            onEnemyDied(nodeA, nodeB);
            break;

        case ENEMY_ENEMY:
            break;

        case BULLET_E_HERO:
            onHeroHit(nodeA, nodeB);
            break;

        case BULLET_H_ENEMY:
            onEnemyDied(nodeA, nodeB);
            break;

        case BULLET_E_BOX:
            break;

        case BULLET_H_BOX:
            break;

        default:
            break;

    }

    log("GameMap: contactBegin: %d, %d", nodeA->getPhysicsBody()->getCategoryBitmask(),
        nodeB->getPhysicsBody()->getCategoryBitmask());

    return true;
}

void GameMap::updatePercent(int percent) {

    if (isOnLand()) {
        float h = percent > 30 ? percent * 3 : 0;
        float v = percent > 60 ? (percent - 50) * 7 : 0;
        log("HERO1: updateSpeed: %d, %f, %f, %f, %f", percent, h, v, getPosition().x, getPosition().y);
        mHero->updateSpeed(h, v);
    }
}

GameMap::GameMap() {
    mHero = NULL;
    mWin = NULL;
    mGameEventCallback = NULL;
}

GameMap::~GameMap() {
    mInactiveBulletE.clear();
    mInactiveEnemyVector.clear();
    mInactiveMoveLand.clear();
    mInactiveDropLand.clear();
    mActiveLand.clear();
    mHitLand.clear();
    mHero = NULL;
    mWin = NULL;
    mGameEventCallback = NULL;
}

void GameMap::onHeroHit(Node* nodeA, Node* nodeB) {
    if (mHero != nullptr) {
        if (mHero->isCrazy()) {
            onEnemyDied(nodeA, nodeB);
        } else {
            onHeroDied();
        }
    }
}

void GameMap::onHeroDied() {
    if (mHero != nullptr) {
        mHero->setStateFlag(StateFlag::removeFlag(mHero->getStateFlag(), STATE_LIVE_MASK));
    }
    if (mGameEventCallback != NULL) {
        mGameEventCallback->onEvent(GAME_OVER);
    }
}

void GameMap::onHeroWin() {
    if (mGameEventCallback != NULL) {
        mGameEventCallback->onEvent(GAME_WIN);
    }
}

void GameMap::onHeroLand(Node *nodeA, Node *nodeB) {

    Node* landNode = nullptr;
    if (nodeA->getPhysicsBody()->getCategoryBitmask() == LAND_CATEGORY_MASK) {
        landNode = nodeA;
    } else {
        landNode = nodeB;
    }

    mHitLand.pushBack(landNode);

    if (landNode->getTag() == FLOAT_LAND_DROP) {
        mContactDropLand.pushBack((FloatingLand*)landNode);
    }

    if (landNode->getTag() == FLOAT_LAND_MOVE) {
        mContactFloatingLand = (FloatingLand*)landNode;
    }
}

void GameMap::onHeroTrap(Node *nodeA, Node *nodeB) {
    Node* trapNode = nodeA;
    if (trapNode->getPhysicsBody()->getCategoryBitmask() != TRAP_CATEGORY_MASK) {
        trapNode = nodeB;
    }
    float trapTop = NodeUtil::getTop(trapNode);
    float trapBottom = NodeUtil::getBottom(trapNode);
    float heroTop = NodeUtil::getTop(mHero);
    float heroBottom = NodeUtil::getBottom(mHero);
    if (heroTop > trapBottom || heroBottom < trapTop) {
        onHeroDied();
    }
}

void GameMap::onEnemyDied(Node *nodeA, Node *nodeB) {
    if (nodeA->getPhysicsBody()->getCategoryBitmask() == ENEMY_CATEGORY_MASK) {
        ((Enemy*)nodeA)->updateState(ENEMY_DIE);
    }
    if (nodeB->getPhysicsBody()->getCategoryBitmask() == ENEMY_CATEGORY_MASK) {
        ((Enemy*)nodeB)->updateState(ENEMY_DIE);
    }
}

bool GameMap::loadObjects() {

    TMXObjectGroup* group = getObjectGroup("objects");
    if (group == nullptr) {
        return false;
    }

    ValueVector vector = group->getObjects();
    for (Value value : vector) {
        if (value.getType() == Value::Type::MAP) {
            ValueMap valueMap = value.asValueMap();
            std::string name = valueMap.at("name").asString();

            log("name: %s", name.c_str());

            float x = valueMap.at("x").asFloat();
            float y = valueMap.at("y").asFloat();
            if (name == "others") {
                // hero born;
            } else if (name == "win") {
                Sprite *win = Sprite::create("princess.png");
                PhysicsBody *body = PhysicsBody::createBox(win->getContentSize(),
                                                           PhysicsMaterial(0.1f, 0.0f, 0.0f));
                body->setContactTestBitmask(WIN_CONTACT_TEST_MASK);
                body->setCategoryBitmask(WIN_CATEGORY_MASK);
                body->setCollisionBitmask(WIN_COLLECTION_MASK);
                body->setGravityEnable(true);
                body->setDynamic(false);
                body->setRotationEnable(false);
                win->setPhysicsBody(body);
                addChild(win, 100);
                win->setAnchorPoint(Point(0.5, 0));
                win->setPosition(Point(x, y));
                mWin = win;
            } else if (name == "fire") {
                Bullet *bullet = Bullet::createBullet(name);
                if (bullet != NULL) {
                    bullet->setAnchorPoint(Point(0.5, 0));
                    bullet->setPosition(Point(x, y));
                    addChild(bullet, 101);
                    mInactiveBulletE.pushBack(bullet);
                }
            } else if (name == "floatingMove") {
                float distance = valueMap.at("distance").asFloat();
                float speed = valueMap.at("speed").asFloat();
                int direction = valueMap.at("direction").asInt();
                int group = valueMap.at("group").asInt();
                FloatingMovingLand* floatingLand = (FloatingMovingLand*)FloatingLand::createFloatingLand(name);
                if (floatingLand != nullptr) {
                    floatingLand->setDirection(direction);
                    floatingLand->setSpeed(speed);
                    floatingLand->setDistance(distance);
                    floatingLand->setGroup(group);
                    mInactiveMoveLand.pushBack(floatingLand);
                    floatingLand->setAnchorPoint(Point(0.5, 0));
                    floatingLand->setPosition(Point(x, y));
                    addChild(floatingLand, 100);
                }
            } else if (name == "floatingDrop") {
                float duration = valueMap.at("duration").asFloat();
                FloatingDropLand* floatingLand = (FloatingDropLand*)FloatingLand::createFloatingLand(name);
                if (floatingLand != nullptr) {
                    floatingLand->setDuration(duration);
                    mInactiveDropLand.pushBack(floatingLand);
                    floatingLand->setAnchorPoint(Point(0.5, 0));
                    floatingLand->setPosition(Point(x, y));
                    addChild(floatingLand, 100);
                }
            } else {
                Enemy* enemy = Enemy::createEnemy(name);
                if (enemy != NULL) {
                    enemy->setAnchorPoint(Point(0.5, 0));
                    enemy->setPosition(Point(x, y));
                    addChild(enemy, 100);
                    mInactiveEnemyVector.pushBack(enemy);
                }
            }
        }
    }

    return true;
}

void GameMap::setGameEventCallback(GameEventCallback* callback) {
    mGameEventCallback = callback;
}

bool GameMap::reliveHero() {
    removeChild(mHero);
    // find land to born
    TMXLayer* pLandLayer = getLayer("land");
    Size tileSize = getTileSize();
    Size mapSize = getMapSize();
    float startX = -getPosition().x;
    float startTile = startX / tileSize.width;
    float endTile = startTile + mMapVisibleWidth / tileSize.width;
    int count = 0;
    mHero = nullptr;
    for (float i = startTile; i < endTile; i++) {
        Node* land = pLandLayer->getTileAt(Point(i, mapSize.height - 1));
        if (land != nullptr) {
            count++;
            // find four land nearby
            if (count >= LIVE_LAND_COUNT) {
                float bornX = (i - LIVE_LAND_OFFSET) * tileSize.width;
                mHero = createHero(Point(bornX, 200));
                break;
            }
        } else {
            count = 0;
        }
    }

    return mHero != nullptr;
}

Hero *GameMap::createHero(Point position) {
    Hero* hero = Hero::createHero(this);
    if (hero != nullptr) {
        hero->setAnchorPoint(Vec2(0.5, 0.5));
        hero->setPosition(position);
        hero->setCrazy(5.0f);
        addChild(hero, 100);
    }
    return hero;
}

bool GameMap::isOnLand() {

    if (mHero != nullptr && mHitLand.size() > 0) {
        Point anchor = mHero->getAnchorPoint();
        float left = mHero->getPosition().x - mHero->getContentSize().width * anchor.x;
        float right = mHero->getPosition().x + mHero->getContentSize().width * (1 - anchor.x);
        float bottom = mHero->getPosition().y - mHero->getContentSize().height * anchor.y;

        for (Node* node : mHitLand) {
            Rect rect = NodeUtil::getRectToParent(node);
            float offsetToTop = bottom - rect.getMaxY();
            float offsetToBottom = bottom - rect.getMinY();
            if ( offsetToTop < 0.0f
                 && offsetToBottom > 0.0f
                 && left < rect.getMaxX()
                 && right > rect.getMinX()) {
                return true;
            }
        }
    }

    return false;
}

bool GameMap::loadLand() {

    TMXLayer* pLandLayer = getLayer("land");
    if (pLandLayer == NULL) {
        return false;
    }

    Size mapSize = getMapSize();
    for (int i = 0; i < mapSize.width; i++) {
        for (int j = 0; j < mapSize.height; j++) {
            Sprite* sprite = pLandLayer->getTileAt(Point(i, j));
            if (sprite != nullptr) {
                PhysicsBody* body = PhysicsBody::createBox(sprite->getContentSize(), PhysicsMaterial(0.1f, 0.0f, 0.0f));
                body->setDynamic(false);
                body->setContactTestBitmask(LAND_CONTACT_TEST_MASK);
                body->setCategoryBitmask(LAND_CATEGORY_MASK);
                body->setCollisionBitmask(LAND_COLLECTION_MASK);
                sprite->setPhysicsBody(body);
                sprite->setTag(STATIC_LAND);
            }
        }
    }

    return true;
}

bool GameMap::loadTrap() {
    // add trap
    TMXLayer* pTrapLayer = getLayer("trap");
    if (pTrapLayer == NULL) {
        return false;
    }

    Size mapSize = getMapSize();
    for (int i = 0; i < mapSize.width; i++) {
        for (int j = 0; j < mapSize.height; j++) {
            Sprite *sprite = pTrapLayer->getTileAt(Point(i, j));
            if (sprite != nullptr) {
                PhysicsBody *body = PhysicsBody::createBox(sprite->getContentSize(),
                                                           PhysicsMaterial(0.1f, 0.0f, 0.0f));
                body->setDynamic(false);
                body->setCollisionBitmask(0x01);
                body->setContactTestBitmask(TRAP_CONTACT_TEST_MASK);
                body->setCategoryBitmask(TRAP_CATEGORY_MASK);
                body->setCollisionBitmask(TRAP_COLLECTION_MASK);
                sprite->setPhysicsBody(body);
            }
        }
    }

    return true;
}

void GameMap::onContactSeparate(PhysicsContact &contact) {

    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();

    int mask = nodeA->getPhysicsBody()->getCategoryBitmask()
               | nodeB->getPhysicsBody()->getCategoryBitmask();

    switch (mask) {
        case HERO_LAND:
            onHeroLandSeparate(nodeA, nodeB);
            break;

        case ENEMY_LAND:
            break;

        default:
            break;

    }

    log("GameMap: contactBegin: %d, %d", nodeA->getPhysicsBody()->getCategoryBitmask(),
        nodeB->getPhysicsBody()->getCategoryBitmask());
}

void GameMap::onHeroLandSeparate(Node *nodeA, Node *nodeB) {

    Node* landNode = nullptr;
    if (nodeA->getPhysicsBody()->getCategoryBitmask() == LAND_CATEGORY_MASK) {
        landNode = nodeA;
    } else {
        landNode = nodeB;
    }

    ssize_t index = mHitLand.getIndex(landNode);
    if (index >= 0) {
        mHitLand.erase(index);
    }

    if (landNode->getTag() == FLOAT_LAND_DROP) {
        ssize_t index = mContactDropLand.getIndex((FloatingLand*)landNode);
        if (index >= 0) {
            mContactDropLand.erase(index);
        }
    }

    log("GameMap: onHeroLandSeparate: %d, %d", nodeA->getPhysicsBody()->getCategoryBitmask(),
        nodeB->getPhysicsBody()->getCategoryBitmask());
}