/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: ����UIManager�࣬����ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class UIManager : public Node {
private:
    static UIManager* instance;

    // UIԪ��
    Label* dateLabel;                  // ��ʾ����
    Label* timeLabel;                  // ��ʾʱ��
    Label* moneyLabel;                 // ��ʾũ���ʽ�
    ProgressTimer* energyBar;          // ��ʾ������
    Sprite* iron;                      // ����ͼ��
    Label* shortcutKeysLabel;          // ��ʾ��ݼ���ʾ

    // ���ݳ�Ա
    std::string nickname;              // ����ǳ�
    int selectedCharacter;             // ѡ��Ľ�ɫ
    int money;                         // ��ǰũ���ʽ�

    int currentMonth;                  // ��ǰ�·�
    int currentDay;                    // ��ǰ����
    int currentWeekday;                // ��ǰ�ܼ���0=���գ�1=��һ��...��6=������
    int currentHour;                   // ��ǰСʱ
    int currentMinute;                 // ��ǰ����
    int currentEnergy;                 // ��ǰ����ֵ���ٷֱȣ�

    float timeElapsed;                 // �����ۻ�ʱ�䣨�룩

    UIManager();                       // ˽�л����캯��

    // ˽�з���
    void updateDateAndTime();          // �������ں�ʱ���ǩ
    int getDaysInMonth(int month);     // ��ȡָ���·ݵ�����

public:
    static UIManager* getInstance(int selectedCharacter, const std::string& nickname);

    bool init(int selectedCharacter, const std::string& nickname); // ��ʼ��

    void update(float delta);           // ÿ֡����

    // ���� UI ��ʾ����
    void setDateAndTime(const std::string& date, const std::string& time);

    void setMoney(int money);
    void setEnergy(int energy);

    UIManager* getLayer();                  // ��ȡ UI ��
    void setToolBar(const std::vector<Sprite*>& tools); // ���ù�����
};

#endif // UI_MANAGER_H