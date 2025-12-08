/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    Chest.h
 * File Function: 实现Chest类，使用工厂模式和对象池管理箱子物品存储
 * Author:       王小萌 2351882
 * Update Date:  2024/12/21
 *********************************************************************************************************/
#pragma once
#ifndef CHEST_H
#define CHEST_H

#include "cocos2d.h"
#include "Inventory/Item.h"
#include "Inventory/ItemFactory.h"
#include "Inventory/ItemManager.h"
#include "Inventory/Tool.h"
#include <vector>
#include <unordered_map>

USING_NS_CC;

class Chest : public Node
{
public:
    friend class ItemManager;
    friend class ToolManager;
    friend class Player;
    friend class GameScene;

    // 单例模式获取实例
    static Chest *getInstance();

    // 箱子操作
    void openChest();                            // 打开箱子
    void closeChest();                           // 关闭箱子
    void chestSetPosition(const Vec2 &position); // 设置箱子位置
    Vec2 getChestPosition() const;               // 获取箱子位置

    // 物品存储管理（使用对象池）
    bool storeItem(Item::ItemType type, int quantity = 1); // 存储物品到箱子
    Item *retrieveItem(Item::ItemType type);               // 从箱子取出物品
    bool hasItem(Item::ItemType type, int quantity = 1);   // 检查箱子是否有物品
    int getItemCount(Item::ItemType type);                 // 获取物品数量

    // 工具存储管理（保持向后兼容）
    void addTool(Tool *tool);    // 添加工具到箱子
    void removeTool(Tool *tool); // 从箱子移除工具

    // 批量操作
    void storeAllItems();    // 将ItemManager中所有物品存入箱子
    void retrieveAllItems(); // 将箱子中所有物品取出到ItemManager

    // 箱子状态
    bool isOpen;        // 箱子是否打开
    void toggleChest(); // 切换箱子开关状态

    // UI管理
    void updateChestUI();     // 更新箱子UI显示
    void showChestContents(); // 显示箱子内容
    void hideChestContents(); // 隐藏箱子内容

    // 对象池管理
    void preloadChestPool();     // 预加载箱子物品池
    void clearChest();           // 清空箱子
    size_t getChestSize() const; // 获取箱子物品总数

    virtual ~Chest();

private:
    Chest(); // 私有构造函数
    static Chest *_instance;

    // 箱子存储（使用对象池模式）
    std::unordered_map<Item::ItemType, std::vector<Item *>> storedItems; // 按类型存储的物品
    std::vector<Item *> activeChestItems;                                // 当前显示的物品

    // 工具存储（保持向后兼容）
    std::vector<Tool *> chestTools;  // 箱子中的工具
    std::vector<Sprite *> toolgrids; // 工具槽位网格

    // 箱子位置和UI
    Vec2 chestPosition;
    Sprite *chestSprite;             // 箱子精灵
    LayerColor *chestPanel;          // 箱子面板
    std::vector<Sprite *> itemSlots; // 物品槽位

    // 池管理方法
    Item *getItemFromChestPool(Item::ItemType type);   // 从箱子池获取物品
    void returnItemToChestPool(Item *item);            // 将物品返回箱子池
    void initializeChestSlots();                       // 初始化物品槽位
    void initializeToolGrids();                        // 初始化工具槽位
    void updateSlotDisplay(int slotIndex, Item *item); // 更新槽位显示

    // 工厂集成
    ItemFactory *itemFactory; // 物品工厂引用
    ItemManager *itemManager; // 物品管理器引用
};

#endif // CHEST_H