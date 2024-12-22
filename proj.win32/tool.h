/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.h
 * File Function: ʵ��tool��������ʵ�ֹ��ߵĹ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#pragma once
#ifndef TOOL_H
#define TOOL_H
#include "cocos2d.h"


class Tool : public cocos2d::Sprite {
public:
    friend class Chest;
    enum class ToolType {
        NONE,
        HOE, //��ͷ
        AXE, //��ͷ
        WATERING_CAN, //ˮ��
        FISHING_ROD, //���
        FERTILIZER,//����


        // ������*******************//
        HOEPLUS, //��ͷ
        AXEPLUS, //��ͷ
        WATERING_CANPLUS, //ˮ��
        FISHING_RODPLUS, //���
        ANIMALFOOD,//ιʳ



    };
    friend class Player;
    static Tool* create(ToolType type);
    bool init(ToolType type);
    ToolType getType() const;
    void usetool();//���Ի�ʹ��
    void buytool();
    int selectedCharacter;
    std::string nickname;
    int getPrice() {

        return price;
    }
private:
    ToolType type; // ��������
    int price;
};


#endif // TOOL_H