
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SaveSelectScene.cpp
 * File Function: 实现SaveSelectScene类，完成存档选择界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

// 创建场景
#include "MenuScene.h"
#include "NewSelectScene.h"        // 新游戏场景
#include "SaveSelectScene.h"    // 存档选择界面
#include "cocos2d.h"
#include"CreateErrorScene.h"
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

    // 1. 添加背景图
    auto background = Sprite::create("../Resources/LooseSprites-73/stardewPanorama.png");
  
    if (background) {
    //背景尺寸
        const Size spriteSize = background->getContentSize();

    // 计算宽度和高度的缩放比例
        float scaleX = screenSize.width / spriteSize.width;
        float scaleY = screenSize.height / spriteSize.height;

    // 选择更大的缩放比例，确保图片覆盖整个屏幕
        float scale = std::max(scaleX, scaleY);
        background->setScale(scale);
        background->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));
        this->addChild(background);
    }
    else {
        CCLOG("Failed  to load Sava Select background image!");
        throw std::runtime_error("Failed to load SaveSelectScene Image!!");
        return false;
    }

   

    // 2. 添加返回按钮
    auto backButton = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            // 返回主菜单
            try {
                auto menuScene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene, Color3B::BLACK));
            }
            catch (const std::exception& e) {
                //捕获异常 记录日志
                CCLOG("Exception caught : %s", e.what());
                //加载备用错误界面
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });
  
    // 添加文字到按钮上
    auto backLabel = Label::createWithTTF("Back", "../Resources/fonts/Marker Felt.ttf", 24);
    backLabel->setPosition(backButton->getContentSize() / 2); // 设置文字居中
    backButton->addChild(backLabel); // 将文字作为按钮的子节点

    //按钮尺寸
    const auto buttonSize = backButton->getContentSize();

    // 计算宽度和高度的缩放比例
    auto scaleX = (screenSize.width / 10) / buttonSize.width*1.2;
    auto scaleY = (screenSize.height / 10) / buttonSize.height * 0.8;
    //scale = std::min(scaleX, scaleY);
    backButton->setScale(scaleX, scaleY);
    backButton->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.8f));


    // 3. 添加存档1按钮（示例）
    auto saveSlot1 = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            try {
                // 加载存档2逻辑 (假设跳转到游戏场景)
                CCLOG("Load Save Slot 1...");
            }
            catch (const std::exception& e) {
                //捕获异常 记录日志
                CCLOG("Exception caught : %s", e.what());
                //加载备用错误界面
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });
   
    auto saveLabel = Label::createWithTTF("Saving1", "../Resources/fonts/Marker Felt.ttf", 24);
    saveLabel->setPosition(saveSlot1->getContentSize() / 2); // 设置文字居中
    saveSlot1->addChild(saveLabel); // 将文字作为按钮的子节点
    saveSlot1->setScale(scaleX, scaleY);
    saveSlot1->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f));


  // 3. 添加存档2按钮（示例）
    auto saveSlot2 = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            try {
                // 加载存档2逻辑 (假设跳转到游戏场景)
                CCLOG("Load Save Slot 1...");
            }
            catch (const std::exception& e) {
                //捕获异常 记录日志
                CCLOG("Exception caught : %s", e.what());
                //加载备用错误界面
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });

    auto saveLabel2 = Label::createWithTTF("Saving2", "../Resources/fonts/Marker Felt.ttf", 24);
    saveLabel2->setPosition(saveSlot2->getContentSize() / 2); // 设置文字居中
    saveSlot2->addChild(saveLabel2); // 将文字作为按钮的子节点
    saveSlot2->setScale(scaleX, scaleY);
    saveSlot2->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.2f));



    auto menu = Menu::create(backButton, saveSlot1, saveSlot2, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}