
/****************************************************************
 * Project Name:  StardewValley
 * File Name:    CreateErrorScene.cpp
 * File Function: 实现ErrorSceneHelper类，完成异常显示界面，并可以跳回主页面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/4
 ****************************************************************/

#include "Scenes/CreateErrorScene.h"

#include "Scenes/MenuScene.h"
#include "App/SceneRouter.h"

USING_NS_CC;

Scene* ErrorSceneHelper::createErrorScene(const std::string& errorMessage) {
    auto scene = Scene::create();
    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    auto origin = director->getVisibleOrigin();

    // 显示错误信息
    auto label = Label::createWithTTF("Error occurred:\n" + errorMessage, "fonts/Marker Felt.ttf", 32);
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    label->setTextColor(Color4B::RED);
    scene->addChild(label);

    // 添加返回按钮
    auto retryButton = MenuItemLabel::create(
        Label::createWithTTF("Return to Main Menu", "fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            try {
                SceneRouter::getInstance()->goTo("Menu");
            }
            catch (const std::exception& e) {
                CCLOG("Retry failed: %s", e.what());
                Director::getInstance()->end();
            }
        });

    auto menu = Menu::create(retryButton, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 - 50 + origin.y));
    scene->addChild(menu);

    return scene;
}
