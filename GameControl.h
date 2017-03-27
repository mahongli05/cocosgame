//
// Created by MHL on 2017/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_GAMECONTROL_H
#define PROJ_ANDROID_STUDIO_GAMECONTROL_H

enum GameEvent {
    GAME_OVER,
    GAME_WIN
};

class GameControl {
public:
    virtual void play(int level) = 0;
};

class GameEventCallback {
public:
    virtual void onEvent(GameEvent gameEvent) = 0;
};


#endif //PROJ_ANDROID_STUDIO_GAMECONTROL_H
