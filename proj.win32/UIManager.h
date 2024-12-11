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

USING_NS_CC;

class UIManager : public Node {
private:
    static UIManager* instance;

    // UI元素
    Label* dateLabel;                  // 显示日期
    Label* timeLabel;                  // 显示时间
    Label* moneyLabel;                 // 显示农场资金
    ProgressTimer* energyBar;          // 显示精力条
    Sprite* iron;                      // 美观图标
    Label* shortcutKeysLabel;          // 显示快捷键提示

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

public:
    static UIManager* getInstance(int selectedCharacter, const std::string& nickname);

    bool init(int selectedCharacter, const std::string& nickname); // 初始化

    void update(float delta);           // 每帧更新

    // 设置 UI 显示内容
    void setDateAndTime(const std::string& date, const std::string& time);

    void setMoney(int money);
    void setEnergy(int energy);

    UIManager* getLayer();                  // 获取 UI 层
    void setToolBar(const std::vector<Sprite*>& tools); // 设置工具栏
};

#endif // UI_MANAGER_H