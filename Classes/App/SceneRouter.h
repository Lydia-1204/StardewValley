#pragma once
#ifndef SCENEROUTER_H
#define SCENEROUTER_H

#include "cocos2d.h"
#include <string>

class SceneRouter {
public:
    static SceneRouter* getInstance();
    void goTo(const std::string& route);
    void goTo(const std::string& route, int selectedCharacter, const std::string& nickname);
    void goToError(const std::string& message);
    void pause();
    void resume();

private:
    static SceneRouter* instance;
    void replaceWithFade(cocos2d::Scene* scene, const cocos2d::Color3B& color);
};

#endif
