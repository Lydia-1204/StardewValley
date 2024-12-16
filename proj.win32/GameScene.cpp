/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: ʵ��GameScene�࣬չʾ��Ϸ������ͻ������ܣ�ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI 
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 *********************************************************************************************************/

#include "GameScene.h"
#include "MenuScene.h" // �˵�����
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

        // ��ʼ����ͼ������
        mapManager = MapManager::getInstance();
        //CCLOG("Map Manager created");
        mapManager->loadMapBlocks("map");
        CCLOG("MapManager getInstance");

        // ���ص�һ����ͼ��
        auto currentMap = mapManager->getCurrentBlock();
        this->addChild(mapManager);

        CCLOG("currentMap : %p ", currentMap);
        CCLOG("currentMap getCurrentBlock  created");


        // ��ʼ����� selectedCharacter
        player = Player::create(selectedCharacter, nickname);
        player->setPosition(mapManager->getPlayerStartPos()); // ��ʼλ��
        this->addChild(player);
        CCLOG("Player created");

        manager = AnimalManager::getInstance();
        // ��ʼ�������͵�С����
        chicken = Animal::create(1, "chicken");
        chicken->setPosition(mapManager->getChickenStartPos()); // ��ʼλ��
        this->addChild(chicken);
        CCLOG("Chicken created");
        manager->addAnimal(chicken);
        chicken->initialmove("Chicken.png");

        cow = Animal::create(2, "cow");
        cow->setPosition(mapManager->getCowStartPos()); // ��ʼλ��
        this->addChild(cow);
        manager->addAnimal(cow);
        CCLOG("Cow created");
        cow->initialmove("Cow.png");

        cat = Animal::create(3, "cat");
        cat->setPosition(mapManager->getCatStartPos()); // ��ʼλ��
        this->addChild(cat);
        manager->addAnimal(cat);
        CCLOG("Cat created");
        cat->initialmove("Cat.png");

        dog = Animal::create(4, "dog");
        dog->setPosition(mapManager->getDogStartPos()); // ��ʼλ��
        this->addChild(dog);
        manager->addAnimal(dog);
        CCLOG("Dog created");
        dog->initialmove("Dog.png");

        pig = Animal::create(5, "pig");
        pig->setPosition(mapManager->getPigStartPos()); // ��ʼλ��
        this->addChild(pig);
        manager->addAnimal(pig);
        CCLOG("Pig created");
        pig->initialmove("Pig.png");

        sheep = Animal::create(6, "sheep");
        sheep->setPosition(mapManager->getSheepStartPos()); // ��ʼλ��
        this->addChild(sheep);
        manager->addAnimal(sheep);
        CCLOG("Sheep created");
        sheep->initialmove("Sheep.png");

        // ��ʼ����ֲ


        // ��ʼ�� UI ������
        uiManager = UIManager::getInstance(selectedCharacter, nickname);
        this->addChild(uiManager->getLayer());
        CCLOG("UIManager created");
    }
    catch (const std::exception& e) {
        //�����쳣 ��¼��־
        CCLOG("Exception caught : %s", e.what());
        //���ر��ô������
        auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
        Director::getInstance()->replaceScene(errorScene);
    }


    // ע�� update ����
   this->scheduleUpdate();

    return true;
}

void GameScene::update(float dt) {
    // ������Ϸ�߼�


    // ����ʱ���߼���1����Ϸʱ���Ϊ1����
    gameTime += dt;  // ������Ϸʱ�䣨����ƣ�
    CCLOG("ʱ�����");
    
    // ���� dt ������ uiManager
    if (gameTime >= 1.0f) {  // ÿ����Ϸʱ������һ����
        gameTime = 0.0f;  // ������Ϸ��ʱ��
        
        currentMinute += 1;  // ���ӷ���
        if (currentMinute >= 60) {  // ������ӳ���60�����
            currentMinute = 0;
            currentHour += 1;  // ����Сʱ
        }
        if (currentHour >= 24) {  // ����Сʱ����24�����
            currentHour = 0;  // ���¿�ʼһ��
        }

        // ����UI�е�ʱ��
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