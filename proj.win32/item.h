/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    Item.h
 * File Function: 实现Item类声明，实现物品的的创造使用
 * Author:        王小萌 2351882
 * Update Date:   2024/12/17
 *********************************************************************************************************/
#pragma once
#ifndef ITEM_H
#define ITEM_H

#include "map.h"
#include "cocos2d.h"
#include"player.h"
class Item : public cocos2d::Sprite {
public:
    enum class ItemType {
        NONE,
        SEED, //种子
        FISH, //鱼肉
        EGG, //鸡蛋
        MILK, //牛奶
        BONE,//骨头
        WOOL,//羊毛
        // 添加其他工具类型
    };
    friend class player;
    static Item* create(ItemType type);

    bool init(ItemType type);
    ItemType getType() const;   //获得类型
    void useitem();//个性化使用
   
    void increaseQuantity(int amout); //增加amout个数量 并更新数字ui
    void decreaseQuantity(int amout);//减少amout个数量  并更新数字ui
    int getQuantity()const;    //获得此物品的数字
    void updateItemUI();      //更新数字ui
   
    int selectedCharacter;
    std::string nickname;
private:
    ItemType type; // 工具类型
    int quantity;//数量
    Label* quantityLabel; //数字标签
};


#endif // ITEM_H
