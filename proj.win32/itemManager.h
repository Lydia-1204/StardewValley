/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ItemManager.h
 * File Function: ʵ��ItemManager��������ʵ����Ʒ�Ĺ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/20
 *********************************************************************************************************/
#pragma once
#ifndef ITEMMANAGER_H
#define ITEMMANAGER_H
#include "cocos2d.h"

#include"Player.h"
#include"item.h"
#include<vector>

class ItemManager : public cocos2d::Node {
public:

    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class player;
    friend class GameScene;
    void addItem(Item::ItemType type); // �����Ʒ����Ʒ��
    void selectItem(int index);        // ѡ����Ʒ
    void useitem();                    // ʹ��ѡ�е���Ʒ
    void discardItem();                // ����ѡ�е���Ʒ ����>0ʱ���ּ���
    int selectedCharacter;
    std::string nickname;
    Chest* chest;
    static ItemManager* getInstance(int selectedCharacter, const std::string& nickname);
    int selectedItemIndex;            // ��ǰѡ�е���Ʒ����
    int getItemQuantity(Item::ItemType type);

private:
    friend class Chest;
    static ItemManager* instance;
    Player* player;
    std::vector<Item*> Items;         // ��Ʒ���е���Ʒ
    cocos2d::Sprite* selectionBox;    // ѡ����Ʒ�ĸ�����
    void updateSelectionBox();        // ����ѡ�п�λ��
    void initKeyboardListener();
};

#endif // ITEMMANAGER_H