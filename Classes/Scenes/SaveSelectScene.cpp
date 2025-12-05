
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SaveSelectScene.cpp
 * File Function: 实现SaveSelectScene类，完成存档选择界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

// 创建场景
#include "Scenes/MenuScene.h"
#include "Scenes/NewSelectScene.h"        // ����Ϸ����
#include "Scenes/SaveSelectScene.h"    // �浵ѡ�����
#include "cocos2d.h"
#include "Scenes/CreateErrorScene.h"
#include "App/SceneRouter.h"
#include "App/SceneUIFacade.h"
USING_NS_CC;

Scene* SaveSelectScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SaveSelectScene::create();
    scene->addChild(layer);
    return scene;
}

bool SaveSelectScene::init()
{
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for SaaveSelectScene!!");
        return false;
    }

    const auto screenSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();


    if (!SceneUIFacade::getInstance()->applyBackground(this, "../Resources/LooseSprites-73/stardewPanorama.png")) {
        CCLOG("Failed  to load Sava Select background image!");
        throw std::runtime_error("Failed to load SaveSelectScene Image!!");
        return false;
    }

   

    // 2. 添加返回按钮
    auto backButton = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/textBox..png",
        "Back",
        "../Resources/fonts/Marker Felt.ttf",
        24,
        screenSize,
        0.12f,
        0.08f,
        Vec2(screenSize.width * 0.5f, screenSize.height * 0.8f),
        [](Ref* sender) {
            try {
                SceneRouter::getInstance()->goTo("Menu");
            }
            catch (const std::exception& e) {
                CCLOG("Exception caught : %s", e.what());
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
    );


    // 3. 添加存档1按钮（示例）
    auto saveSlot1 = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/textBox..png",
        "Saving1",
        "../Resources/fonts/Marker Felt.ttf",
        24,
        screenSize,
        0.12f,
        0.08f,
        Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f),
        [](Ref* sender) {
            try {
                CCLOG("Load Save Slot 1...");
            }
            catch (const std::exception& e) {
                CCLOG("Exception caught : %s", e.what());
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
    );


  // 3. 添加存档2按钮（示例）
    auto saveSlot2 = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/textBox..png",
        "Saving2",
        "../Resources/fonts/Marker Felt.ttf",
        24,
        screenSize,
        0.12f,
        0.08f,
        Vec2(screenSize.width * 0.5f, screenSize.height * 0.2f),
        [](Ref* sender) {
            try {
                CCLOG("Load Save Slot 1...");
            }
            catch (const std::exception& e) {
                CCLOG("Exception caught : %s", e.what());
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
    );



    auto menu = Menu::create(backButton, saveSlot1, saveSlot2, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}
