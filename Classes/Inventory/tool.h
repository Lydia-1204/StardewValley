/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.h
 * File Function: 实现tool类声明，实现工具的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#pragma once
#ifndef TOOL_H
#define TOOL_H
#include "cocos2d.h"

class Tool : public cocos2d::Sprite
{
public:
    friend class Chest;
    enum class ToolType
    {
        NONE,
        HOE,          // 锄头
        AXE,          // 斧头
        WATERING_CAN, // 水壶
        FISHING_ROD,  // 鱼竿
        FERTILIZER,   // 化肥

        // 升级版*******************//
        HOEPLUS,          // 锄头
        AXEPLUS,          // 斧头
        WATERING_CANPLUS, // 水壶
        FISHING_RODPLUS,  // 鱼竿
        ANIMALFOOD,       // 喂食
    };

    friend class Player;
    friend class ToolManager;
    friend class ToolFactory;

    static Tool *create(ToolType type);
    bool init(ToolType type);

    // 新增：对象池重置方法
    void reset();

    ToolType getType() const;
    void usetool(); // 个性化使用
    void buytool();

    int selectedCharacter;
    std::string nickname;
    int price;

private:
    ToolType type; // 工具类型
    // 私有构造函数，支持工厂模式
    Tool();
};

#endif // TOOL_H