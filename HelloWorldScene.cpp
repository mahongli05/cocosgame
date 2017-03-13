#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Point(0, -200));

    // 'layer' is an autorelease object
    auto helloWord = HelloWorld::create();

    auto layer = GameContainer::createContainer(0);
    Size winSize = Director::getInstance()->getWinSize();
    Size mapSize = layer->getMapContentSize();
    float scale = winSize.height / mapSize.height;
    layer->setScale(scale);
    layer->setAnchorPoint(Vec2(0, 0));
    // add layer as a child to scene
    scene->addChild(layer, 3);
    layer->startGame();

    helloWord->mGameContainer = layer;

    Slider * slider = Slider::create();
    slider->loadBarTexture("bg_precent.png");
    slider->loadProgressBarTexture("precent.png");
    slider->loadSlidBallTextures("CloseNormal.png");
    slider->setAnchorPoint(Point(0, 0));
    slider->setPosition(Point(0, winSize.height-slider->getContentSize().height));
    slider->setMaxPercent(100);
    slider->setPercent(0);
    slider->addEventListener(CC_CALLBACK_2(HelloWorld::sliderEvent, helloWord));
    scene->addChild(slider, 4);

    scene->addChild(helloWord);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Layer::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. add a menu item with "X" image, which is clicked to quit the program
    //    you may modify it.

    // add a "close" icon to exit the progress. it's an autorelease object
    auto closeItem = MenuItemImage::create(
                                           "CloseNormal.png",
                                           "CloseSelected.png",
                                           CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    closeItem->setPosition(Vec2(origin.x + visibleSize.width - closeItem->getContentSize().width/2 ,
                                origin.y + closeItem->getContentSize().height/2));

    // create menu, it's an autorelease object
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. add your codes below...

    // add a label shows "Hello World"
    // create and initialize a label

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);

    // position the label on the center of the screen
    label->setPosition(Vec2(origin.x + visibleSize.width/2,
                            origin.y + visibleSize.height - label->getContentSize().height));

    // add the label as a child to this layer
    this->addChild(label, 1);

    // add "HelloWorld" splash screen"
    auto sprite = Sprite::create("HelloWorld.png");

    // position the sprite on the center of the screen
    sprite->setPosition(Vec2(visibleSize.width/2 + origin.x, visibleSize.height/2 + origin.y));

    // add the sprite as a child to this layer
    this->addChild(sprite, 0);

    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    //Close the cocos2d-x game scene and quit the application
    Director::getInstance()->end();

    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    exit(0);
#endif

    /*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);


}

void HelloWorld::sliderEvent(cocos2d::Ref *sender, cocos2d::ui::Slider::EventType eventType) {
//    if (eventType == cocos2d::ui::Slider::EventType::ON_PERCENTAGE_CHANGED) {
        Slider* slider = dynamic_cast<Slider*>(sender);
        int percent = slider->getPercent();
        mGameContainer->updateHeroSpeed(percent);
//    }
}


