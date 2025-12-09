// --- START OF FILE Inventory/ItemFactory.h ---
#pragma once
#ifndef ITEMFACTORY_H
#define ITEMFACTORY_H

#include "cocos2d.h"
#include "Inventory/Item.h"
#include <map>
#include <string>

// 定义物品的基本数据结构
struct ItemData
{
    std::string texturePath;
    int price;
};

class ItemFactory
{
public:
    static ItemFactory *getInstance();

    // 创建一个新的 Item 对象
    Item *createItem(Item::ItemType type);

    // 配置现有的 Item 对象 (用于初始化或重置)
    void configureItem(Item *item, Item::ItemType type);

private:
    ItemFactory(); // 私有构造函数
    static ItemFactory *instance;
    std::map<Item::ItemType, ItemData> itemRegistry; // 注册表，代替 switch-case

    void initRegistry(); // 初始化注册表
};

#endif // ITEMFACTORY_H