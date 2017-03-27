//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_GAMECONTAINER_H
#define PROJ_ANDROID_STUDIO_GAMECONTAINER_H

#include "cocos2d.h"
#include "Hero.h"
#include "GameMap.h"

using namespace cocos2d;

class GameContainer : public Layer {

public:
    static GameContainer* createContainer(int level);

    bool loadMap(int level);
    void setGameEventCallback(GameEventCallback* callback);
    void startGame();
    int getCurrentLevel();

    void onFrameUpdate(float dt);

    virtual bool init();

    Size getMapContentSize();

    // implement the "static create()" method manually
    CREATE_FUNC(GameContainer);

    GameMap* getGameMap();

    GameContainer();
    ~GameContainer();

private:

    GameMap* mGameMap;
    int mCurrentLevel;
    GameEventCallback* mGameEventCallback;
};


#endif //PROJ_ANDROID_STUDIO_GAMECONTAINER_H
