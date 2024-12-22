/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.h
 * File Function: ʵ��tool����toolManager������ʵ�ֹ��ߵĹ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#pragma once
#ifndef TOOLMANAGER_H
#define TOOLMANAGER_H
#include "cocos2d.h"
#include "tool.h"
#include<vector>
class Chest;
class ToolManager : public cocos2d::Node {
public:
    friend class Chest;
    friend class Animal;
    friend class Crop;
    friend class GameScene;
    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class Player;
    void addTool(Tool::ToolType type); // ��ӹ��ߵ�������
    void selectTool(int index);        // ѡ�й���
    void useTool();                    // ʹ��ѡ�еĹ���
    void discardTool();                // ����ѡ�еĹ���
    int selectedCharacter;
    std::string nickname;
    Chest* chest;
    static ToolManager* getInstance(int selectedCharacter, const std::string& nickname);
    ToolManager() :chest() {}
private:

    static ToolManager* instance;
    Player* player;
    std::vector<Tool*> tools;         // �������еĹ���
    int selectedToolIndex;            // ��ǰѡ�еĹ�������
    cocos2d::Sprite* selectionBox;    // ѡ�й��ߵĸ�����
    void updateSelectionBox();        // ����ѡ�п�λ��
    void initKeyboardListener();
};

#endif // TOOL_H#pragma once
