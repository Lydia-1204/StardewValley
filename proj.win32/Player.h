/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.h
 * File Function: Player�������
 * Author:        ������ 2351273
 * Update Date:   2024/12/16
 *********************************************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "UIManager.h"  // ���� UIManager ͷ�ļ�
#include <string>
#include <set>

USING_NS_CC;

class Player : public Node {
private:
    UIManager* uiManager;  // ���� UIManager ʵ����ָ��

    Sprite* _toolSprite;             // ��ͷ���߾���


    Sprite* _fishing_rodSprite;      // ��͹��߾���
    Sprite* _watering_canSprite;     // ˮ������
    Sprite* _scytheSprite;           // ��������
    Sprite* _hoeSprite;              // ��ͷ���߾���

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

public:
    Player(UIManager* ui) : uiManager(ui) {}  // ���캯����ʼ�� uiManager
    Sprite* _sprite[4];              // ��ҽ�ɫ�ľ��� �ĸ��������Һ���
    static Player* create(int selectedCharacter, const std::string& nickname);

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

    // ��ȡ�ǳ�
    const std::string& getNickname() const;

    // ��ȡѡ��Ľ�ɫ
    int getSelectedCharacter() const;
    void setPlayerPosition(cocos2d::Vec2& position);
    Vec2 getPlayerPosition() const;       // ����Player��ǰλ��
    int _currentDirection = 0;       // ��¼��ʱ��Ҿ�����Եķ���
};

#endif // PLAYER_H