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
    screenSize = Director::getInstance()->getVisibleSize();
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

       currentMap = mapManager->getCurrentBlock();
        this->addChild(mapManager);
        this->addChild(currentMap);

        //CCLOG("currentMap : %p ", currentMap);
        CCLOG("currentMap getCurrentBlock  created");


        // 初始化玩家 selectedCharacter
        player = Player::create(selectedCharacter, nickname);

        player->setPosition(mapManager->getPlayerStartPos()); // 初始位置
        this->addChild(player);
        CCLOG("Player created");


        // 初始化 UI 管理器
        uiManager = UIManager::getInstance(selectedCharacter, nickname);
        this->addChild(uiManager->getLayer());
        CCLOG("UIManager created");

        //工具
        
        toolManager = ToolManager::create();
        this->addChild(toolManager);
        toolManager->addTool(Tool::ToolType::HOE);
        toolManager->addTool(Tool::ToolType::AXE);
        toolManager->addTool(Tool::ToolType::FISHING_ROD);
        toolManager->addTool(Tool::ToolType::WATERING_CAN);
       
        auto keyboardListener = EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_C) {
                toolManager->useTool(); // 使用选中的工具
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_V) {
                toolManager->discardTool(); // 丢弃选中的工具
            }
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

        //植物


        //动物



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


    
    //地图切换
    if (mapManager && player&&currentMap) {
       Vec2 direction;
      
        if (mapManager->isAtEdge(player->getPosition(), direction)) {
           // mapManager->switchToBlock(direction); // 切换地图
           // this->removeChild(currentMap);
           // currentMap = mapManager->getCurrentBlock();
           // this->addChild(currentMap);
            //重置位置
            const Vec2 vec = player->getPosition();
            if (direction == Vec2(-1, 0)) { // 左边界
                
                player->setPosition(screenSize.width-10, vec.y);
            }
            else if (direction == Vec2(1, 0)) { // 右边界
                player->setPosition(10, vec.y);
            }
            else if (direction == Vec2(0, -1)) { // 下边界
                player->setPosition(vec.x,screenSize.height-10);
            }
            else if (direction == Vec2(0, 1)) { // 上边界
                player->setPosition(vec.x,10);
            }
          
        }
    }

    
// 更新时间逻辑，1秒游戏时间变为1分钟
    gameTime += dt;  // 增加游戏时间（按秒计）
    
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