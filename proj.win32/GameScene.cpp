/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: 实现GameScene类，展示游戏主界面和基本功能：实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI 
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 *********************************************************************************************************/

#include "GameScene.h"
#include "MenuScene.h" // 菜单场景


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
        return false;
    }
    this->nickname = nickname;
    this->selectedCharacter = selectedCharacter;


    // 初始化地图管理器
    mapManager = MapManager::getInstance();
    mapManager->loadMapBlocks("map");

    // 加载第一个地图块
    auto currentMap = mapManager->getCurrentBlock();
    this->addChild(currentMap);

    // 初始化玩家
    player = Player::create(selectedCharacter);
    player->setPosition(Vec2(100, 100)); // 初始位置
    this->addChild(player);

    // 初始化 UI 管理器
    uiManager = UIManager::getInstance();
    this->addChild(uiManager->getLayer());

    // 注册 update 方法
    this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt) {
    // 更新游戏逻辑
    if (mapManager && player) {
        Direction direction;
        if (mapManager->isAtEdge(player->getPosition(), direction)) {
            mapManager->switchToBlock(direction);
        }
    }

    // 传递 dt 参数给 uiManager
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