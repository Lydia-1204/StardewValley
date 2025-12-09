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
#include <memory>

class IToolBehavior;
struct IToolBehaviorDeleter
{
    void operator()(IToolBehavior *ptr) const;
};
using ToolBehaviorPtr = std::unique_ptr<IToolBehavior, IToolBehaviorDeleter>;


class Tool : public cocos2d::Sprite {
public:
    friend class Chest;
    enum class ToolType {
        NONE,
        HOE, //锄头
        AXE, //斧头
        WATERING_CAN, //水壶
        FISHING_ROD, //鱼竿
        FERTILIZER,//化肥


        // 升级版*******************//
        HOEPLUS, //锄头
        AXEPLUS, //斧头
        WATERING_CANPLUS, //水壶
        FISHING_RODPLUS, //鱼竿
        ANIMALFOOD,//喂食



    };
    friend class Player;
    static Tool* create(ToolType type);
    bool init(ToolType type);
    ~Tool();
    ToolType getType() const;
    void usetool();//个性化使用
    void buytool();
    void setBehavior(ToolBehaviorPtr newBehavior);
    void setOwnerContext(int ownerSelectedCharacter, const std::string& ownerNickname);
    int selectedCharacter;
    std::string nickname;
    int getPrice() {

        return price;
    }
private:
    ToolType type; // 工具类型
    int price;
    ToolBehaviorPtr behavior;
};


#endif // TOOL_H
