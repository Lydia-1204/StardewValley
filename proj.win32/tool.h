/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.h
 * File Function: ʵ��tool����toolManager������ʵ�ֹ��ߵĹ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#pragma once
#ifndef TOOL_H
#define TOOL_H

#include "map.h"
#include "cocos2d.h"
#include"player.h"
class Tool : public cocos2d::Sprite {
public:
    enum class ToolType {
        NONE,
        HOE, //��ͷ
        AXE, //��ͷ
        WATERING_CAN, //ˮ��
        FISHING_ROD, //���
        // ���������������
    };
    friend class player;
    static Tool* create(ToolType type);
    bool init(ToolType type);
    ToolType getType() const;
    void usetool();//���Ի�ʹ��
    int selectedCharacter;
    std::string nickname;
private:
    ToolType type; // ��������
};

class ToolManager : public cocos2d::Node {
public:
   
    virtual bool init(int selectedCharacter, const std::string& nickname);
    friend class player;
    void addTool(Tool::ToolType type); // ��ӹ��ߵ�������
    void selectTool(int index);        // ѡ�й���
    void useTool();                    // ʹ��ѡ�еĹ���
    void discardTool();                // ����ѡ�еĹ���
    int selectedCharacter;
    std::string nickname;
    
    static ToolManager* getInstance(int selectedCharacter, const std::string& nickname);
   
private:

    static ToolManager* instance;
    Player* player;
    std::vector<Tool*> tools;         // �������еĹ���
    int selectedToolIndex;            // ��ǰѡ�еĹ�������
    cocos2d::Sprite* selectionBox;    // ѡ�й��ߵĸ�����
    void updateSelectionBox();        // ����ѡ�п�λ��
    void initKeyboardListener();
};

#endif // TOOL_H