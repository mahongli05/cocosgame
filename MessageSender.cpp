//
// Created by Administrator on 2017/3/12.
//

#include "MessageSender.h"

void MessageSender::setReceiver(MessageReceiver *pReceiver) {
    mMessageReceiver = pReceiver;
}

void MessageSender::sendMsg(int enMsg, void *pData, int nSize) {
    if (mMessageReceiver != NULL) {
        mMessageReceiver->onMsgReceive(enMsg, pData, nSize);
    }
}



