/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: 声明UIManager类，用于实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "cocos2d.h"

USING_NS_CC;

class UIManager : public Node {
private:
    static UIManager* instance;         // 单例实例

    Layer* uiLayer;                     // 用于存放 UI 元素的层
    Label* energyLabel;                 // 精力条显示
    Label* clockLabel;                  // 时钟显示
    Label* playerNameLabel;             // 玩家昵称显示
    Menu* toolBar;                      // 工具栏菜单

    UIManager();                        // 构造函数私有化

    void setupUIElements();             // 初始化 UI 元素
    void handleShortcuts(EventKeyboard::KeyCode keyCode, Event* event); // 快捷键处理

public:
    static UIManager* getInstance();    // 获取单例实例

    Layer* getLayer();                  // 获取 UI 层
    void update(float dt);              // 每帧更新 UI

    void togglePauseMenu();             // 切换暂停菜单
    void openMiniMap();                 // 打开缩略地图
    void showPlayerStats();             // 显示玩家状态
};

#endif // UI_MANAGER_H