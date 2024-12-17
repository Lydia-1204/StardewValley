/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    Item.h
 * File Function: ʵ��Item��������ʵ����Ʒ�ĵĴ���ʹ��
 * Author:        ��С�� 2351882
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
        SEED, //����
        FISH, //����
        EGG, //����
        MILK, //ţ��
        BONE,//��ͷ
        WOOL,//��ë
        // ���������������
    };
    friend class player;
    static Item* create(ItemType type);

    bool init(ItemType type);
    ItemType getType() const;   //�������
    void useitem();//���Ի�ʹ��
   
    void increaseQuantity(int amout); //����amout������ ����������ui
    void decreaseQuantity(int amout);//����amout������  ����������ui
    int getQuantity()const;    //��ô���Ʒ������
    void updateItemUI();      //��������ui
   
    int selectedCharacter;
    std::string nickname;
private:
    ItemType type; // ��������
    int quantity;//����
    Label* quantityLabel; //���ֱ�ǩ
};


#endif // ITEM_H
