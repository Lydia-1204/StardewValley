
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LoadingScene.cpp
 * File Function: 实现LoadingScene类，完成游戏加载界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/4
 ****************************************************************/
#include "Scenes/LoadingScene.h"
#include "ui/CocosGUI.h"
#include "App/Constant.h"
#include "App/AudioService.h"
#include "Scenes/CreateErrorScene.h"
#include "Scenes/MenuScene.h"
#include "App/SceneRouter.h"
#include "App/SceneUIFacade.h"
USING_NS_CC;
using namespace cocos2d::experimental;
// 创建场景
Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

// 初始化场景
bool LoadingScene::init()
{
    //播放音乐
    AudioService::getInstance()->playBgm("../Resources/Stardew Valley Overture.mp3", true, 0.7f);
    
    // 创建场景
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for LoadingScene!!");
        return false;
    }

    // 获取屏幕尺寸 
    const auto screen_Size = cocos2d::Director::getInstance()->getVisibleSize();
   // 加载背景
    if (!SceneUIFacade::getInstance()->applyBackground(this, "../Resources/LoadingScene.png")) {
        CCLOG("Failed  to load  LoadingScene background image!");
        throw std::runtime_error("Failed to load LoadingScene Image!!");
    }

    // 创建ui进度条
    auto LoadingProgressBar = cocos2d::ui::LoadingBar::create("../Resources/LoadingBar.png");
    if(LoadingProgressBar==NULL)
        throw std::runtime_error("Failed to generate LoadingProgressBar !!");
    LoadingProgressBar->setPosition(Vec2(screen_Size.width / 2, screen_Size.height / 2 + LOADING_SCENE_LOADINGBAR_OFFSET_Y));
    LoadingProgressBar->setPercent(0);
    this->addChild(LoadingProgressBar);

    // 创建标签以显示进度
    auto progressLabel = Label::createWithTTF("0%", "../Resources/fonts/Marker Felt.ttf", LOADING_SCENE_FONT_SIZE);
    if (progressLabel == NULL)
        throw std::runtime_error("Failed to generate  progressLabel !!");
    progressLabel->setPosition(Vec2(LoadingProgressBar->getPosition().x - LoadingProgressBar->getContentSize().width / 2 + LOADING_SCENE_LOADING_LABEL_OFFSET_X, LoadingProgressBar->getPosition().y));
    progressLabel->setVisible(false);
    this->addChild(progressLabel);

    

   // 更新进度条和标签
   constexpr float intervalTime = LOADING_SCENE_DURATION_TIME / 100.0f; // 每 1% 所需时间间隔
   for (int i = 0; i <= 100; i++) {
       this->scheduleOnce([LoadingProgressBar, progressLabel, i](float dt) {
           progressLabel->setVisible(i >= 10);
           LoadingProgressBar->setPercent(i);
           progressLabel->setString(std::to_string(i) + "%");
           float percentage = LoadingProgressBar->getPercent() / 100.0f;
           float x_Position = LoadingProgressBar->getPosition().x - LoadingProgressBar->getContentSize().width / 2 + LoadingProgressBar->getContentSize().width * percentage + LOADING_SCENE_LOADING_LABEL_OFFSET_X;
           progressLabel->setPosition(Vec2(x_Position, LoadingProgressBar->getPosition().y));
           }, intervalTime * i, "UpdateLoadingSceneLoadingBar" + std::to_string(i));
   }

   //设置计时器
   this->scheduleOnce([](float dt) {
       try {
           SceneRouter::getInstance()->goTo("Menu");
       } catch(const std::exception& e) {
           CCLOG("Exception caught : %s", e.what());
           SceneRouter::getInstance()->goToError(e.what());
       }
       }, LOADING_SCENE_DURATION_TIME + SCENE_TRANSITION_DURATION, "StartupSceneToMenuScene");

    return true;
}
