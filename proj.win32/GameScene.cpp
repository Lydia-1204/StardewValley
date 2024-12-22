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
#include "npcTemplate.h"

USING_NS_CC;

GameScene* GameScene::instance = nullptr;

GameScene::GameScene()
    : player(nullptr), mapManager(nullptr), uiManager(nullptr) {}

GameScene* GameScene::getInstance(int& selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {
        instance = GameScene::createScene(selectedCharacter, nickname);
    }
    return instance;
}

 GameScene* GameScene::createScene(int& selectedCharacter, const std::string& nickname) {
    auto scene = new (std::nothrow) GameScene();
    if (scene && scene->init(selectedCharacter,nickname )) {
        scene->autorelease();
        return scene;
    }
    CC_SAFE_DELETE(scene);
    return nullptr;
}

bool GameScene::init(int& selectedCharacter,const std::string& nickname ) {
    screenSize = Director::getInstance()->getVisibleSize();
    if (!Scene::init()) {
        throw std::runtime_error ("GameScene create failed!!");
        return false;
    }
    isGamePaused = false;
    this->nickname = nickname;
    this->selectedCharacter = selectedCharacter;


    gameTime = 0.0f;
    currentHour = 8;
    currentMinute = 0;
    try {

        // ��ʼ����ͼ������
        mapManager = MapManager::getInstance();
        //CCLOG("Map Manager created");
        mapManager->loadMapBlocks("map"); // ���ص�һ����ͼ��
        CCLOG("MapManager getInstance");
        this->addChild(mapManager);


        //this->addChild(currentMap); mapManager��add ��Ҫ�ظ�add

        //CCLOG("currentMap : %p ", currentMap);
        CCLOG("currentMap getCurrentBlock  created");


        // ��ʼ����� selectedCharacter
        player = Player::getInstance(selectedCharacter, nickname);

        player->setPlayerPosition(mapManager->getPlayerStartPos()); // ��ʼλ��
        player->setPosition(200, 350);
        this->addChild(player);
        CCLOG("Player created");


        
        // ��ʼ�� UI ������
        uiManager = UIManager::getInstance(selectedCharacter, nickname);
        this->addChild(uiManager->getLayer());
        CCLOG("UIManager created");
        

        //����
        
        iron = Sprite::create("../Resources/boxxx.png");
        if (!iron)
            throw("iron created failed!!");
        iron->setPosition(Vec2(230,430));
        this->addChild(iron, 20);
        iron->setVisible(false);
        //����lei

        auto chest = Chest::getInstance();
        if (chest == nullptr) {
            throw std::runtime_error("Chest create failed!!");
            return false;
        }


        this->addChild(chest);
        chest->isOpen = 0;
        chest->chestSetPosition(Vec2(230, 430));

        //����

        toolManager = ToolManager::getInstance(selectedCharacter, nickname);
        this->addChild(toolManager);
        toolManager->addTool(Tool::ToolType::HOE);
        toolManager->addTool(Tool::ToolType::AXE);
        toolManager->addTool(Tool::ToolType::WATERING_CAN);
     
        toolManager->addTool(Tool::ToolType::FISHING_ROD);
        toolManager->addTool(Tool::ToolType::FERTILIZER);

        toolManager->addTool(Tool::ToolType::HOEPLUS);
        toolManager->addTool(Tool::ToolType::AXEPLUS);
        toolManager->addTool(Tool::ToolType::ANIMALFOOD);

        //��Ʒ
        itemManager = ItemManager::getInstance(selectedCharacter, nickname);
        this->addChild(itemManager);
        itemManager->addItem(Item::ItemType::FISH);
        itemManager->addItem(Item::ItemType::FISH);
        itemManager->addItem(Item::ItemType::FISH);
        itemManager->addItem(Item::ItemType::WOOL);
        for (int i = 0; i < 10; i++)
            itemManager->addItem(Item::ItemType::SEED);
        itemManager->addItem(Item::ItemType::MILK);
        itemManager->addItem(Item::ItemType::BONE);
        itemManager->addItem(Item::ItemType::EGG);
        itemManager->addItem(Item::ItemType::EGG);
        itemManager->addItem(Item::ItemType::WOODEN);
        itemManager->addItem(Item::ItemType::FRUIT);
        for (int i = 0; i < 5; i++)
            itemManager->addItem(Item::ItemType::MINERAL);
        for (int i = 0; i < 3; i++)
            itemManager->addItem(Item::ItemType::GIFT);
        for (int i = 0; i < 2; i++)
            itemManager->addItem(Item::ItemType::FAT);
        //toolManager->addTool(Tool::ToolType::HOE);�������


        // ��ʼ�������͵�С����
        chicken = Animal::create(1, this);
        chicken->setPosition(mapManager->getChickenStartPos()); // ��ʼλ��
        this->addChild(chicken);
        CCLOG("Chicken created");
        chicken->initialmove("Chicken.png");

        cow = Animal::create(2, this);
        cow->setPosition(mapManager->getCowStartPos()); // ��ʼλ��
        this->addChild(cow);
        CCLOG("Cow created");
        cow->initialmove("Cow.png");

        cat = Animal::create(3, this);
        cat->setPosition(mapManager->getCatStartPos()); // ��ʼλ��
        this->addChild(cat);
        CCLOG("Cat created");
        cat->initialmove("Cat.png");

        dog = Animal::create(4, this);
        dog->setPosition(mapManager->getDogStartPos()); // ��ʼλ��
        this->addChild(dog);
        CCLOG("Dog created");
        dog->initialmove("Dog.png");

        pig = Animal::create(5, this);
        pig->setPosition(mapManager->getPigStartPos()); // ��ʼλ��
        this->addChild(pig);
        CCLOG("Pig created");
        pig->initialmove("Pig.png");

        sheep = Animal::create(6, this);
        sheep->setPosition(mapManager->getSheepStartPos()); // ��ʼλ��
        this->addChild(sheep);
        CCLOG("Sheep created");
        sheep->initialmove("Sheep.png");
        
        // ��ʼ����ֲ
        // ����һ���µ� Crop ʵ��
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 6; j++) {

                Crop* crop = Crop::create(this, "crop");
                crop->setPosition(mapManager->getCropStartPos().x + i * 16, mapManager->getCropStartPos().y - j * 16);  // ���������λ��
                myCrops.push_back(crop); // ����thisָ���Crop
                this->addChild(crop);  // ��������ӵ������� 
                // ���ûص�����
                CCLOG("Crop %d created", i);
                crop->setTextureChangedCallback([this](const std::string& texturePath) {
                    onCropTextureChanged(texturePath);
                    //  isCropAlive = true;  // ��ʼ����־Ϊ true

                    });
            }
        }
      
     
       // ��ʼ��˯�����
        sleepPanel = LayerColor::create(Color4B(20, 20, 20, 220)); // ��ɫ�Ҽ�����͸��
        sleepPanel->setVisible(false);
        this->addChild(sleepPanel);
        // ����һ����ǩ����ʾ����
        auto label = Label::createWithTTF("Sleeping...", "fonts/Marker Felt.ttf", 100);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(520, 250)); // ������ǩλ��
        sleepPanel->addChild(label);
       
        // ����shubiao
        auto listener = EventListenerMouse::create();
        listener->onMouseDown = [=](EventMouse* event) {
            auto playerPos = player->getPosition();
            auto chestPos = chest->getPosition();
            auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����
            auto locationInChestBg = this->convertToNodeSpace(locationInWorld); // ת���� tools_bg ������ϵ
            // ��ҿ������Ӳ����ʱ������
            if (locationInChestBg.distance(chestPos) < 50&& playerPos.distance(chestPos) < 50) {
                    if (chest) {
                       
                        if (chest->isOpen) {
                            ;
                        }
                        else {
                            chest->openChest();
                        }
                    }
            }
            else if (mapManager->currentMapLabel == 6 && locationInWorld.distance(Vec2(550, 230)) < 50) {
                if (uiManager->isPriceBoardOpen == 0)
                    uiManager->showPriceBoard();
            }
            else if (mapManager->currentMapLabel == 5 && locationInWorld.distance(Vec2(521, 223)) < 50 && playerPos.distance(Vec2(521, 223)) < 50) {
                if (sleepPanel->isVisible()) {
                    sleepPanel->setVisible(false);
                    Director::getInstance()->resume();
                    uiManager->currentEnergy = 100;
                    uiManager->currentDay += 1;
                    uiManager->currentHour = 8;
                    uiManager->currentMinute = 0;
                    uiManager->currentWeekday = (uiManager->currentWeekday + 1) % 7;

                    if (uiManager->currentDay > uiManager->getDaysInMonth(uiManager->currentMonth)) {
                        uiManager->currentDay = 1;
                        uiManager->currentMonth += 1;
                        if (uiManager->currentMonth > 12) {
                            uiManager->currentMonth = 1;
                        }
                    }
                }
                else {
                    sleepPanel->setVisible(true);
                    Director::getInstance()->pause();
                }
            }
           
        };
        _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);
    }
    catch (const std::exception& e) {
        //�����쳣 ��¼��־
        CCLOG("Exception caught : %s", e.what());
        //���ر��ô������
        auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
        Director::getInstance()->replaceScene(errorScene);
    }

    initKeyboardListener();
    // ע�� update ����
   this->scheduleUpdate();

    return true;
}
void GameScene::update(float dt) {
    // ������Ϸ�߼�
    if (MapManager::getInstance()->currentMapLabel == 2)
    {
        iron->setVisible(true);
    }
    else{
        iron->setVisible(false);
    }
    
    //��ͼ�л�
    if (mapManager && player&&currentMap) {
       Vec2 direction;
      
      // CCLOG("player position %f,%f", player->getPlayerPosition().x, player->getPlayerPosition().y);
     //  CCLOG("screenSize %f,%f", screenSize.width, screenSize.height);

        if (mapManager->isAtEdge(player->getPlayerPosition(), direction)) { 
         //   CCLOG("player position %f,%f", player->getPlayerPosition().x, player->getPlayerPosition().y);
         //   CCLOG("screenSize %f,%f", screenSize.width, screenSize.height);
             mapManager->switchToBlock(direction); // �л���ͼ
             uiManager->setNpcVision();
           //����λ��
            const Vec2 vec = player->getPlayerPosition();
            if (direction == Vec2(-1, 0)) { // ��߽�
                
                player->setPlayerPosition(Vec2(screenSize.width-10, vec.y));
            }
            else if (direction == Vec2(1, 0)) { // �ұ߽�
                player->setPlayerPosition(Vec2(10, vec.y));
            }
            else if (direction == Vec2(0, -1)) { // �±߽�
                player->setPlayerPosition(Vec2(vec.x,screenSize.height-10));
            }
            else if (direction == Vec2(0, 1)) { // �ϱ߽�
                player->setPlayerPosition(Vec2(vec.x,10));
            }
          
        }

        int outsideLabel = mapManager->getCurrentBlockLabel();

        //���ż��
        if (mapManager->isEnterHouse(player->getPlayerPosition(), direction) && (outsideLabel == 1 || outsideLabel == 3 || outsideLabel == 2)) {
            if (outsideLabel == 1) {
                //������Ǽ�
                CCLOG("player enter house!");
                mapManager->switchToHouseOrOutside(5); // �л������ͼ
                uiManager->setNpcVision();
                //����λ��
                player->setPlayerPosition(Vec2(748, 236));
            }
            else if (outsideLabel == 3) {
                //��������̵�
                CCLOG("player enter shop!");
                mapManager->switchToHouseOrOutside(6); // �л��̵��ͼ
                uiManager->setNpcVision();
                //����λ��
                player->setPlayerPosition(Vec2(748, 236));
            }
            else if (outsideLabel == 2) {
                //������ǿ�
                CCLOG("player enter mine!");
                mapManager->switchToHouseOrOutside(7); // �л��󾮵�ͼ
                uiManager->setNpcVision();
                //����λ��
                player->setPlayerPosition(Vec2(748, 300));
            }
        }

        //�뿪�ż��
        if (mapManager->isExitHouse(player->getPlayerPosition(), direction) && (outsideLabel == 5 || outsideLabel == 6 || outsideLabel == 7)) {
            if (outsideLabel == 6) {
                //�뿪�����̵�
                CCLOG("player exit shop!");
                mapManager->switchToHouseOrOutside(3); // �л��������ͼ
                uiManager->setNpcVision();
                //����λ��
                player->setPlayerPosition(Vec2(1000, 100));
            }
            else if (outsideLabel == 5) {
                //�뿪���Ǽ�
                CCLOG("player exit house!");
                mapManager->switchToHouseOrOutside(1); // �л�һ�����ͼ
                uiManager->setNpcVision();
                //����λ��
                player->setPlayerPosition(Vec2(680, 200));
            }
            else if (outsideLabel == 7) {
                //�뿪���ǿ�
                CCLOG("player exit mine!");
                mapManager->switchToHouseOrOutside(2); // �л��������ͼ
                uiManager->setNpcVision();
                //����λ��
                player->setPlayerPosition(Vec2(500, 500));
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

    for (int i = 0; i < myCrops.size(); i++) {
        if (myCrops[i]) {
            myCrops[i]->update(dt);
        }
    }
  
}

void GameScene::removeCrop() {
    for (int i = 0; i < myCrops.size(); i++) {
        if (myCrops[i]) {
            myCrops[i]->removeFromParentAndCleanup(true);
            myCrops[i] = nullptr;  // ��ָ������Ϊ nullptr
           // isCropAlive = false;  // ���±�־Ϊ false
        }
    }
}

void GameScene::onCropTextureChanged(const std::string& texturePath) {
    // ���� texturePath ���� GameScene �е���ʾ
    // ���� texturePath ���� GameScene �е���ʾ
    for (int i = 0; i < myCrops.size(); i++) {
        if (myCrops[i]) {
            auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(texturePath);
            if (texture) {
                myCrops[i]->setTexture(texture);  // ���� myCrop ������
            }
        }
    }
    CCLOG("Crop texture changed to: %s", texturePath.c_str());
    CCLOG("Crop texture changed to: %s", texturePath.c_str());
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
void GameScene::initKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        auto P = getPlayer();
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_E:
            togglePause();
            break;
        case EventKeyboard::KeyCode::KEY_M:
            if (uiManager) 
                uiManager->toggleMiniMap(player->getPlayerPosition(),mapManager->getCurrentMapSize(1) ); // ��ʾ������ Mini Map
            break;
        case EventKeyboard::KeyCode::KEY_F:
            if (uiManager) 
                uiManager->toggleTaskList(); // ��ʾ�����������б�
            break;
        case EventKeyboard::KeyCode::KEY_P:
            if (P)
                P->openPlayerMenu();
            break;
        default:
            break;
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}
void GameScene::togglePause() {
    if (isGamePaused) {
        // �ָ���Ϸ
        Director::getInstance()->resume();
        uiManager->hidePausePanel(); // ������ͣ���
        isGamePaused = false;
        CCLOG("Game Resumed");
    }
    else {
        // ��ͣ��Ϸ
        Director::getInstance()->pause();
        uiManager->showPausePanel(); // ��ʾ��ͣ���
        isGamePaused = true;
        CCLOG("Game Paused");
    }
}