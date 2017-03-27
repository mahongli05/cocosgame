#include "HelloWorldScene.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

using namespace ui;

Scene* HelloWorld::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::createWithPhysics();

    scene->getPhysicsWorld()->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    scene->getPhysicsWorld()->setGravity(Point(0, -500));

    // 'layer' is an autorelease object
    auto helloWord = HelloWorld::create();

    auto layer = GameContainer::createContainer(0);
    Size winSize = Director::getInstance()->getWinSize();
    Size mapSize = layer->getMapContentSize();
    float scale = winSize.height / mapSize.height;
    layer->setScale(scale);
    layer->setAnchorPoint(Point(0, 0));
    layer->setGameEventCallback(helloWord);
    layer->startGame();
    helloWord->addChild(layer, 3);

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
    helloWord->addChild(slider, 4);

    MenuLayer* menuLayer = MenuLayer::create();
    menuLayer->setAnchorPoint(Point(0, 0));
    menuLayer->setMenuCallback(helloWord);
    helloWord->mMenuLayer = menuLayer;
    menuLayer->setVisible(false);
    helloWord->addChild(menuLayer, 8);
    helloWord->setKeyboardEnabled(true);

    scene->addChild(helloWord);

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

    return true;
}

void HelloWorld::sliderEvent(cocos2d::Ref *sender, cocos2d::ui::Slider::EventType eventType) {
    Slider* slider = dynamic_cast<Slider*>(sender);
    int percent = slider->getPercent();
    GameMap* gameMap = mGameContainer->getGameMap();
    if (gameMap != NULL) {
        gameMap->updatePercent(percent);
    }
}

HelloWorld::HelloWorld() {
    mGameContainer = NULL;
    mMenuLayer = NULL;
}

HelloWorld::~HelloWorld() {
    mGameContainer = NULL;
    mMenuLayer = NULL;
}

void HelloWorld::onEvent(GameEvent gameEvent) {
    switch (gameEvent) {
        case GAME_OVER:
            mMenuLayer->setVisible(true);
            mMenuLayer->setTitle("GAME OVER!!");
            break;

        case GAME_WIN:
            mMenuLayer->setVisible(true);
            mMenuLayer->setTitle("YOU WIN!!");
            break;

        default:
            break;
    }
}

void HelloWorld::onContinueClick() {
    GameMap* gameMap = mGameContainer->getGameMap();
    if (gameMap != nullptr) {
        if (gameMap->reliveHero()) {
            mMenuLayer->setVisible(false);
        }
    }
}

void HelloWorld::onRetryClick() {
    mGameContainer->loadMap(mGameContainer->getCurrentLevel());
    mMenuLayer->setVisible(false);
}

void HelloWorld::onExitClick() {
    Director::getInstance()->end();
}

void HelloWorld::onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event *event) {
    Layer::onKeyReleased(keyCode, event);
    if (keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE || keyCode == EventKeyboard::KeyCode::KEY_BACK) {
        Director::getInstance()->end();
    }
}


















