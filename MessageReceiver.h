//
// Created by Administrator on 2017/3/12.
//

#ifndef PROJ_ANDROID_STUDIO_MESSAGE_H
#define PROJ_ANDROID_STUDIO_MESSAGE_H


class MessageReceiver {
public:
    virtual void onMsgReceive(int enMsg, void *pData, int nSize) = 0;
};


#endif //PROJ_ANDROID_STUDIO_MESSAGE_H
