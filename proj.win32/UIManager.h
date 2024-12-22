/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: 声明UIManager类，用于实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#ifndef UI_MANAGER_H
#define UI_MANAGER_H

#include "cocos2d.h"
#include <string>
#include"map.h"
#include"toolManager.h"


USING_NS_CC;

class Player;  // 声明 Player 类

class UIManager : public Node {
private:
    static UIManager* instance;
    MapManager* mapManager;
    ToolManager* toolManager;
    // UI元素
    Label* dateLabel;                  // 显示日期
    Label* timeLabel;                  // 显示时间
    Label* moneyLabel;                 // 显示农场资金
    Label*weatherLabel;                 // 显示农场资金
    ProgressTimer* energyBar;          // 显示精力条
    Sprite* iron;                      // 美观图标
    Label* shortcutKeysLabel;          // 显示快捷键提示
    Label* energyLabel;                //精力条数据显示

   
    cocos2d::LayerColor* pausePanel;    // 暂停面板
    cocos2d::Sprite* miniMap;           // 小地图
    cocos2d::Sprite* playerMarker;           // 小地图玩家标记
    bool isMiniMapVisible;

    LayerColor* taskListPanel;
    std::vector<cocos2d::Label*> taskLabels;      // 任务列表中的任务标签
    bool isTaskListVisible;                       // 任务列表显示状态
    //cocos2d::Sprite* priceBoard;           // 价格表
    // 数据成员
    std::string nickname;              // 玩家昵称
    int selectedCharacter;             // 选择的角色
    int money;                         // 当前农场资金

    int currentMonth;                  // 当前月份
    int currentDay;                    // 当前日期
    int currentWeekday;                // 当前周几（0=周日，1=周一，...，6=周六）
    int currentHour;                   // 当前小时
    int currentMinute;                 // 当前分钟
    int currentEnergy;                 // 当前精力值（百分比）

    float timeElapsed;                 // 用于累积时间（秒）
  
    UIManager();                       // 私有化构造函数

    // 私有方法
    void updateDateAndTime();          // 更新日期和时间标签
    int getDaysInMonth(int month);     // 获取指定月份的天数


   // Chest* chest;
public:
    friend class Player;
    friend class GameScene;
    cocos2d::Sprite* dialogSprite; // 显示对话状态精灵
    cocos2d::Sprite* chatSprite; // 对话框精灵
    Label* dialogTextLabel;  // 用来显示文本的 Label
    static UIManager* getInstance(int& selectedCharacter, const std::string& nickname);

    // 添加 getter currentenergy方法（用于其他Classes中）
    int getCurrentEnergy() const { return currentEnergy; }
    int getMoney() const { return money; }
    // 添加 setter 方法（如果需要更新精力值）
    void setCurrentEnergy(int energy) {
        currentEnergy = energy;
        setEnergy(currentEnergy);  // 更新 UI
    }

    bool init(int selectedCharacter, const std::string& nickname); // 初始化

    void update(float delta);           // 每帧更新

    // 设置 UI 显示内容
    void setDateAndTime(const std::string& date, const std::string& time);

    void setMoney(int dmoney); //变化的钱+-
    void setEnergy(int energy); 
    void increaseEnergy(float deltaEnergy);

    UIManager* getLayer();                  // 获取 UI 层
    void setToolBar(const std::vector<Sprite*>& tools); // 设置工具栏

    // 小地图方法
    void toggleMiniMap(const cocos2d::Vec2& playerPos, const cocos2d::Size& mapSize); // 显示/隐藏小地图
    void updateMiniMapPosition(const cocos2d::Vec2& playerPos, const cocos2d::Size& mapSize); // 更新玩家在小地图的位置

    /*      价格表        */
    void showPriceBoard();
    void closePriceBoard();
    bool isPriceBoardOpen; 


    // 任务面板方法
    void toggleTaskList();                        // 显示/隐藏任务列表

    void updateTaskList(const std::vector<std::string>& tasks); // 更新任务列表

    void showPausePanel();              // 显示暂停面板

    void hidePausePanel();              // 隐藏暂停面板


    //npc设置
    void setNpcVision();
};

#endif // UI_MANAGER_H