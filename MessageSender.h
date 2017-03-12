//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_MESSAGESENDER_H
#define PROJ_ANDROID_STUDIO_MESSAGESENDER_H

#include <cstdio>
#include "MessageReceiver.h"

class MessageSender {
protected:
    MessageReceiver* mMessageReceiver;
public:
    void setReceiver(MessageReceiver *pReceiver);

    void sendMsg(int enMsg, void *pData = NULL, int nSize = 0);
};


#endif //PROJ_ANDROID_STUDIO_MESSAGESENDER_H
