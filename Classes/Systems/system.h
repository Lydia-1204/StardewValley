/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: 声明GameScene类，用于实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include "cocos2d.h"

USING_NS_CC;

class System {
private:
    static System* instance; // 单例实例

    System();                // 构造函数私有化
    ~System();               // 析构函数私有化

    void initialize();       // 系统初始化
    void cleanup();          // 系统清理

public:
    static System* getInstance(); // 获取单例实例

    void run();                  // 启动游戏
    void exitGame();             // 退出游戏
};

#endif // SYSTEM_H
