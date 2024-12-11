#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include <string>
#include <set>

USING_NS_CC;

class Player : public Node {
private:
    Sprite* _sprite;                 // ��ҽ�ɫ�ľ���
    Vec2 _velocity;                  // �ƶ��ٶȷ���
    int _energy;                     // ��Ҿ���ֵ
    int _currentTool;                // ��ǰ���� ID
    float _speed;                    // �ƶ��ٶ�
    bool _isMoving;                  // �Ƿ����ƶ�
    std::set<EventKeyboard::KeyCode> _keysPressed; // ��������
    std::string _nickname;           // ����ǳ�
    int _selectedCharacter;          // ���ѡ��Ľ�ɫ��1��2��

    Player();                        // ˽�л����캯��
    virtual ~Player();

    void interactWithMap();          // ��ͼ�����߼�
    void updateEnergy(float delta);  // ���¾���

public:
    static Player* create(int selectedCharacter, const std::string& nickname);

    bool init(int selectedCharacter, const std::string& nickname);

    void setEnergy(int energy);
    int getEnergy() const;

    void setCurrentTool(int toolId);
    void move(float delta);

    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void onMouseDown(Event* event);

    void update(float delta);

    // ��ȡ�ǳ�
    const std::string& getNickname() const;

    // ��ȡѡ��Ľ�ɫ
    int getSelectedCharacter() const;
};

#endif // PLAYER_H