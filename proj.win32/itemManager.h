/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ItemManager.h
 * File Function: ʵ��ItemManager��������ʵ����Ʒ�Ĺ��� ����
 * Author:        ��С�� 2351882
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
    void addItem(Item::ItemType type); // �����Ʒ����Ʒ��
    void selectItem(int index);        // ѡ����Ʒ
    void useitem();                    // ʹ��ѡ�е���Ʒ
    void discardItem();                // ����ѡ�е���Ʒ ����>0ʱ���ּ���
    int selectedCharacter;
    std::string nickname;

    static ItemManager* getInstance(int selectedCharacter, const std::string& nickname);

private:

    static ItemManager* instance;
    Player* player;
    std::vector<Item*> Items;         // ��Ʒ���е���Ʒ
    int selectedItemIndex;            // ��ǰѡ�е���Ʒ����
    cocos2d::Sprite* selectionBox;    // ѡ����Ʒ�ĸ�����
    void updateSelectionBox();        // ����ѡ�п�λ��
    void initKeyboardListener();
};

#endif // ITEMMANAGER_H