//
// Created by MHL on 2017/3/17.
//

#include "NodeUtil.h"

Rect NodeUtil::getRectToParent(Node *node) {
    if (node != nullptr) {
        Point anchor = node->getAnchorPoint();
        float left = node->getPosition().x - node->getContentSize().width * anchor.x;
        float bottom = node->getPosition().y - node->getContentSize().height * anchor.y;
        return cocos2d::Rect(left, bottom, node->getContentSize().width, node->getContentSize().height);
    }
    return cocos2d::Rect();
}

float NodeUtil::getLeft(Node *node) {
    Point anchor = node->getAnchorPoint();
    return node->getPosition().x - node->getContentSize().width * anchor.x;
}

float NodeUtil::getRight(Node *node) {
    Point anchor = node->getAnchorPoint();
    return node->getPosition().x + node->getContentSize().width * (1 - anchor.x);
}

float NodeUtil::getTop(Node *node) {
    Point anchor = node->getAnchorPoint();
    return node->getPosition().y + node->getContentSize().height * (1 - anchor.y);
}

float NodeUtil::getBottom(Node *node) {
    Point anchor = node->getAnchorPoint();
    return node->getPosition().y - node->getContentSize().height * anchor.y;
}









