//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_GAMEMAP_H
#define PROJ_ANDROID_STUDIO_GAMEMAP_H

#include "cocos2d.h"
#include "MessageReceiver.h"
#include "MapHelper.h"

using namespace cocos2d;

class GameMap:public TMXTiledMap, public MessageReceiver, public MapHelper{

public:
    static GameMap* createGameMap(const char *pFileName);

    void onFrameUpdate(float dt);

    virtual Sprite* getSpriteInPosition(Vec2 point);
    virtual Size getMapContentSize();

    int TileMapPosToTileType(Point HeroPos, float fMapMove);

protected:
    virtual bool init();
    virtual void onMsgReceive(int enMsg, void *pData, int nSize);

private:
    void heroMove(float dt);

    bool mLeftKeyDown;
    bool mRightKeyDown;
    bool mJumKeyDown;

    float mMapMoveStartPosition;
    float mMapMoveEndPosition;
};


#endif //PROJ_ANDROID_STUDIO_GAMEMAP_H
