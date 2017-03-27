//
// Created by MHL on 2017/3/17.
//

#ifndef PROJ_ANDROID_STUDIO_NODEUTIL_H
#define PROJ_ANDROID_STUDIO_NODEUTIL_H

#include "cocos2d.h"

using namespace cocos2d;

class NodeUtil {

public:
    static Rect getRectToParent(Node* node);
    static float getLeft(Node* node);
    static float getRight(Node* node);
    static float getTop(Node* node);
    static float getBottom(Node* node);
};


#endif //PROJ_ANDROID_STUDIO_NODEUTIL_H
