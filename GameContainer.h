//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_GAMECONTAINER_H
#define PROJ_ANDROID_STUDIO_GAMECONTAINER_H

#include "cocos2d.h"

using namespace cocos2d;

class GameContainer : public Layer {

public:
    static GameContainer* createContainer(int level);

    void startGame();

    void onFrameUpdate(float dt);

    virtual bool init();

    Size getMapContentSize();

    // implement the "static create()" method manually
    CREATE_FUNC(GameContainer);

private:
};


#endif //PROJ_ANDROID_STUDIO_GAMECONTAINER_H
