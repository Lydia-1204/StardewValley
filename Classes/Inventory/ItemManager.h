/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    ItemManager.h
 * File Function: 实现ItemManager类，使用对象池模式管理物品，配合工厂模式创建物品
 * Author:       王小萌 2351882
 * Update Date:  2024/12/21
 *********************************************************************************************************/
#pragma once
#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

// 确保 cocos2d 头文件在最前面
#include "cocos2d.h"
#include "2d/CCNode.h"

// 然后是项目头文件
#include "Inventory/Item.h"
#include "Inventory/ItemFactory.h"
#include <vector>
#include <unordered_map>

USING_NS_CC;

class ItemManager : public Node
{
public:
    friend class Chest;
    friend class Player;
    friend class GameScene;

    // 单例模式获取实例
    static ItemManager *getInstance(int selectedCharacter = 1, const std::string &nickname = "guest");

    // 物品管理方法
    void addItem(Item::ItemType type); // 添加物品（使用对象池）
    void removeItem(Item *item);       // 移除物品（回收至对象池）
    void recycleItem(Item *item);      // 回收物品到对象池

    // 选择和使用物品
    void selectItem(int index); // 选中物品
    void useitem();             // 使用选中的物品
    void discardItem();         // 丢弃选中的物品

    // 获取方法
    Item *getItem(int index);                 // 获取指定索引的物品
    int getItemCount() const;                 // 获取物品总数
    int getItemQuantity(Item::ItemType type); // 获取指定类型物品数量

    // 对象池管理
    void preloadPool();                                  // 预加载对象池
    void expandPool(Item::ItemType type, int count = 5); // 扩展对象池
    void clearAllItems();                                // 清空所有物品
    size_t getPoolSize(Item::ItemType type) const;       // 获取对象池大小

    // UI更新
    void updateItemPositions(); // 更新物品位置
    void refreshUI();           // 刷新UI显示

    // 成员变量
    int selectedCharacter;
    std::string nickname;
    Chest *chest;
    int selectedItemIndex; // 当前选中的物品索引

    virtual ~ItemManager();

private:
    // 私有构造函数
    ItemManager();

    // 静态实例
    static ItemManager *_instance;

    // 对象池实现
    std::vector<Item *> availablePool; // 可用物品池
    std::vector<Item *> activePool;    // 活动物品池
    std::vector<Item *> items;         // 物品列表（保持向后兼容）

    // 池管理方法
    Item *getItemFromPool(Item::ItemType type); // 从池中获取物品
    void returnItemToPool(Item *item);          // 将物品返回池中

    // UI相关
    Player *player;
    cocos2d::Sprite *selectionBox; // 选中框
    void updateSelectionBox();     // 更新选中框位置
    void initKeyboardListener();   // 初始化键盘监听
};

#endif // ITEMMANAGER_H