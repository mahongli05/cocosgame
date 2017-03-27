//
// Created by MHL on 2017/3/16.
//

#ifndef PROJ_ANDROID_STUDIO_ACTIONSTATE_H
#define PROJ_ANDROID_STUDIO_ACTIONSTATE_H

static const int STATE_ACTIVE_MASK = 0x020;
static const int STATE_CRAZY_MASK  = 0x002;
static const int STATE_JUMP_MASK   = 0x004;
static const int STATE_RUN_MASK    = 0x008;
static const int STATE_DIRECTION_MASK = 0x010;
static const int STATE_LIVE_MASK = 0x001;

static const int FULL_MASK = STATE_ACTIVE_MASK | STATE_CRAZY_MASK
                             | STATE_JUMP_MASK | STATE_RUN_MASK
                             | STATE_DIRECTION_MASK | STATE_LIVE_MASK;

static const int CRAZY_JUMP_RUN_DIRECTION = STATE_CRAZY_MASK | STATE_JUMP_MASK | STATE_RUN_MASK | STATE_DIRECTION_MASK;

static const int CRAZY_JUMP = STATE_CRAZY_MASK | STATE_JUMP_MASK;
static const int CRAZY_RIGHT = STATE_CRAZY_MASK | STATE_DIRECTION_MASK;
static const int CRAZY_JUMP_RIGHT = STATE_CRAZY_MASK | STATE_JUMP_MASK | STATE_DIRECTION_MASK;
static const int CRAZY_JUMP_RUN_RIGHT = STATE_CRAZY_MASK | STATE_JUMP_MASK | STATE_RUN_MASK | STATE_DIRECTION_MASK;
static const int CRAZY_RUN_RIGHT = STATE_CRAZY_MASK | STATE_RUN_MASK | STATE_DIRECTION_MASK;

static const int JUMP = STATE_JUMP_MASK;
static const int JUMP_RIGHT = STATE_JUMP_MASK | STATE_DIRECTION_MASK;
static const int JUMP_RUN_RIGHT = STATE_JUMP_MASK | STATE_RUN_MASK | STATE_DIRECTION_MASK;
static const int RUN_RIGHT = STATE_RUN_MASK | STATE_DIRECTION_MASK;

class StateFlag {

public:
    static bool containFlag(int flag, int mask);
    static int addFlag(int oldFlag, int mask);
    static int removeFlag(int oldFlag, int mask);
};


#endif //PROJ_ANDROID_STUDIO_ACTIONSTATE_H
