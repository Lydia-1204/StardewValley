#pragma once


#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"

class Player : public cocos2d::Node {
public:
    // ���캯������������
    Player();
    ~Player();

    // ��ʼ�����
    virtual bool init();

    // �������ʵ��
    static Player* create(int selectedCharacterIndex);

    // ������Ҿ���ֵ
    void setEnergy(int energy);
    int getEnergy() const;

    // �������״̬
    void update(float delta);

    // �����������
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // �����������
    void onMouseDown(cocos2d::Event* event);

    // �ƶ��߼�
    void move(float delta);

    // ��ͼ�����߼�
    void interactWithMap();

    // ���õ�ǰʹ�õĹ���
    void setCurrentTool(int toolId);

private:
    cocos2d::Sprite* _sprite;  // ��ҽ�ɫ�ľ���
    cocos2d::Vec2 _velocity;  // �ƶ��ٶ�
    int _energy;              // ��ǰ����ֵ
    int _currentTool;         // ��ǰ����ID
    float _speed;             // ����ƶ��ٶ�

    bool _isMoving;           // ����Ƿ������ƶ�
    std::set<cocos2d::EventKeyboard::KeyCode> _keysPressed;  // ��¼���µļ�

    void updateEnergy(float delta);  // ���¾���ֵ
};

#endif // PLAYER_H
