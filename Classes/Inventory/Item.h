// --- START OF FILE Inventory/Item.h ---
#pragma once
#ifndef ITEM_H
#define ITEM_H
#include "cocos2d.h"
#include "Characters/Player.h"

class GameScene;
class ItemFactory; // 前向声明

class Item : public cocos2d::Sprite
{
public:
    friend class Chest;
    friend class ItemFactory; // 允许工厂访问私有成员

    enum class ItemType
    {
        NONE,
        SEED,    // 种子
        FISH,    // 鱼肉
        EGG,     // 鸡蛋
        MILK,    // 牛奶
        BONE,    // 骨头
        WOOL,    // 羊毛
        WOODEN,  // 木头
        FRUIT,   // 南瓜
        MINERAL, // 矿石
        GIFT,    // 礼物
        FAT,     // 油脂
    };

    friend class player;
    friend class ItemManager;

    static Item *create(ItemType type);

    bool init(ItemType type);

    // 对象池模式：重置物品状态
    void reset(ItemType type);

    ItemType getType() const; // 获得类型
    void useitem();           // 个性化使用

    void increaseQuantity(int amout); // 增加amout个数量 并更新数字ui
    void decreaseQuantity(int amout); // 减少amout个数量  并更新数字ui
    int getQuantity() const;          // 获得此物品的数字
    void updateItemUI();              // 更新数字ui

    int selectedCharacter;
    std::string nickname;
    int price;

private:
    ItemType type; // 物品类型
    int quantity;  // 数量
    int getPrice();
    cocos2d::Label *quantityLabel; // 数字标签
    void displayItemAbovePlayer(Player *player, const std::string &itemImagePath);
    cocos2d::Sprite *itemSprite;
};

#endif // ITEM_H