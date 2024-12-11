#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include <string>
#include <set>

USING_NS_CC;

class Player : public Node {
private:
    Sprite* _sprite;                 // 玩家角色的精灵
    Vec2 _velocity;                  // 移动速度方向
    int _energy;                     // 玩家精力值
    int _currentTool;                // 当前工具 ID
    float _speed;                    // 移动速度
    bool _isMoving;                  // 是否在移动
    std::set<EventKeyboard::KeyCode> _keysPressed; // 按键集合
    std::string _nickname;           // 玩家昵称
    int _selectedCharacter;          // 玩家选择的角色（1或2）

    Player();                        // 私有化构造函数
    virtual ~Player();

    void interactWithMap();          // 地图交互逻辑
    void updateEnergy(float delta);  // 更新精力

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

    // 获取昵称
    const std::string& getNickname() const;

    // 获取选择的角色
    int getSelectedCharacter() const;
};

#endif // PLAYER_H