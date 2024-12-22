/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    item.h
 * File Function: ʵ��Item��������ʵ����Ʒ�ĵĴ���ʹ��
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#pragma once
#ifndef ITEM_H
#define ITEM_H
#include "cocos2d.h"
#include"Player.h"

class GameScene;
class Item : public cocos2d::Sprite {
public:
    friend class Chest;
    enum class ItemType {
        NONE,
        SEED, //����
        FISH, //����
        EGG, //����
        MILK, //ţ��
        BONE,//��ͷ
        WOOL,//��ë
        WOODEN,//ľͷ
        FRUIT,//�Ϲ�
        MINERAL,//��ʯ
        GIFT,//����
        FAT,//��֬
        // ���������������
    };
    friend class player;
    friend class ItemManager;
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
    int price;
private:
    ItemType type; // ��������
    int quantity;//����
    int getPrice();
    cocos2d::Label* quantityLabel; //���ֱ�ǩ
    void displayItemAbovePlayer(Player* player, const std::string& itemImagePath);
    cocos2d::Sprite* itemSprite;
};


#endif // ITEM_H
