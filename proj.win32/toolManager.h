/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.h
 * File Function: 实现tool类与toolManager声明，实现工具的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#pragma once
#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H
#include "cocos2d.h"
#include "tool.h"
#include<vector>
class Chest;
class ToolManager : public cocos2d::Node {
public:
    friend class Chest;
    friend class Animal;
    friend class Crop;
    friend class GameScene;
    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class Player;
    void addTool(Tool::ToolType type); // 添加工具到工具栏
    void selectTool(int index);        // 选中工具
    void useTool();                    // 使用选中的工具
    void discardTool();                // 丢弃选中的工具
    int selectedCharacter;
    std::string nickname;
    Chest* chest;
    static ToolManager* getInstance(int selectedCharacter, const std::string& nickname);
    ToolManager() :chest() {}
private:

    static ToolManager* instance;
    Player* player;
    std::vector<Tool*> tools;         // 工具栏中的工具
    int selectedToolIndex;            // 当前选中的工具索引
    cocos2d::Sprite* selectionBox;    // 选中工具的高亮框
    void updateSelectionBox();        // 更新选中框位置
    void initKeyboardListener();
};

#endif // TOOL_H#pragma once
