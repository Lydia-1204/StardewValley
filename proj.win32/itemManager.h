/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ItemManager.h
 * File Function: 实现ItemManager类声明，实现物品的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/17
 *********************************************************************************************************/
#pragma once
#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H

#include "map.h"
#include "cocos2d.h"
#include"player.h"
#include"Item.h"

class ItemManager : public cocos2d::Node {
public:

    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class player;
    void addItem(Item::ItemType type); // 添加物品到物品栏
    void selectItem(int index);        // 选中物品
    void useitem();                    // 使用选中的物品
    void discardItem();                // 丢弃选中的物品 数量>0时数字减少
    int selectedCharacter;
    std::string nickname;

    static ItemManager* getInstance(int selectedCharacter, const std::string& nickname);

private:

    static ItemManager* instance;
    Player* player;
    std::vector<Item*> Items;         // 物品栏中的物品
    int selectedItemIndex;            // 当前选中的物品索引
    cocos2d::Sprite* selectionBox;    // 选中物品的高亮框
    void updateSelectionBox();        // 更新选中框位置
    void initKeyboardListener();
};

#endif // ITEMMANAGER_H