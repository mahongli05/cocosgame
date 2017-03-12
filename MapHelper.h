//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_MAPHELPER_H
#define PROJ_ANDROID_STUDIO_MAPHELPER_H

#include "cocos2d.h"

using namespace cocos2d;

class MapHelper {

public:
    virtual Sprite* getSpriteInPosition(Vec2 point) = 0;
    virtual Size getMapContentSize() = 0;
};


#endif //PROJ_ANDROID_STUDIO_MAPHELPER_H
