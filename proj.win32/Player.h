/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.h
 * File Function: Player类的声明
 * Author:        邓语乐 2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#ifndef PLAYER_H
#define PLAYER_H

#define NO_TRANSPARENT 255
#define HALF_TRANSPARENT 128

#include "cocos2d.h"
#include "UIManager.h"  // 引入 UIManager 头文件
#include "tool.h"
#include "NPC.h"
#include <string>
#include <set>

USING_NS_CC;

class Player : public Node {
private:
    class UIManager* uiManager;            // 保存 UIManager 实例的指针
    class Tool* tool;        // 保存 Tool 实例的指针
    class ToolManager* toolManager;        // 保存 ToolManager 实例的指针
   
    Sprite* _toolSprite;             // 工具精灵

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
    static Player* instance;     // 单例实例

    Layer* menuLayer; // 添加一个成员变量来存储菜单层的引用
    int Planting_Skills;// 种植技能
    int Breeding_Skills;// 养殖技能
    int Mining_Skills;// 挖矿技能
    int Fishing_Skills;// 钓鱼技能
public:
    static Player* getInstance(int selectedCharacter, const std::string& nickname);
    static Player* create(int selectedCharacter, const std::string& nickname);
    Player(UIManager* ui) : uiManager(ui) {}  // 构造函数初始化 uiManager
    Player(Tool* _to) : tool(_to) {}
    Player(ToolManager* _toMa) : toolManager(_toMa) {}
    Sprite* _sprite[4];              // 玩家角色的精灵 四个方向（上右后左）

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
    void onMouseMove(cocos2d::Event* event); // 鼠标移动事件响应 用于完成与npc的互动
    bool isGift = 1;

    // 获取昵称
    const std::string& getNickname() const;

    // 获取选择的角色
    int getSelectedCharacter() const;
    void setPlayerPosition(cocos2d::Vec2& position);
    Vec2 getPlayerPosition() const;       // 返回Player当前位置
    int _currentDirection = 0;       // 记录当时玩家精灵面对的方向
    void updateToolSprite();
    void updateDialogPosition(NPC* npc);
    void showDialogBox(NPC* npc, const std::string& category);

    std::chrono::steady_clock::time_point lastClickTime; // 用于记录上次点击的时间点
    bool wasDoubleClick = false; // 标记是否可能是双击
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