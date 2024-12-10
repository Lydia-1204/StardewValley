#pragma once


#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"

class Player : public cocos2d::Node {
public:
    // 构造函数和析构函数
    Player();
    ~Player();

    // 初始化玩家
    virtual bool init();

    // 创建玩家实例
    static Player* create(int selectedCharacterIndex);

    // 设置玩家精力值
    void setEnergy(int energy);
    int getEnergy() const;

    // 更新玩家状态
    void update(float delta);

    // 处理键盘输入
    void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
    void onKeyReleased(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

    // 处理鼠标输入
    void onMouseDown(cocos2d::Event* event);

    // 移动逻辑
    void move(float delta);

    // 地图交互逻辑
    void interactWithMap();

    // 设置当前使用的工具
    void setCurrentTool(int toolId);

private:
    cocos2d::Sprite* _sprite;  // 玩家角色的精灵
    cocos2d::Vec2 _velocity;  // 移动速度
    int _energy;              // 当前精力值
    int _currentTool;         // 当前工具ID
    float _speed;             // 玩家移动速度

    bool _isMoving;           // 玩家是否正在移动
    std::set<cocos2d::EventKeyboard::KeyCode> _keysPressed;  // 记录按下的键

    void updateEnergy(float delta);  // 更新精力值
};

#endif // PLAYER_H
