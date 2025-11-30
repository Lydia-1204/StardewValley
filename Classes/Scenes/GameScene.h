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
#include <vector>
#include "Characters/Player.h"
#include "World/Map.h"
#include "Systems/UIManager.h"
#include "Inventory/ToolManager.h"
#include "Inventory/ItemManager.h"
#include "Animals/Animal.h"
#include "Farming/Crop.h"
#include "Characters/NpcTemplate.h"
#include "Inventory/Chest.h"
USING_NS_CC;

class GameScene : public Scene {
private:
    friend class Item;
    static GameScene* instance;   // 单例实例

    bool isGamePaused; //是否暂停
    Player* player;               // 玩家角色
    MapManager* mapManager;       // 地图管理器
    TMXTiledMap* currentMap; //当前地图快
    UIManager* uiManager;         // UI 管理器
    ToolManager* toolManager;
    ItemManager* itemManager;
    NpcTemplate* npcs;
    std::string nickname;  //昵称
    int selectedCharacter; // 1 -Amily/2-Harvey
    Size screenSize;
    float gameTime;  //时间流逝 单位为秒
    int currentHour;   //小时
    int currentMinute;    //分钟


       
    Animal* chicken;            // 动物    
    Animal* cat;            // 动物       
    Animal* dog;            // 动物      
    Animal* pig;            // 动物       
    Animal* sheep;            // 动物       
    Animal* cow;            // 动物       
    //AnimalManager* manager = new AnimalManager();

   
   std::vector<Crop*> myCrops;  // 作物实例
   bool isCropAlive;  // 跟踪 myCrop 是否存在
   Sprite* iron;
   LayerColor* sleepPanel;
public:
    static GameScene* getInstance(int& selectedCharacter, const std::string& nickname);     // 获取单例实例
    static GameScene* createScene(int& selectedCharacter, const std::string& nickname);     // 创建场景

    virtual bool init(int& selectedCharacter, const std::string& nickname);        // 初始化
    virtual void update(float dt) override; // 每帧更新
    void togglePause();                    //切换暂停状态
    Player* getPlayer();                 // 获取玩家实例

    void removeCrop();
    void onCropTextureChanged(const std::string& texturePath);  // 声明函数

    Animal* getChicken() { return chicken; }
    Animal* getCow() { return cow; }
    Animal* getCat() { return cat; }
    Animal* getDog() { return dog; }
    Animal* getPig() { return pig; }
    Animal* getSheep() { return sheep; }

    void pauseGame();                    // 暂停游戏
    void resumeGame();                   // 恢复游戏

    // 容器管理
    void replaceChild(Node* oldChild, Node* newChild);
    GameScene();
    virtual~GameScene()= default;
    void initKeyboardListener();    //快捷键
};

#endif // GAME_SCENE_Hwdas
