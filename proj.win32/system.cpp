/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SysttemScene.cpp
 * File Function: 实现SystemScene类，展示游戏主界面和基本功能：实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 ****************************************************************/

#include "AppDelegate.h"
#include "GameScene.h"
#include "MenuScene.h"

USING_NS_CC;


int main(int argc, char** argv) {
    // 初始化应用程序
    AppDelegate app;
    Application::getInstance()->setAnimationInterval(1.0 / 60.0); // 设置帧率

    // 初始化 Director
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    if (!glView) {
        glView = GLViewImpl::create("Farm Simulation Game");
        director->setOpenGLView(glView);
    }

    // 设置分辨率
    glView->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);

    // 加载主菜单场景
    auto mainMenuScene = MenuScene::createScene();
    director->runWithScene(mainMenuScene);

    // 启动游戏主循环
    return Application::getInstance()->run();
}