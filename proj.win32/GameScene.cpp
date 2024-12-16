/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: 实现GameScene类，展示游戏主界面和基本功能：实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI 
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 *********************************************************************************************************/

#include "GameScene.h"
#include "MenuScene.h" // 菜单场景
#include "CreateErrorScene.h"
#include "AnimalManager.h"

USING_NS_CC;

GameScene* GameScene::instance = nullptr;

GameScene::GameScene()
    : player(nullptr), mapManager(nullptr), uiManager(nullptr) {}

GameScene* GameScene::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {
        instance = GameScene::createScene(selectedCharacter, nickname);
    }
    return instance;
}

 GameScene* GameScene::createScene(int selectedCharacter, const std::string& nickname) {
    auto scene = new (std::nothrow) GameScene();
    if (scene && scene->init(selectedCharacter,nickname )) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool GameScene::init(int selectedCharacter,const std::string& nickname ) {
    if (!Scene::init()) {
        throw std::runtime_error ("GameScene create failed!!");
        return false;
    }
    
    this->nickname = nickname;
    this->selectedCharacter = selectedCharacter;


    gameTime = 0.0f;
    currentHour = 8;
    currentMinute = 0;
    try {

        // 初始化地图管理器
        mapManager = MapManager::getInstance();
        //CCLOG("Map Manager created");
        mapManager->loadMapBlocks("map");
        CCLOG("MapManager getInstance");

        // 加载第一个地图块
        auto currentMap = mapManager->getCurrentBlock();
        this->addChild(mapManager);

        CCLOG("currentMap : %p ", currentMap);
        CCLOG("currentMap getCurrentBlock  created");


        // 初始化玩家 selectedCharacter
        player = Player::create(selectedCharacter, nickname);
        player->setPosition(mapManager->getPlayerStartPos()); // 初始位置
        this->addChild(player);
        CCLOG("Player created");

        manager = AnimalManager::getInstance();
        // 初始化各类型的小动物
        chicken = Animal::create(1, "chicken");
        chicken->setPosition(mapManager->getChickenStartPos()); // 初始位置
        this->addChild(chicken);
        CCLOG("Chicken created");
        manager->addAnimal(chicken);
        chicken->initialmove("Chicken.png");

        cow = Animal::create(2, "cow");
        cow->setPosition(mapManager->getCowStartPos()); // 初始位置
        this->addChild(cow);
        manager->addAnimal(cow);
        CCLOG("Cow created");
        cow->initialmove("Cow.png");

        cat = Animal::create(3, "cat");
        cat->setPosition(mapManager->getCatStartPos()); // 初始位置
        this->addChild(cat);
        manager->addAnimal(cat);
        CCLOG("Cat created");
        cat->initialmove("Cat.png");

        dog = Animal::create(4, "dog");
        dog->setPosition(mapManager->getDogStartPos()); // 初始位置
        this->addChild(dog);
        manager->addAnimal(dog);
        CCLOG("Dog created");
        dog->initialmove("Dog.png");

        pig = Animal::create(5, "pig");
        pig->setPosition(mapManager->getPigStartPos()); // 初始位置
        this->addChild(pig);
        manager->addAnimal(pig);
        CCLOG("Pig created");
        pig->initialmove("Pig.png");

        sheep = Animal::create(6, "sheep");
        sheep->setPosition(mapManager->getSheepStartPos()); // 初始位置
        this->addChild(sheep);
        manager->addAnimal(sheep);
        CCLOG("Sheep created");
        sheep->initialmove("Sheep.png");

        // 初始化种植


        // 初始化 UI 管理器
        uiManager = UIManager::getInstance(selectedCharacter, nickname);
        this->addChild(uiManager->getLayer());
        CCLOG("UIManager created");
    }
    catch (const std::exception& e) {
        //捕获异常 记录日志
        CCLOG("Exception caught : %s", e.what());
        //加载备用错误界面
        auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
        Director::getInstance()->replaceScene(errorScene);
    }


    // 注册 update 方法
   this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt) {
    // 更新游戏逻辑


    // 更新时间逻辑，1秒游戏时间变为1分钟
    gameTime += dt;  // 增加游戏时间（按秒计）
    CCLOG("时间更新");
    
    // 传递 dt 参数给 uiManager
    if (gameTime >= 1.0f) {  // 每秒游戏时间增加一分钟
        gameTime = 0.0f;  // 重置游戏计时器
        
        currentMinute += 1;  // 增加分钟
        if (currentMinute >= 60) {  // 处理分钟超过60的情况
            currentMinute = 0;
            currentHour += 1;  // 增加小时
        }
        if (currentHour >= 24) {  // 处理小时超过24的情况
            currentHour = 0;  // 重新开始一天
        }

        // 更新UI中的时间
        char timeStr[10];
       snprintf(timeStr, sizeof(timeStr), "%02d:%02d", currentHour, currentMinute);
        uiManager->setDateAndTime("March 1st, Tuesday", timeStr);
        
    }
   
    if (uiManager) {
        uiManager->update(dt);
    }

}

Player* GameScene::getPlayer() {
    return player;
}

void GameScene::pauseGame() {
    // 显示暂停菜单
    Director::getInstance()->pause();
    log("Game Paused");
}

void GameScene::resumeGame() {
    // 恢复游戏
    Director::getInstance()->resume();
    log("Game Resumed");
}

void GameScene::replaceChild(Node* oldChild, Node* newChild) {
    if (oldChild && newChild) {
        this->removeChild(oldChild);
        this->addChild(newChild);
    }
}