/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: ����GameScene�࣬����ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#pragma once
#ifndef SYSTEM_H
#define SYSTEM_H

#include "cocos2d.h"

USING_NS_CC;

class System {
private:
    static System* instance; // ����ʵ��

    System();                // ���캯��˽�л�
    ~System();               // ��������˽�л�

    void initialize();       // ϵͳ��ʼ��
    void cleanup();          // ϵͳ����

public:
    static System* getInstance(); // ��ȡ����ʵ��

    void run();                  // ������Ϸ
    void exitGame();             // �˳���Ϸ
};

#endif // SYSTEM_H
