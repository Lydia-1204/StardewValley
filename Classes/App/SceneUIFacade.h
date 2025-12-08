#pragma once
#ifndef SCENEUIFACADE_H
#define SCENEUIFACADE_H

#include "cocos2d.h"
#include <string>

class SceneUIFacade {
public:
    static SceneUIFacade* getInstance();
    bool applyBackground(cocos2d::Scene* target, const std::string& imagePath);
    cocos2d::MenuItemImage* createLabeledButton(
        const std::string& imagePath,
        const std::string& text,
        const std::string& fontPath,
        float fontSize,
        const cocos2d::Size& screenSize,
        float widthRatio,
        float heightRatio,
        const cocos2d::Vec2& position,
        const std::function<void(cocos2d::Ref*)>& onClick);

private:
    static SceneUIFacade* instance;
};

#endif
