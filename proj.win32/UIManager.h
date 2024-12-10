/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: ����UIManager�࣬����ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#pragma once
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "cocos2d.h"

USING_NS_CC;

class UIManager : public Node {
private:
    static UIManager* instance;         // ����ʵ��

    Layer* uiLayer;                     // ���ڴ�� UI Ԫ�صĲ�
    Label* energyLabel;                 // ��������ʾ
    Label* clockLabel;                  // ʱ����ʾ
    Label* playerNameLabel;             // ����ǳ���ʾ
    Menu* toolBar;                      // �������˵�

    UIManager();                        // ���캯��˽�л�

    void setupUIElements();             // ��ʼ�� UI Ԫ��
    void handleShortcuts(EventKeyboard::KeyCode keyCode, Event* event); // ��ݼ�����

public:
    static UIManager* getInstance();    // ��ȡ����ʵ��

    Layer* getLayer();                  // ��ȡ UI ��
    void update(float dt);              // ÿ֡���� UI

    void togglePauseMenu();             // �л���ͣ�˵�
    void openMiniMap();                 // �����Ե�ͼ
    void showPlayerStats();             // ��ʾ���״̬
};

#endif // UI_MANAGER_H