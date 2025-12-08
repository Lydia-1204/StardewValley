/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     toolManager.h
 * File Function: 实现toolManager类，使用对象池模式管理工具
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#pragma once
#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H
#include "cocos2d.h"
#include "Inventory/Tool.h"
#include "Inventory/ToolFactory.h"
#include "Characters/Player.h"
#include <vector>
#include <unordered_map>

USING_NS_CC;

class Chest;
class ToolManager : public cocos2d::Node
{
public:
    friend class Chest;
    friend class Animal;
    friend class Crop;
    friend class GameScene;
    friend class Player;

    virtual bool init(int selectedCharacter, const std::string &nickname);

    void addTool(Tool::ToolType type); // 添加工具（使用对象池）
    void selectTool(int index);        // 选中工具
    void useTool();                    // 使用选中的工具
    void discardTool();                // 丢弃选中的工具

    // 对象池管理
    void recycleTool(Tool *tool); // 回收工具到对象池
    void preloadToolPool();       // 预加载工具池
    void clearAllTools();         // 清空所有工具

    int selectedCharacter;
    std::string nickname;
    Chest *chest;
    static ToolManager *getInstance(int selectedCharacter, const std::string &nickname);

    std::vector<Tool *> tools; // 工具列表
    int selectedToolIndex;

    virtual ~ToolManager();

private:
    ToolManager();
    static ToolManager *instance;
    Player *player;
    cocos2d::Sprite *selectionBox; // 选中工具的高亮框

    // 对象池实现
    std::vector<Tool *> availablePool; // 可用工具池
    std::vector<Tool *> activePool;    // 活动工具池

    // 池管理方法
    Tool *getToolFromPool(Tool::ToolType type);              // 从池中获取工具
    void returnToolToPool(Tool *tool);                       // 将工具返回池中
    void expandToolPool(Tool::ToolType type, int count = 3); // 扩展工具池

    void updateSelectionBox(); // 更新选中框位置
    void initKeyboardListener();
};

#endif // TOOLMANAGER_H