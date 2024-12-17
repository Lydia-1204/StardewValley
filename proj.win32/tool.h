/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.h
 * File Function: 实现tool类与toolManager声明，实现工具的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#pragma once
#ifndef TOOL_H
#define TOOL_H

#include "map.h"
#include "cocos2d.h"
#include"player.h"
class Tool : public cocos2d::Sprite {
public:
    enum class ToolType {
        NONE,
        HOE, //锄头
        AXE, //斧头
        WATERING_CAN, //水壶
        FISHING_ROD, //鱼竿
        // 添加其他工具类型
    };
    friend class player;
    static Tool* create(ToolType type);
    bool init(ToolType type);
    ToolType getType() const;
    void usetool();//个性化使用
    int selectedCharacter;
    std::string nickname;
private:
    ToolType type; // 工具类型
};

class ToolManager : public cocos2d::Node {
public:
   
    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class player;
    void addTool(Tool::ToolType type); // 添加工具到工具栏
    void selectTool(int index);        // 选中工具
    void useTool();                    // 使用选中的工具
    void discardTool();                // 丢弃选中的工具
    int selectedCharacter;
    std::string nickname;
    
    static ToolManager* getInstance(int selectedCharacter, const std::string& nickname);
   
private:

    static ToolManager* instance;
    Player* player;
    std::vector<Tool*> tools;         // 工具栏中的工具
    int selectedToolIndex;            // 当前选中的工具索引
    cocos2d::Sprite* selectionBox;    // 选中工具的高亮框
    void updateSelectionBox();        // 更新选中框位置
    void initKeyboardListener();
};

#endif // TOOL_H