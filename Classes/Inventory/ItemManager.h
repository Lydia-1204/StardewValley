// --- START OF FILE Inventory/ItemManager.h ---
#pragma once
#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H
#include "cocos2d.h"

#include "Characters/Player.h"
#include "Inventory/Item.h"
#include <vector>

class ItemManager : public cocos2d::Node
{
public:
    virtual bool init(int selectedCharacter, const std::string &nickname);
    friend class player;
    friend class GameScene;

    void addItem(Item::ItemType type); // 添加物品到物品栏
    void selectItem(int index);        // 选中物品
    void useitem();                    // 使用选中的物品
    void discardItem();                // 丢弃选中的物品 (现在会将物品回收到池中)

    int selectedCharacter;
    std::string nickname;
    Chest *chest;
    static ItemManager *getInstance(int selectedCharacter, const std::string &nickname);
    int selectedItemIndex; // 当前选中的物品索引
    int getItemQuantity(Item::ItemType type);

private:
    friend class Chest;
    static ItemManager *instance;
    Player *player;
    std::vector<Item *> Items; // 物品栏中的物品

    /* --------------------------------------------------------------------------
     * 【使用对象池模式重构】池容器定义
     * 说明：用于存储被回收的、暂时不用的 Item 对象，等待复用。
     * 避免频繁调用 new/delete，提升游戏运行时的帧率稳定性。
     * -------------------------------------------------------------------------- */
    std::vector<Item *> itemPool;             // 闲置的物品池
    Item *getPooledItem(Item::ItemType type); // 从池中获取或新建
    void returnItemToPool(Item *item);        // 将物品归还到池中
    // -----------------

    cocos2d::Sprite *selectionBox; // 选中物品的高亮框
    void updateSelectionBox();     // 更新选中框位置
    void initKeyboardListener();
};

#endif // ITEMMANAGER_H