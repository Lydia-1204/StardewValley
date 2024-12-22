
/****************************************************************
 * Project Name:  StardewValley
 * File Name:    CreateErrorScene.cpp
 * File Function: ʵ��ErrorSceneHelper�࣬����쳣��ʾ���棬������������ҳ��
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/4
 ****************************************************************/

#include "CreateErrorScene.h"

#include "MenuScene.h" // ��Ҫ�������˵�

USING_NS_CC;

Scene* ErrorSceneHelper::createErrorScene(const std::string& errorMessage) {
    auto scene = Scene::create();
    auto director = Director::getInstance();
    auto visibleSize = director->getVisibleSize();
    auto origin = director->getVisibleOrigin();

    // ��ʾ������Ϣ
    auto label = Label::createWithTTF("Error occurred:\n" + errorMessage, "fonts/Marker Felt.ttf", 32);
    label->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
    label->setTextColor(Color4B::RED);
    scene->addChild(label);

    // ��ӷ��ذ�ť
    auto retryButton = MenuItemLabel::create(
        Label::createWithTTF("Return to Main Menu", "fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            try {
                // �������˵�
                auto menuScene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene, Color3B::BLACK));
            }
            catch (const std::exception& e) {
                CCLOG("Retry failed: %s", e.what());
                Director::getInstance()->end(); // �޷��ָ�������ʱֱ���˳���Ϸ
            }
        });

    auto menu = Menu::create(retryButton, nullptr);
    menu->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 - 50 + origin.y));
    scene->addChild(menu);

    return scene;
}