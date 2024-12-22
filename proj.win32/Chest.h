/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    item.h
 * File Function: ʵ��Chest��������ʵ�ֹ�����ĵĴ���ʹ��
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#pragma once
#ifndef CHEST_H
#define CHEST_H

#include "cocos2d.h"
#include"map.h"

class Tool;
class Item;
class Chest : public cocos2d::Node {
public:
    bool isOpen;  // �����Ƿ��
    std::vector<Item*> chestItems;  // ��Ʒ����Ʒ
    std::vector<Tool*> chestTools;  // ����������
    friend class toolManager;
    friend class itemManager;
    // ������
    void openChest();

    // �ر�����
   // void closeChest();
    void closeChest(Ref* sender);
    // ��ʾ�����е���Ʒ
    void showItemsAndTools();

    // �����������Ʒ�򹤾�
    void addItem(Item* item);
    void addTool(Tool* tool);

    //������Ʒ�򹤾�
    //void deleteItem(Item* item);
   // void deleteTool(Tool* tool);

     
    // �������ӵĳ�ʼλ��
   void chestSetPosition(const cocos2d::Vec2& position);

    //static Chest* create();


    // �����Ʒ�򹤾ߵĴ���
    void onItemClicked(Item* item);
    void onToolClicked(Tool* tool);
    void initMouseListener();
    static Chest* getInstance();
private:
    void setupItemsAndTools();  // ��ʼ�������е���Ʒ�͹���
    cocos2d::Sprite* itemgrids[4][7];
    cocos2d::Sprite* toolgrids[4][7];
    // ��ʾ��Ʒ���͹�������UI
    cocos2d::Sprite* itemBarBg;
    cocos2d::Sprite* toolBarBg;
    Label* itemLabel;
    Label* toolLabel;
    static Chest* instance;
};

#endif // CHEST_H