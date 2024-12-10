/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: ʵ��GameScene�࣬չʾ��Ϸ������ͻ������ܣ�ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI 
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 *********************************************************************************************************/

#include "GameScene.h"
#include "MenuScene.h" // �˵�����


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


    // ��ʼ����ͼ������
    mapManager = MapManager::getInstance();
    mapManager->loadMapBlocks("map");

    // ���ص�һ����ͼ��
    auto currentMap = mapManager->getCurrentBlock();
    this->addChild(currentMap);

    // ��ʼ�����
    player = Player::create(selectedCharacter);
    player->setPosition(Vec2(100, 100)); // ��ʼλ��
    this->addChild(player);

    // ��ʼ�� UI ������
    uiManager = UIManager::getInstance();
    this->addChild(uiManager->getLayer());

    // ע�� update ����
    this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt) {
    // ������Ϸ�߼�
    if (mapManager && player) {
        Direction direction;
        if (mapManager->isAtEdge(player->getPosition(), direction)) {
            mapManager->switchToBlock(direction);
        }
    }

    // ���� dt ������ uiManager
    if (uiManager) {
        uiManager->update(dt);
    }
}

Player* GameScene::getPlayer() {
    return player;
}

void GameScene::pauseGame() {
    // ��ʾ��ͣ�˵�
    Director::getInstance()->pause();
    log("Game Paused");
}

void GameScene::resumeGame() {
    // �ָ���Ϸ
    Director::getInstance()->resume();
    log("Game Resumed");
}

void GameScene::replaceChild(Node* oldChild, Node* newChild) {
    if (oldChild && newChild) {
        this->removeChild(oldChild);
        this->addChild(newChild);
    }
}