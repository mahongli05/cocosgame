//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_GAMEMAP_H
#define PROJ_ANDROID_STUDIO_GAMEMAP_H

#include "cocos2d.h"
#include "MapHelper.h"
#include "Hero.h"
#include "Enemy.h"
#include "Bullet.h"
#include "GameControl.h"
#include "FloatingLand.h"

using namespace cocos2d;

class GameMap:public TMXTiledMap, public MapHelper{

public:
    static GameMap* createGameMap(const char *pFileName);

    void onFrameUpdate(float dt);

    virtual Sprite* getSpriteInPosition(Vec2 point);
    virtual Size getMapContentSize();

    void updatePercent(int percent);
    void setGameEventCallback(GameEventCallback* callback);
    bool reliveHero();
protected:
    virtual bool init();
    bool onContactBegin(PhysicsContact& contact);
    void onContactSeparate(PhysicsContact& contact);

private:

    GameMap();
    ~GameMap();

    float mMapMoveStartPosition;
    float mMapMoveEndPosition;
    float mMapVisibleWidth;
    float mMapScale;

    Hero* mHero;
    Sprite* mWin;
    Vector<Enemy*> mInactiveEnemyVector;
    Vector<Bullet*> mInactiveBulletE;
    Vector<Node*> mHitLand;
    Vector<FloatingLand*> mInactiveMoveLand;
    Vector<FloatingLand*> mInactiveDropLand;
    Vector<FloatingLand*> mActiveLand;

    Vector<FloatingLand*> mContactDropLand;

    FloatingLand* mContactFloatingLand;

    GameEventCallback* mGameEventCallback;

    void onHeroHit(Node* nodeA, Node* nodeB);
    void onHeroLand(Node* nodeA, Node* nodeB);
    void onHeroTrap(Node* nodeA, Node* nodeB);
    void onEnemyDied(Node* nodeA, Node* nodeB);
    void onHeroDied();
    void onHeroWin();

    void onHeroLandSeparate(Node *nodeA, Node *nodeB);

    bool isOnLand();

    bool loadObjects();
    bool loadLand();
    bool loadTrap();

    Hero* createHero(Point position);


    // control
    // continue with a new life
    // reload
};


#endif //PROJ_ANDROID_STUDIO_GAMEMAP_H
