/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ItemManager.h
 * File Function: 实现ItemManager类声明，实现物品的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/20
 *********************************************************************************************************/
#pragma once
#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H
#include "cocos2d.h"

#include "Characters/Player.h"
#include "Inventory/Item.h"
#include<vector>

class ItemManager : public cocos2d::Node {
public:

    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class player;
    friend class GameScene;
    void addItem(Item::ItemType type); // 添加物品到物品栏
    void selectItem(int index);        // 选中物品
    void useitem();                    // 使用选中的物品
    void discardItem();                // 丢弃选中的物品 数量>0时数字减少
    int selectedCharacter;
    std::string nickname;
    Chest* chest;
    static ItemManager* getInstance(int selectedCharacter, const std::string& nickname);
    int selectedItemIndex;            // 当前选中的物品索引
    int getItemQuantity(Item::ItemType type);

private:
    friend class Chest;
    static ItemManager* instance;
    Player* player;
    std::vector<Item*> Items;         // 物品栏中的物品
    cocos2d::Sprite* selectionBox;    // 选中物品的高亮框
    void updateSelectionBox();        // 更新选中框位置
    void initKeyboardListener();
};

#endif // ITEMMANAGER_H
