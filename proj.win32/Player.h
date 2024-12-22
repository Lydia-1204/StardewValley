/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.h
 * File Function: Player�������
 * Author:        ������ 2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#define NO_TRANSPARENT 255
#define HALF_TRANSPARENT 128

#include "cocos2d.h"
#include "UIManager.h"  // ���� UIManager ͷ�ļ�
#include "tool.h"
#include "NPC.h"
#include <string>
#include <set>

USING_NS_CC;

class Player : public Node {
private:
    class UIManager* uiManager;            // ���� UIManager ʵ����ָ��
    class Tool* tool;        // ���� Tool ʵ����ָ��
    class ToolManager* toolManager;        // ���� ToolManager ʵ����ָ��
   
    Sprite* _toolSprite;             // ���߾���

    bool _shiftKeyPressed;           // ���Shift���Ƿ���
    bool _isMousePressed;            // ����Ƿ���
    Vec2 _velocity;                  // �ƶ��ٶȷ���
    int _energy;                     // ��Ҿ���ֵ
    int _currentTool;                // ��ǰ���� ID
    float _speed;                    // �ƶ��ٶ�
    bool _isMoving;                  // �Ƿ����ƶ�
    std::set<EventKeyboard::KeyCode> _keysPressed;  // ��������
    std::string _nickname;           // ����ǳ�
    int _selectedCharacter;          // ���ѡ��Ľ�ɫ��1��2��
    bool _isToolActive;              // ��Ǹ�ͷ�Ƿ��ڼ���״̬

    Player();                        // ˽�л����캯��
    virtual ~Player();

    void interactWithMap();          // ��ͼ�����߼�
    void updateEnergy(float delta);  // ���¾���
    static Player* instance;     // ����ʵ��

    Layer* menuLayer; // ���һ����Ա�������洢�˵��������
    int Planting_Skills;// ��ֲ����
    int Breeding_Skills;// ��ֳ����
    int Mining_Skills;// �ڿ���
    int Fishing_Skills;// ���㼼��
public:
    static Player* getInstance(int selectedCharacter, const std::string& nickname);
    static Player* create(int selectedCharacter, const std::string& nickname);
    Player(UIManager* ui) : uiManager(ui) {}  // ���캯����ʼ�� uiManager
    Player(Tool* _to) : tool(_to) {}
    Player(ToolManager* _toMa) : toolManager(_toMa) {}
    Sprite* _sprite[4];              // ��ҽ�ɫ�ľ��� �ĸ��������Һ���

    bool init(int selectedCharacter, const std::string& nickname);
    void setEnergy(int energy);
    int getEnergy() const;
    void setCurrentTool(int toolId);
    void move(float delta);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void update(float delta);
    void onMouseDown(Event* event);           // ��갴���¼�
    void onMouseUp(cocos2d::Event* event);    // ����ͷ��¼�
    void onMouseMove(cocos2d::Event* event); // ����ƶ��¼���Ӧ ���������npc�Ļ���
    bool isGift = 1;

    // ��ȡ�ǳ�
    const std::string& getNickname() const;

    // ��ȡѡ��Ľ�ɫ
    int getSelectedCharacter() const;
    void setPlayerPosition(cocos2d::Vec2& position);
    Vec2 getPlayerPosition() const;       // ����Player��ǰλ��
    int _currentDirection = 0;       // ��¼��ʱ��Ҿ�����Եķ���
    void updateToolSprite();
    void updateDialogPosition(NPC* npc);
    void showDialogBox(NPC* npc, const std::string& category);

    std::chrono::steady_clock::time_point lastClickTime; // ���ڼ�¼�ϴε����ʱ���
    bool wasDoubleClick = false; // ����Ƿ������˫��
    int getPlanting() const {
        return Planting_Skills;
    }
    int getBreeding() const {
        return Breeding_Skills;
    }
    int getMining() const {
        return Mining_Skills;
    }
    int getFishing() const {
        return Fishing_Skills;
    }

    void changePlanting() {
        Planting_Skills += 15;
        if (Planting_Skills >= 100)
            Planting_Skills = 100;
    }
    void changeBreeding() {
        Breeding_Skills += 15;
        if (Breeding_Skills >= 100)
            Breeding_Skills = 100;
    }
    void changeMining() {
        Mining_Skills += 15;
        if (Mining_Skills >= 100)
            Mining_Skills = 100;
    }
    void changeFishing() {
        Fishing_Skills += 15;
        if (Fishing_Skills >= 100)
            Fishing_Skills = 100;
    }
    void openPlayerMenu();
    void closePlayerMenu(Ref* sender);
};

#endif // PLAYER_H