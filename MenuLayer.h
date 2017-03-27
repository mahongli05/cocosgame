//
// Created by MHL on 2017/3/15.
//

#ifndef PROJ_ANDROID_STUDIO_MENULAYER_H
#define PROJ_ANDROID_STUDIO_MENULAYER_H

#include "cocos2d.h"
#include "ui/UIButton.h"

using namespace cocos2d;
using namespace ui;

class MenuCallback {
public:
    virtual void onRetryClick() = 0;
    virtual void onExitClick() = 0;
    virtual void onContinueClick() = 0;
};

class MenuLayer : public Layer{

public:
    MenuLayer();
    ~MenuLayer();
    CREATE_FUNC(MenuLayer);

    void setMenuCallback(MenuCallback* menuCallback);

    void onContinueClick(Ref* ref, Button::TouchEventType type);
    void onRetryClick(Ref* ref, Button::TouchEventType type);
    void onExitClick(Ref* ref, Button::TouchEventType type);
    void setTitle(std::string title);

protected:
    virtual bool init();

private:
    MenuCallback* mMenuCallback;
    Label* mTitle;
};


#endif //PROJ_ANDROID_STUDIO_MENULAYER_H
