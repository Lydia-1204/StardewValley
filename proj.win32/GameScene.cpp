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

        // ��ʼ����ͼ������
        mapManager = MapManager::getInstance();
        //CCLOG("Map Manager created");
        mapManager->loadMapBlocks("map");
        CCLOG("MapManager getInstance");

        // ���ص�һ����ͼ��

       currentMap = mapManager->getCurrentBlock();
        this->addChild(mapManager);
        this->addChild(currentMap);

        //CCLOG("currentMap : %p ", currentMap);
        CCLOG("currentMap getCurrentBlock  created");


        // ��ʼ����� selectedCharacter
        player = Player::create(selectedCharacter, nickname);

        player->setPosition(mapManager->getPlayerStartPos()); // ��ʼλ��
        this->addChild(player);
        CCLOG("Player created");


        // ��ʼ�� UI ������
        uiManager = UIManager::getInstance(selectedCharacter, nickname);
        this->addChild(uiManager->getLayer());
        CCLOG("UIManager created");

        //����
        
        toolManager = ToolManager::create();
        this->addChild(toolManager);
        toolManager->addTool(Tool::ToolType::HOE);
        toolManager->addTool(Tool::ToolType::AXE);
        toolManager->addTool(Tool::ToolType::FISHING_ROD);
        toolManager->addTool(Tool::ToolType::WATERING_CAN);
       
        auto keyboardListener = EventListenerKeyboard::create();
        keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
            if (keyCode == EventKeyboard::KeyCode::KEY_C) {
                toolManager->useTool(); // ʹ��ѡ�еĹ���
            }
            else if (keyCode == EventKeyboard::KeyCode::KEY_V) {
                toolManager->discardTool(); // ����ѡ�еĹ���
            }
            };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

        //ֲ��


        //����



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


    
    //��ͼ�л�
    if (mapManager && player&&currentMap) {
       Vec2 direction;
      
        if (mapManager->isAtEdge(player->getPosition(), direction)) {
           // mapManager->switchToBlock(direction); // �л���ͼ
           // this->removeChild(currentMap);
           // currentMap = mapManager->getCurrentBlock();
           // this->addChild(currentMap);
            //����λ��
            const Vec2 vec = player->getPosition();
            if (direction == Vec2(-1, 0)) { // ��߽�
                
                player->setPosition(screenSize.width-10, vec.y);
            }
            else if (direction == Vec2(1, 0)) { // �ұ߽�
                player->setPosition(10, vec.y);
            }
            else if (direction == Vec2(0, -1)) { // �±߽�
                player->setPosition(vec.x,screenSize.height-10);
            }
            else if (direction == Vec2(0, 1)) { // �ϱ߽�
                player->setPosition(vec.x,10);
            }
          
        }
    }

    
// ����ʱ���߼���1����Ϸʱ���Ϊ1����
    gameTime += dt;  // ������Ϸʱ�䣨����ƣ�
    
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