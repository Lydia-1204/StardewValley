/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.h
 * File Function: Player类的声明
 * Author:        邓语乐 2351273
 * Update Date:   2024/12/16
 *********************************************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#include "cocos2d.h"
#include "UIManager.h"  // 引入 UIManager 头文件
#include <string>
#include <set>

USING_NS_CC;

class Player : public Node {
private:
    UIManager* uiManager;  // 保存 UIManager 实例的指针

    Sprite* _toolSprite;             // 斧头工具精灵


    Sprite* _fishing_rodSprite;      // 鱼竿工具精灵
    Sprite* _watering_canSprite;     // 水壶精灵
    Sprite* _scytheSprite;           // 镰刀精灵
    Sprite* _hoeSprite;              // 锄头工具精灵

    bool _shiftKeyPressed;           // 标记Shift键是否按下
    bool _isMousePressed;            // 鼠标是否按下
    Vec2 _velocity;                  // 移动速度方向
    int _energy;                     // 玩家精力值
    int _currentTool;                // 当前工具 ID
    float _speed;                    // 移动速度
    bool _isMoving;                  // 是否在移动
    std::set<EventKeyboard::KeyCode> _keysPressed;  // 按键集合
    std::string _nickname;           // 玩家昵称
    int _selectedCharacter;          // 玩家选择的角色（1或2）
    bool _isToolActive;              // 标记斧头是否处于激活状态

    Player();                        // 私有化构造函数
    virtual ~Player();

    void interactWithMap();          // 地图交互逻辑
    void updateEnergy(float delta);  // 更新精力

public:
    Player(UIManager* ui) : uiManager(ui) {}  // 构造函数初始化 uiManager
    Sprite* _sprite[4];              // 玩家角色的精灵 四个方向（上右后左）
    static Player* create(int selectedCharacter, const std::string& nickname);

    bool init(int selectedCharacter, const std::string& nickname);
    void setEnergy(int energy);
    int getEnergy() const;
    void setCurrentTool(int toolId);
    void move(float delta);
    void onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event);
    void onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event);
    void update(float delta);
    void onMouseDown(Event* event);           // 鼠标按下事件
    void onMouseUp(cocos2d::Event* event);    // 鼠标释放事件

    // 获取昵称
    const std::string& getNickname() const;

    // 获取选择的角色
    int getSelectedCharacter() const;
    void setPlayerPosition(cocos2d::Vec2& position);
    Vec2 getPlayerPosition() const;       // 返回Player当前位置
    int _currentDirection = 0;       // 记录当时玩家精灵面对的方向
};

#endif // PLAYER_H