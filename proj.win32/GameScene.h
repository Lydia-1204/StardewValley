/**********************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.h
 * File Function: 声明GameScene类，用于实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI 
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 **********************************************************************************************/

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "Player.h"
#include "map.h"
#include "UIManager.h"
#include "Animal.h"
#include "AnimalManager.h"

USING_NS_CC;

class GameScene : public Scene {
private:
    static GameScene* instance;   // 单例实例

    Player* player;               // 玩家角色
    MapManager* mapManager;       // 地图管理器
    UIManager* uiManager;         // UI 管理器
    std::string nickname;
    int selectedCharacter;

    Animal* chicken;            // 动物    
    Animal* cat;            // 动物       
    Animal* dog;            // 动物      
    Animal* pig;            // 动物       
    Animal* sheep;            // 动物       
    Animal* cow;            // 动物       
    AnimalManager* manager = new AnimalManager();


    float gameTime;  //时间流逝 单位为秒
    int currentHour;   //小时
    int currentMinute;    //分钟

public:
    static GameScene* getInstance(int selectedCharacter, const std::string& nickname);     // 获取单例实例
    static GameScene* createScene(int selectedCharacter, const std::string& nickname);     // 创建场景

    virtual bool init(int selectedCharacter, const std::string& nickname);        // 初始化
    virtual void update(float dt) override; // 每帧更新

    Player* getPlayer();                 // 获取玩家实例

    void pauseGame();                    // 暂停游戏
    void resumeGame();                   // 恢复游戏

    // 容器管理
    void replaceChild(Node* oldChild, Node* newChild);
    GameScene();
    virtual~GameScene()= default;
};

#endif // GAME_SCENE_Hwdas