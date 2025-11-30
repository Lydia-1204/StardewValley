/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.cpp
 * File Function: 实现GameScene类，展示游戏主界面和基本功能：实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI 
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 *********************************************************************************************************/

#include "Scenes/GameScene.h"
#include "Scenes/MenuScene.h" // �˵�����
#include "Scenes/CreateErrorScene.h"
#include "Characters/NpcTemplate.h"

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

        // 初始化地图管理器
        mapManager = MapManager::getInstance();
        //CCLOG("Map Manager created");
        mapManager->loadMapBlocks("map"); // 加载第一个地图块
        CCLOG("MapManager getInstance");
        this->addChild(mapManager);


        //this->addChild(currentMap); mapManager内add 不要重复add

        //CCLOG("currentMap : %p ", currentMap);
        CCLOG("currentMap getCurrentBlock  created");


        // 初始化玩家 selectedCharacter
        player = Player::getInstance(selectedCharacter, nickname);

        player->setPlayerPosition(mapManager->getPlayerStartPos()); // 初始位置
        player->setPosition(200, 350);
        this->addChild(player);
        CCLOG("Player created");


        
        // 初始化 UI 管理器
        uiManager = UIManager::getInstance(selectedCharacter, nickname);
        this->addChild(uiManager->getLayer());
        CCLOG("UIManager created");
        

        //箱子
        
        iron = Sprite::create("../Resources/boxxx.png");
        if (!iron)
            throw("iron created failed!!");
        iron->setPosition(Vec2(230,430));
        this->addChild(iron, 20);
        iron->setVisible(false);
        //箱子lei

        auto chest = Chest::getInstance();
        if (chest == nullptr) {
            throw std::runtime_error("Chest create failed!!");
            return false;
        }


        this->addChild(chest);
        chest->isOpen = 0;
        chest->chestSetPosition(Vec2(230, 430));

        //工具

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

        //物品
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
        //toolManager->addTool(Tool::ToolType::HOE);二次相加


        // 初始化各类型的小动物
        chicken = Animal::create(1, this);
        chicken->setPosition(mapManager->getChickenStartPos()); // 初始位置
        this->addChild(chicken);
        CCLOG("Chicken created");
        chicken->initialmove("Chicken.png");

        cow = Animal::create(2, this);
        cow->setPosition(mapManager->getCowStartPos()); // 初始位置
        this->addChild(cow);
        CCLOG("Cow created");
        cow->initialmove("Cow.png");

        cat = Animal::create(3, this);
        cat->setPosition(mapManager->getCatStartPos()); // 初始位置
        this->addChild(cat);
        CCLOG("Cat created");
        cat->initialmove("Cat.png");

        dog = Animal::create(4, this);
        dog->setPosition(mapManager->getDogStartPos()); // 初始位置
        this->addChild(dog);
        CCLOG("Dog created");
        dog->initialmove("Dog.png");

        pig = Animal::create(5, this);
        pig->setPosition(mapManager->getPigStartPos()); // 初始位置
        this->addChild(pig);
        CCLOG("Pig created");
        pig->initialmove("Pig.png");

        sheep = Animal::create(6, this);
        sheep->setPosition(mapManager->getSheepStartPos()); // 初始位置
        this->addChild(sheep);
        CCLOG("Sheep created");
        sheep->initialmove("Sheep.png");
        
        // 初始化种植
        // 创建一个新的 Crop 实例
        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 6; j++) {

                Crop* crop = Crop::create(this, "crop");
                crop->setPosition(mapManager->getCropStartPos().x + i * 16, mapManager->getCropStartPos().y - j * 16);  // 设置作物的位置
                myCrops.push_back(crop); // 传递this指针给Crop
                this->addChild(crop);  // 将作物添加到场景中 
                // 设置回调函数
                CCLOG("Crop %d created", i);
                crop->setTextureChangedCallback([this](const std::string& texturePath) {
                    onCropTextureChanged(texturePath);
                    //  isCropAlive = true;  // 初始化标志为 true

                    });
            }
        }
      
     
       // 初始化睡眠面板
        sleepPanel = LayerColor::create(Color4B(20, 20, 20, 220)); // 深色且几乎不透明
        sleepPanel->setVisible(false);
        this->addChild(sleepPanel);
        // 创建一个标签，显示标题
        auto label = Label::createWithTTF("Sleeping...", "fonts/Marker Felt.ttf", 100);
        label->setColor(Color3B::WHITE);
        label->setPosition(Vec2(520, 250)); // 调整标签位置
        sleepPanel->addChild(label);
       
        // 监听shubiao
        auto listener = EventListenerMouse::create();
        listener->onMouseDown = [=](EventMouse* event) {
            auto playerPos = player->getPosition();
            auto chestPos = chest->getPosition();
            auto locationInWorld = event->getLocationInView();  // 获取屏幕视图中的坐标
            auto locationInChestBg = this->convertToNodeSpace(locationInWorld); // 转换到 tools_bg 的坐标系
            // 玩家靠近箱子并点击时，交互
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
        //捕获异常 记录日志
        CCLOG("Exception caught : %s", e.what());
        //加载备用错误界面
        auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
        Director::getInstance()->replaceScene(errorScene);
    }

    initKeyboardListener();
    // 注册 update 方法
   this->scheduleUpdate();

    return true;
}
void GameScene::update(float dt) {
    // 更新游戏逻辑
    if (MapManager::getInstance()->currentMapLabel == 2)
    {
        iron->setVisible(true);
    }
    else{
        iron->setVisible(false);
    }
    
    //地图切换
    if (mapManager && player&&currentMap) {
       Vec2 direction;
      
      // CCLOG("player position %f,%f", player->getPlayerPosition().x, player->getPlayerPosition().y);
     //  CCLOG("screenSize %f,%f", screenSize.width, screenSize.height);

        if (mapManager->isAtEdge(player->getPlayerPosition(), direction)) { 
         //   CCLOG("player position %f,%f", player->getPlayerPosition().x, player->getPlayerPosition().y);
         //   CCLOG("screenSize %f,%f", screenSize.width, screenSize.height);
             mapManager->switchToBlock(direction); // 切换地图
             uiManager->setNpcVision();
           //重置位置
            const Vec2 vec = player->getPlayerPosition();
            if (direction == Vec2(-1, 0)) { // 左边界
                
                player->setPlayerPosition(Vec2(screenSize.width-10, vec.y));
            }
            else if (direction == Vec2(1, 0)) { // 右边界
                player->setPlayerPosition(Vec2(10, vec.y));
            }
            else if (direction == Vec2(0, -1)) { // 下边界
                player->setPlayerPosition(Vec2(vec.x,screenSize.height-10));
            }
            else if (direction == Vec2(0, 1)) { // 上边界
                player->setPlayerPosition(Vec2(vec.x,10));
            }
          
        }

        int outsideLabel = mapManager->getCurrentBlockLabel();

        //进门检测
        if (mapManager->isEnterHouse(player->getPlayerPosition(), direction) && (outsideLabel == 1 || outsideLabel == 3 || outsideLabel == 2)) {
            if (outsideLabel == 1) {
                //进入的是家
                CCLOG("player enter house!");
                mapManager->switchToHouseOrOutside(5); // 切换家里地图
                uiManager->setNpcVision();
                //重置位置
                player->setPlayerPosition(Vec2(748, 236));
            }
            else if (outsideLabel == 3) {
                //进入的是商店
                CCLOG("player enter shop!");
                mapManager->switchToHouseOrOutside(6); // 切换商店地图
                uiManager->setNpcVision();
                //重置位置
                player->setPlayerPosition(Vec2(748, 236));
            }
            else if (outsideLabel == 2) {
                //进入的是矿井
                CCLOG("player enter mine!");
                mapManager->switchToHouseOrOutside(7); // 切换矿井地图
                uiManager->setNpcVision();
                //重置位置
                player->setPlayerPosition(Vec2(748, 300));
            }
        }

        //离开门检测
        if (mapManager->isExitHouse(player->getPlayerPosition(), direction) && (outsideLabel == 5 || outsideLabel == 6 || outsideLabel == 7)) {
            if (outsideLabel == 6) {
                //离开的是商店
                CCLOG("player exit shop!");
                mapManager->switchToHouseOrOutside(3); // 切换三号外地图
                uiManager->setNpcVision();
                //重置位置
                player->setPlayerPosition(Vec2(1000, 100));
            }
            else if (outsideLabel == 5) {
                //离开的是家
                CCLOG("player exit house!");
                mapManager->switchToHouseOrOutside(1); // 切换一号外地图
                uiManager->setNpcVision();
                //重置位置
                player->setPlayerPosition(Vec2(680, 200));
            }
            else if (outsideLabel == 7) {
                //离开的是矿井
                CCLOG("player exit mine!");
                mapManager->switchToHouseOrOutside(2); // 切换二号外地图
                uiManager->setNpcVision();
                //重置位置
                player->setPlayerPosition(Vec2(500, 500));
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
            myCrops[i] = nullptr;  // 将指针设置为 nullptr
           // isCropAlive = false;  // 更新标志为 false
        }
    }
}

void GameScene::onCropTextureChanged(const std::string& texturePath) {
    // 根据 texturePath 更新 GameScene 中的显示
    // 根据 texturePath 更新 GameScene 中的显示
    for (int i = 0; i < myCrops.size(); i++) {
        if (myCrops[i]) {
            auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(texturePath);
            if (texture) {
                myCrops[i]->setTexture(texture);  // 更新 myCrop 的纹理
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
                uiManager->toggleMiniMap(player->getPlayerPosition(),mapManager->getCurrentMapSize(1) ); // 显示或隐藏 Mini Map
            break;
        case EventKeyboard::KeyCode::KEY_F:
            if (uiManager) 
                uiManager->toggleTaskList(); // 显示或隐藏任务列表
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
        // 恢复游戏
        Director::getInstance()->resume();
        uiManager->hidePausePanel(); // 隐藏暂停面板
        isGamePaused = false;
        CCLOG("Game Resumed");
    }
    else {
        // 暂停游戏
        Director::getInstance()->pause();
        uiManager->showPausePanel(); // 显示暂停面板
        isGamePaused = true;
        CCLOG("Game Paused");
    }
}
