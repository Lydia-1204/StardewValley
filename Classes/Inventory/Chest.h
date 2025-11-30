/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    item.h
 * File Function: 实现Chest类声明，实现工具箱的的创造使用
 * Author:        王小萌 2351882
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#pragma once
#ifndef CHEST_H
#define CHEST_H

#include "cocos2d.h"
#include "World/map.h"

class Tool;
class Item;
class Chest : public cocos2d::Node {
public:
    bool isOpen;  // 箱子是否打开
    std::vector<Item*> chestItems;  // 物品栏物品
    std::vector<Tool*> chestTools;  // 工具栏工具
    friend class toolManager;
    friend class itemManager;
    // 打开箱子
    void openChest();

    // 关闭箱子
   // void closeChest();
    void closeChest(Ref* sender);
    // 显示箱子中的物品
    void showItemsAndTools();

    // 向箱子添加物品或工具
    void addItem(Item* item);
    void addTool(Tool* tool);

    //减少物品或工具
    //void deleteItem(Item* item);
   // void deleteTool(Tool* tool);

     
    // 设置箱子的初始位置
   void chestSetPosition(const cocos2d::Vec2& position);

    //static Chest* create();


    // 点击物品或工具的处理
    void onItemClicked(Item* item);
    void onToolClicked(Tool* tool);
    void initMouseListener();
    static Chest* getInstance();
private:
    void setupItemsAndTools();  // 初始化箱子中的物品和工具
    cocos2d::Sprite* itemgrids[4][7];
    cocos2d::Sprite* toolgrids[4][7];
    // 显示物品栏和工具栏的UI
    cocos2d::Sprite* itemBarBg;
    cocos2d::Sprite* toolBarBg;
    Label* itemLabel;
    Label* toolLabel;
    static Chest* instance;
};

#endif // CHEST_H
