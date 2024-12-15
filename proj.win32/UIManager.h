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
#include"MenuScene.h"
#include"Constant.h"

USING_NS_CC;

class Player;  // ���� Player ��

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
    Label* energyLabel;                //������������ʾ


    cocos2d::LayerColor* pausePanel;    // ��ͣ���
    cocos2d::Sprite* miniMap;           // С��ͼ
    cocos2d::Sprite* playerMarker;           // С��ͼ��ұ��
    bool isMiniMapVisible;

    LayerColor* taskListPanel;
    std::vector<cocos2d::Label*> taskLabels;      // �����б��е������ǩ
    bool isTaskListVisible;                       // �����б���ʾ״̬

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
    friend class Player;

    static UIManager* getInstance(int& selectedCharacter, const std::string& nickname);

    // ��� getter currentenergy��������������Classes�У�
    int getCurrentEnergy() const { return currentEnergy; }

    // ��� setter �����������Ҫ���¾���ֵ��
    void setCurrentEnergy(int energy) {
        currentEnergy = energy;
        setEnergy(currentEnergy);  // ���� UI
    }

    bool init(int selectedCharacter, const std::string& nickname); // ��ʼ��

    void update(float delta);           // ÿ֡����

    // ���� UI ��ʾ����
    void setDateAndTime(const std::string& date, const std::string& time);

    void setMoney(int money);
    void setEnergy(int energy);
    void increaseEnergy(float deltaEnergy);

    UIManager* getLayer();                  // ��ȡ UI ��
    void setToolBar(const std::vector<Sprite*>& tools); // ���ù�����

    // С��ͼ����
    void toggleMiniMap(const cocos2d::Vec2& playerPos, const cocos2d::Size& mapSize); // ��ʾ/����С��ͼ
    void updateMiniMapPosition(const cocos2d::Vec2& playerPos, const cocos2d::Size& mapSize); // ���������С��ͼ��λ��

    // ������巽��
    void toggleTaskList();                        // ��ʾ/���������б�
    void updateTaskList(const std::vector<std::string>& tasks); // ���������б�
    void showPausePanel();              // ��ʾ��ͣ���
    void hidePausePanel();              // ������ͣ���
};

#endif // UI_MANAGER_H