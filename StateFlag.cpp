//
// Created by MHL on 2017/3/16.
//

#include "StateFlag.h"

bool StateFlag::containFlag(int flag, int mask) {
    return (flag & mask) != 0;
}

int StateFlag::addFlag(int oldFlag, int mask) {
    return oldFlag | mask;
}

int StateFlag::removeFlag(int oldFlag, int mask) {
    return oldFlag & (FULL_MASK - mask);
}





