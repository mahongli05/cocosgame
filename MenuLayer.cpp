//
// Created by MHL on 2017/3/15.
//

#include <ui/UIButton.h>
#include "MenuLayer.h"

using namespace ui;

MenuLayer::MenuLayer() {
    mMenuCallback = NULL;
    mTitle = NULL;
}

MenuLayer::~MenuLayer() {
    mMenuCallback = NULL;
    mTitle = NULL;
}

bool MenuLayer::init() {

    if (!Layer::init()) {
        return false;
    }

    setColor(Color3B(255,255,0));

    mTitle = Label::create();
    mTitle->setAnchorPoint(Point(0.5, 0));
    mTitle->setPosition(Point(getContentSize().width/2, getContentSize().height*0.7f));
    mTitle->setBMFontSize(80);
    addChild(mTitle);

    Button* button = Button::create();
    button->setTitleText("Continue");
    button->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onContinueClick, this));
    button->setTitleFontSize(30);
    button->setAnchorPoint(Point(0.5, 0));
    button->setPosition(Point(getContentSize().width/2, getContentSize().height*0.5f));
    addChild(button);

    button = Button::create();
    button->setTitleText("Retry");
    button->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onRetryClick, this));
    button->setTitleFontSize(30);
    button->setAnchorPoint(Point(0.5, 0));
    button->setPosition(Point(getContentSize().width/2, getContentSize().height*0.4f));
    addChild(button);

    button = Button::create();
    button->setTitleText("Exit");
    button->setTitleFontSize(30);
    button->addTouchEventListener(CC_CALLBACK_2(MenuLayer::onExitClick, this));
    button->setAnchorPoint(Point(0.5, 0));
    button->setPosition(Point(getContentSize().width/2, getContentSize().height*0.3f));
    addChild(button);

    return true;
}

void MenuLayer::setMenuCallback(MenuCallback* menuCallback) {
    mMenuCallback = menuCallback;
}

void MenuLayer::onContinueClick(Ref* ref, Button::TouchEventType type) {
    if (mMenuCallback != NULL && type == Button::TouchEventType::ENDED) {
        mMenuCallback->onContinueClick();
    }
}

void MenuLayer::onRetryClick(Ref* ref, Button::TouchEventType type) {
    if (mMenuCallback != NULL && type == Button::TouchEventType::ENDED) {
        mMenuCallback->onRetryClick();
    }
}

void MenuLayer::onExitClick(Ref* ref, Button::TouchEventType type) {
    if (mMenuCallback != NULL && type == Button::TouchEventType::ENDED) {
        mMenuCallback->onExitClick();
    }
}

void MenuLayer::setTitle(std::string title) {
    if (mTitle!=NULL) {
        mTitle->setString(title);
    }
}
















