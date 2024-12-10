/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SysttemScene.cpp
 * File Function: 实现UIManager类，展示游戏主界面和基本功能：实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#include "UIManager.h"
#include "GameScene.h"

UIManager* UIManager::instance = nullptr;

UIManager::UIManager() : uiLayer(nullptr), energyLabel(nullptr), clockLabel(nullptr), playerNameLabel(nullptr), toolBar(nullptr) {
    setupUIElements();

    // 注册键盘监听器
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(UIManager::handleShortcuts, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

UIManager* UIManager::getInstance() {
    if (!instance) {
        instance = new (std::nothrow) UIManager();
    }
    return instance;
}

Layer* UIManager::getLayer() {
    return uiLayer;
}

void UIManager::setupUIElements() {
    uiLayer = Layer::create();

    // 初始化玩家昵称显示
    playerNameLabel = Label::createWithTTF("Player: Xiao Yuan", "fonts/Marker Felt.ttf", 18);
    playerNameLabel->setPosition(Vec2(100, 680));
    uiLayer->addChild(playerNameLabel);

    // 初始化时钟显示
    clockLabel = Label::createWithTTF("Day 1, 08:00", "fonts/Marker Felt.ttf", 18);
    clockLabel->setPosition(Vec2(1100, 680));
    uiLayer->addChild(clockLabel);

    // 初始化精力条显示
    energyLabel = Label::createWithTTF("Energy: 100%", "fonts/Marker Felt.ttf", 18);
    energyLabel->setPosition(Vec2(100, 650));
    uiLayer->addChild(energyLabel);

    // 初始化工具栏
    auto toolBarLabel = Label::createWithTTF("Toolbar (1-5)", "fonts/Marker Felt.ttf", 18);
    toolBarLabel->setPosition(Vec2(640, 40));
    uiLayer->addChild(toolBarLabel);
}

void UIManager::handleShortcuts(EventKeyboard::KeyCode keyCode, Event* event) {
    auto director = Director::getInstance();
    auto currentScene = director->getRunningScene();

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_E: // E 或 ESC 打开暂停菜单
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        togglePauseMenu();
        break;

    case EventKeyboard::KeyCode::KEY_M: // M 打开缩略地图
        openMiniMap();
        break;

    case EventKeyboard::KeyCode::KEY_F: // F 打开人物状态
        showPlayerStats();
        break;

    default:
        log("Unhandled key: %d", static_cast<int>(keyCode));
        break;
    }
}

void UIManager::update(float dt) {
    // 更新时钟
    // 例如：假设每秒钟游戏时间过一分钟
    static int gameMinutes = 0;
    gameMinutes += static_cast<int>(dt * 60);
    int hours = (gameMinutes / 60) % 24;
    int minutes = gameMinutes % 60;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Day 1, %02d:%02d", hours, minutes);
    clockLabel->setString(buffer);

    // 更新精力条等其他 UI
}

void UIManager::togglePauseMenu() {
    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume();
        log("Game Resumed");
    }
    else {
        Director::getInstance()->pause();
        log("Game Paused");
    }
}

void UIManager::openMiniMap() {
    log("Opening Mini-Map...");
    // 添加打开缩略地图的逻辑
}

void UIManager::showPlayerStats() {
    log("Showing Player Stats...");
    // 添加显示玩家状态的逻辑
}