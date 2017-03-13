#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include <ui/UISlider.h>
#include "cocos2d.h"
#include "GameContainer.h"

class HelloWorld : public cocos2d::Layer
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(HelloWorld);

    void sliderEvent(cocos2d::Ref* sender, cocos2d::ui::Slider::EventType eventType);

private:
    GameContainer* mGameContainer;
};

#endif // __HELLOWORLD_SCENE_H__
