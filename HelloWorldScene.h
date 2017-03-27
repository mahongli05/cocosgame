#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <ui/UISlider.h>
#include "cocos2d.h"
#include "GameContainer.h"
#include "MenuLayer.h"

class HelloWorld : public cocos2d::Layer, public GameEventCallback, public MenuCallback
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType eventType);

    HelloWorld();
    ~HelloWorld();

    // control
    virtual void onEvent(GameEvent gameEvent);
    virtual void onRetryClick();
    virtual void onExitClick();
    virtual void onContinueClick();

    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event);
private:
    GameContainer* mGameContainer;
    MenuLayer* mMenuLayer;
};

#endif // __HELLOWORLD_SCENE_H__
