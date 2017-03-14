//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_GAMEMAP_H
#define PROJ_ANDROID_STUDIO_GAMEMAP_H

#include "cocos2d.h"
#include "MessageReceiver.h"
#include "MapHelper.h"
#include "Hero.h"
#include "Enemy.h"

using namespace cocos2d;

class GameMap:public TMXTiledMap, public MessageReceiver, public MapHelper{

public:
    static GameMap* createGameMap(const char *pFileName);

    void onFrameUpdate(float dt);

    virtual Sprite* getSpriteInPosition(Vec2 point);
    virtual Size getMapContentSize();

    void updatePercent(int percent);

protected:
    virtual bool init();
    virtual void onMsgReceive(int enMsg, void *pData, int nSize);
    bool onContactBegin(PhysicsContact& contact);

private:

    GameMap();
    ~GameMap();

    float mMapMoveStartPosition;
    float mMapMoveEndPosition;
    float mMapVisibleWidth;
    float mMapScale;

    float mHorizontalVelocity;
    float mVerticalVelocity;

    int mPercent;

    Hero* mHero;
    Vector<Enemy*> mEnemyVector;

    void onHeroDied();
    void onEnemyDied(Node* nodeA, Node* nodeB);

    void loadObjects();
};


#endif //PROJ_ANDROID_STUDIO_GAMEMAP_H
