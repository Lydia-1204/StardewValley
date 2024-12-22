/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.cpp
 * File Function: 实现Player类，实现玩家的行走、砍、精力更新等动作
 * Author:        邓语乐 2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#include "Player.h"
#include "tool.h"
#include "NPC.h"
#include <random>
#include <chrono>
USING_NS_CC;

std::random_device rd;
std::mt19937 gen(rd());
std::uniform_real_distribution<> dis(0.0, 1.0);

Player* Player::instance = nullptr;     // 单例实例


Player::Player()
    : _sprite{ nullptr }, _velocity(Vec2::ZERO), _energy(100), _currentTool(0),
    _speed(100.0f), _isMoving(false), _nickname("Player"), _selectedCharacter(1),
    _toolSprite(nullptr), _shiftKeyPressed(false), _isToolActive(false),
    tool(nullptr), toolManager(nullptr), Planting_Skills(0), Breeding_Skills(0),
    Mining_Skills(0), Fishing_Skills(0)
{}

Player::~Player() {}

Player* Player::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {
        instance = new (std::nothrow) Player();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;
}

Player* Player::create(int selectedCharacter, const std::string& nickname) {
    Player* player = new (std::nothrow) Player();
    if (player && player->init(selectedCharacter, nickname)) {
        player->autorelease();
        return player;
    }
    delete player;
    return nullptr;
}

bool Player::init(int selectedCharacter, const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

    _nickname = nickname;
    _selectedCharacter = selectedCharacter;

    // 根据角色加载不同的资源
    std::string texturePath = (_selectedCharacter == 1)
        ? "../Resources/Amily.png"
        : "../Resources/Harvey.png";

    // 创建人物角色的四个方向
    _sprite[0] = Sprite::create(texturePath, Rect(0, 0, 17, 32));
    _sprite[1] = Sprite::create(texturePath, Rect(0, 33, 17, 32));
    _sprite[2] = Sprite::create(texturePath, Rect(0, 65, 17, 32));
    _sprite[3] = Sprite::create(texturePath, Rect(0, 98, 17, 32));

    // 这里忘记添加  未添加成功就报错  了！
    _sprite[0]->setScale(1.4f);
    _sprite[1]->setScale(1.4f);
    _sprite[2]->setScale(1.4f);
    _sprite[3]->setScale(1.4f);
    this->addChild(_sprite[0]);
    this->addChild(_sprite[1]);
    _sprite[1]->setVisible(0);
    this->addChild(_sprite[2]);
    _sprite[2]->setVisible(0);
    this->addChild(_sprite[3]);
    _sprite[3]->setVisible(0);

    // 初始显示正面精灵
    _sprite[0]->setVisible(true);

    // 添加工具精灵
    _toolSprite = Sprite::create();
    this->addChild(_toolSprite);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 设置鼠标点击事件监听器
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this); // 设置鼠标点击事件回调
    listener->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this); // 设置鼠标点击事件回调
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    // 增加鼠标移动监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(Player::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // 定时更新玩家状态
    schedule(CC_SCHEDULE_SELECTOR(Player::update), 1.0f / 60.0f);

    return true;
}

void Player::updateToolSprite() {
    toolManager = ToolManager::getInstance(_selectedCharacter, _nickname);
    // 根据工具类型设置对应的图片
    // CCLOG("index,%d", toolManager->selectedToolIndex);
    if (toolManager && toolManager->selectedToolIndex >= 0 &&
        toolManager->selectedToolIndex < toolManager->tools.size()) {
        switch (toolManager->tools[toolManager->selectedToolIndex]->getType()) {
        case Tool::ToolType::HOE:
            _toolSprite->setTexture("../Resources/tools/HOE.png");
            setVisible(true);
            break;
        case Tool::ToolType::AXE:
            _toolSprite->setTexture("../Resources/tools/AXE.png");
            setVisible(true);
            break;
        case Tool::ToolType::WATERING_CAN:
            _toolSprite->setTexture("../Resources/tools/WATERING_CAN.png");
            setVisible(true);
            break;
        case Tool::ToolType::FISHING_ROD:
            _toolSprite->setTexture("../Resources/tools/FISHING_ROD.png");
            setVisible(true);
            break;
        case Tool::ToolType::FERTILIZER:
            _toolSprite->setTexture("../Resources/tools/fertilizeruse.png");
            setVisible(true);
            break;
        case Tool::ToolType::HOEPLUS:
            _toolSprite->setTexture("../Resources/tools/HOE++.png");
            setVisible(true);
            break;
        case Tool::ToolType::AXEPLUS:
            _toolSprite->setTexture("../Resources/tools/AXE++.png");
            setVisible(true);
            break;
        case Tool::ToolType::WATERING_CANPLUS:
            _toolSprite->setTexture("../Resources/tools/WATERING_CAN++.png");
            setVisible(true);
            break;
        case Tool::ToolType::FISHING_RODPLUS:
            _toolSprite->setTexture("../Resources/tools/FISHING_ROD++.png");
            setVisible(true);
            break;
        case Tool::ToolType::NONE:
        default:
            _toolSprite->setTexture(""); // 清空纹理
            break;
        }
    }
}

void Player::setPlayerPosition(cocos2d::Vec2& position) {
    _sprite[_currentDirection]->setPosition(position);
    Node::setPosition(position);  // 调用父类的 setPosition 设置 Player 节点位置

    // 同步当前方向的精灵位置到 Player 节点的位置
    if (_sprite[_currentDirection]) {
        _sprite[_currentDirection]->setPosition(Vec2::ZERO);  // 将精灵对齐到 Player 节点
    }
}

void Player::setEnergy(int energy) {
    _energy = std::max(0, energy);
}

int Player::getEnergy() const {
    return _energy;
}

void Player::update(float delta) {
    move(delta);
    updateEnergy(delta);
    if (_isToolActive) {
        // 如果工具激活，更新工具的位置
        _toolSprite->setPosition(_sprite[_currentDirection]->getPosition());
    }
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    _keysPressed.insert(keyCode);

    if (keyCode == EventKeyboard::KeyCode::KEY_SHIFT) {
        _shiftKeyPressed = true;  // Shift 键按下
    }

    // 根据按键更新速度
    if (keyCode == EventKeyboard::KeyCode::KEY_W) {
        _velocity.y = 1;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_S) {
        _velocity.y = -1;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_A) {
        _velocity.x = -1;
    }
    else if (keyCode == EventKeyboard::KeyCode::KEY_D) {
        _velocity.x = 1;
    }
}

void Player::onKeyReleased(EventKeyboard::KeyCode keyCode, Event* event) {
    _keysPressed.erase(keyCode);

    if (keyCode == EventKeyboard::KeyCode::KEY_SHIFT) {
        _shiftKeyPressed = false;  // Shift 键释放
    }

    // 停止移动方向
    if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_S) {
        _velocity.y = 0;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D) {
        _velocity.x = 0;
    }
}

void Player::onMouseDown(Event* event) {
    // 获取所有 NPC
    auto npcManager = NPC::getInstance();
    const auto& npcs = npcManager->getAllNPCs();

    updateToolSprite();
    // 获取 UIManager 实例
    uiManager = UIManager::getInstance(_selectedCharacter, _nickname);
    // 获取 toolManager 实例
    toolManager = ToolManager::getInstance(_selectedCharacter, _nickname);

    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    //NPC监听****************************************************************************************//
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {

        auto now = std::chrono::steady_clock::now();

        // 检查是否是双击
        if (wasDoubleClick) {
            wasDoubleClick = false; // 重置双击标记
        }
        else {
            // 标记为双击可能
            wasDoubleClick = true;

            // 检查两次点击的时间间隔，确定是否为双击
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count() < 500) {
                wasDoubleClick = true; // 时间间隔小于500毫秒，视为双击
            }
            else {
                wasDoubleClick = false; // 时间间隔大于500毫秒，不是双击
            }

            // 如果时间间隔不超过500毫秒，执行送礼物逻辑
            if (wasDoubleClick) {
                for (auto npc : npcs) {
                    if (npc && npc->getAffection() < 100 &&
                        getPlayerPosition().distance(npc->getPosition()) < 50.0f) {
                        int currentAffection = npc->getAffection();
                        npc->setAffection(currentAffection + 10);

                        // 限制好感度最大值为100
                        if (npc->getAffection() > 100) npc->setAffection(100);

                        CCLOG("give gift!!!!!!!：%d", npc->getAffection());
                        return; // 送完礼物后退出
                    }
                }
            }

            // 更新上次点击时间
            lastClickTime = now;
        }


        if (uiManager && uiManager->chatSprite && uiManager->dialogTextLabel) {
            if (uiManager->chatSprite->isVisible() && uiManager->dialogTextLabel->isVisible()) {
                // 如果对话框或对话文本可见，点击屏幕将其隐藏
                uiManager->chatSprite->setVisible(false);
                uiManager->dialogTextLabel->setVisible(false);
                CCLOG("Dialog and text label hidden.");
                return; // 直接返回，避免触发其他逻辑
            }
        }

    
        // 获取鼠标点击的位置
        Vec2 mousePosition = mouseEvent->getLocation();

        // 检查是否点击了 NPC(单击聊天）
        for (auto npc : npcs) {
            if (!npc) continue;

            Rect npcBoundingBox = npc->getBoundingBox();
            if (getPlayerPosition().distance(npc->getPosition()) < 50.0f) {
                CCLOG("click!!!");
                showDialogBox(npc, "general");
                break;
            }
        }

 // 鼠标按下时开始工具激活
        _isToolActive = true;
        if (toolManager && toolManager->selectedToolIndex >= 0 &&
            toolManager->selectedToolIndex < toolManager->tools.size())
            toolManager->useTool();

        // 设置 tool 精灵初始位置
        _toolSprite->setPosition(_sprite[_currentDirection]->getPosition());
        _toolSprite->setVisible(true); // 显示工具精灵

        // 开始定时显示和隐藏工具精灵
        schedule([this](float dt) {
            if (_isToolActive) {
                bool isVisible = _toolSprite->isVisible();
                _toolSprite->setVisible(!isVisible); // 切换显示/隐藏

                toolManager->useTool();
                _energy = uiManager->getCurrentEnergy();

                // 每次调用减少 1 点精力
                if (_toolSprite->isVisible() && _toolSprite)
                    uiManager->setCurrentEnergy(_energy - 1);

                CCLOG("Current Energy: %d", _energy);
            }
            }, 0.4f, "toggle_axe_key"); // 每 0.4 秒切换一次
    }
    //youjina
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        // 获取鼠标点击的屏幕坐标（这是一个 Vec2 对象）
        Vec2 mousePosition = mouseEvent->getLocation();

        // 判断是否点击了 NPC
        for (NPC* npc : npcs) {
            float distance = mousePosition.distance(npc->getPosition());
            if (distance < 80.0f) {
                // 点击到了 NPC，显示好感度面板
                npc->showAffectionBox();

                // 不调用工具激活部分的代码，直接返回
                _isToolActive = false;
                _toolSprite->setVisible(false);
                return;
            }
        }
      
    }
    /***************************************************************************************/
}

void Player::onMouseUp(Event* event) {
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        // 鼠标释放时停止工具激活
        _isToolActive = false;
        // 停止斧头显示
        _toolSprite->setVisible(false);
        // 停止定时器
        unschedule("toggle_axe_key");
    }
}

void Player::onMouseMove(Event* event) {
    // 获取 UIManager 实例
    uiManager = UIManager::getInstance(_selectedCharacter, _nickname);

    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    if (!mouseEvent) return;

    // 获取窗口坐标系中的鼠标位置
    Vec2 mousePosInView = mouseEvent->getLocationInView();

    // 转换为左下角原点的坐标系
    float screenHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;
    Vec2 mousePosWithBottomLeftOrigin = Vec2(mousePosInView.x, screenHeight - mousePosInView.y);

    // 转换为设计分辨率下的逻辑坐标
    Vec2 mousePosInDesign = Director::getInstance()->convertToGL(mousePosWithBottomLeftOrigin);

    // 转换为地图上的实际坐标
    auto mapNode = this->getParent(); // 获取地图或场景的节点
    Vec2 mousePosInMap = mapNode->convertToNodeSpace(mousePosInDesign);

    bool foundNpcUnderMouse = false;

    // 获取所有 NPC
    auto npcManager = NPC::getInstance();
    const auto& npcs = npcManager->getAllNPCs();

    for (auto npc : npcs) {
        if (!npc) continue;

        // 获取 NPC 的包围框
        Rect npcWorldBoundingBox = npc->getBoundingBox();

        // 判断鼠标是否在 NPC 包围框内
        if (npcWorldBoundingBox.containsPoint(mousePosInDesign)) {
            // 鼠标在NPC上   
            updateDialogPosition(npc);
            float distance = getPlayerPosition().distance(npc->getPosition());
            if (distance < 50.0f) {
                // 离得近：不透明
                uiManager->dialogSprite->setOpacity(NO_TRANSPARENT);
            }
            else {
                // 离得远：半透明
                uiManager->dialogSprite->setOpacity(HALF_TRANSPARENT);
            }

            foundNpcUnderMouse = true;
            break;
        }
    }
    for (auto npc : npcs)
        if (!foundNpcUnderMouse) {
            uiManager->dialogSprite->setOpacity(0); // 隐藏对话框
        }
}

Vec2 Player::getPlayerPosition() const {
    // 返回第一个精灵的位置，假设玩家的位置由该精灵的位置来确定
  //  CCLOG("before back position,%f,%f", currentPosition.x, currentPosition.y);
    return this->getPosition();  // 直接返回 Player 节点的坐标
}

void Player::move(float delta) {
    Vec2 currentPosition = getPlayerPosition();
    float moveSpeed = _shiftKeyPressed ? _speed / 2 : _speed;  // 如果Shift按下，速度减半

    if (_velocity != Vec2::ZERO) {
        // 处理方向的切换
        if (_velocity.x > 0) {
            if (_currentDirection != 1) {
                // 隐藏当前精灵
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // 更新当前方向为右
                _currentDirection = 1;
            }
        }
        else if (_velocity.x < 0) {
            if (_currentDirection != 3) {
                // 隐藏当前精灵
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // 更新当前方向为左
                _currentDirection = 3;
            }
        }
        else if (_velocity.y > 0) {
            if (_currentDirection != 2) {
                // 隐藏当前精灵
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // 更新当前方向为背面
                _currentDirection = 2;
            }
        }
        else if (_velocity.y < 0) {
            if (_currentDirection != 0) {
                // 隐藏当前精灵
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // 更新当前方向为正面
                _currentDirection = 0;
            }
        }

        //碰撞检测
        Vec2 displacement = _velocity.getNormalized() * moveSpeed * delta;
        if (!MapManager::getInstance()->isObstacleAtPosition(currentPosition + displacement)) {
            //不是障碍物才可以移动
            // 移动精灵
            setPlayerPosition(currentPosition + displacement);


        }
        // 显示当前精灵
        if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
            _sprite[_currentDirection]->setVisible(true);
        }
    }
}

void Player::interactWithMap() {
    // 地图交互逻辑实现
    //CCLOG("Interacting with map...");
}

void Player::setCurrentTool(int toolId) {
    _currentTool = toolId;
}

void Player::updateEnergy(float delta) {
    if (_isMoving) {
        _energy = std::max(0, _energy - static_cast<int>(delta * 10));  // 移动减少精力
    }
}

const std::string& Player::getNickname() const {
    return _nickname;
}

int Player::getSelectedCharacter() const {
    return _selectedCharacter;
}

// 计算对话框的位置
void Player::updateDialogPosition(NPC* npc) {
    // 获取 NPC 的位置（世界坐标）
    Vec2 npcWorldPosition = npc->getParent()->convertToWorldSpace(npc->getPosition());

    // 获取 NPC 的大小
    Size npcSize = npc->getContentSize();

    // 计算对话框的位置（相对于世界坐标）
    Vec2 dialogPosition = npcWorldPosition;

    // 如果对话框是相对于场景或地图的，直接设置对话框的位置
    uiManager->dialogSprite->setPosition(dialogPosition);
}

void Player::showDialogBox(NPC* npc, const std::string& category) {
    // 获取 NPC 的位置（世界坐标）
    Vec2 npcWorldPosition = npc->getParent()->convertToWorldSpace(npc->getPosition());

    // 获取 NPC 的大小
    Size npcSize = npc->getContentSize();

    // 获取 UIManager 实例并确保它已初始化
    if (!uiManager) {
        uiManager = UIManager::getInstance(_selectedCharacter, _nickname);  // 获取 UIManager 实例
    }

    std::string finalCategory = category; // 默认使用传入的类别
    std::string dialogue;                 // 对话内容

    // 获取 NPC 的对话列表
    const auto& taskDialogues = npc->getDialogues("task");
    const auto& generalDialogues = npc->getDialogues("general");
    const auto& relationshipFriendship = npc->getDialogues("relationship_friendship");
    const auto& relationshipRomantic = npc->getDialogues("relationship_romantic");

    // 动态决定对话类别
    if (npc->getAffection() >= 90 && !relationshipRomantic.empty()) {
        finalCategory = "relationship_romantic";
        dialogue = npc->getRandomDialogue(finalCategory);
    }
    else if (npc->getAffection() >= 60 && !relationshipFriendship.empty()) {
        finalCategory = "relationship_friendship";
        dialogue = npc->getRandomDialogue(finalCategory);
    }
    else {
        // 加权随机选择 "task" 或 "general"
        float taskWeight = 0.8f;  // 任务对话的权重
        srand(static_cast<unsigned>(time(0)));
        float randomValue = dis(gen);  // 生成 [0, 1) 的随机值
        CCLOG("random=%f", randomValue);
        if (randomValue < taskWeight && !taskDialogues.empty()) {
            finalCategory = "task";
            dialogue = npc->getRandomDialogue(finalCategory);
        }
        else if (!generalDialogues.empty()) {
            finalCategory = "general";
            dialogue = npc->getRandomDialogue(finalCategory);
        }
        else if (!taskDialogues.empty()) {
            // 如果 general 没有对话，但 task 存在，使用 task 对话
            finalCategory = "task";
            dialogue = npc->getRandomDialogue(finalCategory);
        }
        else {
            dialogue = "I have nothing to say."; // 当无对话可用时的默认内容
        }
    }

    // 如果是任务对话，移除已显示的任务
    if (finalCategory == "task") {
        CCLOG("remove");
        CCLOG("text:%s", dialogue);
        npc->removeDialogue("task", dialogue); // 从任务列表中移除当前任务
    }

    // 确保 uiManager 和 dialogSprite、dialogTextLabel 已正确初始化
    if (uiManager && uiManager->dialogSprite && uiManager->dialogTextLabel) {
        uiManager->chatSprite->setVisible(true); // 显示对话框

        // 设置对话框文本
        uiManager->dialogTextLabel->setString(dialogue);  // 使用 setString 更新文本
        uiManager->dialogTextLabel->setVisible(true); // 显示文字
    }
    else {
        // 如果 uiManager 或 dialogSprite 或 dialogTextLabel 没有初始化，输出日志
        CCLOG("uiManager or dialogSprite or dialogTextLabel is not properly initialized!");
    }
}

void Player::openPlayerMenu() {
    CCLOG("Into player menu!");

    // 获取当前屏幕的可视区域大小
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 创建一个层作为菜单的容器
    menuLayer = Layer::create();
    menuLayer->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));

    // 创建背景图片精灵，并设置锚点为(0.5, 0.5)以居中显示
    auto background = Sprite::create("an_menu.png");
    background->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(2.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(background);

    // 创建一个标签，显示标题
    auto label = Label::createWithTTF("Player Information", "fonts/Marker Felt.ttf", 30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4 + 196)); // 调整标签位置
    menuLayer->addChild(label);

    // 创建一个按钮，点击后关闭菜单
    auto closeBtn = MenuItemImage::create(
        "close.png", // 正常状态的图片
        "close.png", // 选中状态的图片
        CC_CALLBACK_1(Player::closePlayerMenu, this));
    closeBtn->setScale(2.5); // 设置图像大小为原来的1.5倍
    closeBtn->setPosition(Vec2(visibleSize.width / 4 + 65, visibleSize.height / 4 + 32)); // 调整关闭按钮位置

    // 创建一个菜单对象，并添加关闭按钮
    auto menu = Menu::create(closeBtn, NULL);
    menu->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    menuLayer->addChild(menu);

    Sprite* playerImage;
    if (_selectedCharacter == 1) {
        playerImage = Sprite::create("player1.png");
    }
    else {
        playerImage = Sprite::create("player2.png");
    }
    playerImage->setPosition(Vec2(380 - 300, 360 - 175)); // 调整详细信息图片位置
    playerImage->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为图像中心
    playerImage->setScale(4.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(playerImage);


    auto labelPlanting = Label::createWithTTF("Planting_Skills: " + std::to_string(this->getPlanting()) + " / 100", "fonts/Marker Felt.ttf", 50);
    if (this->getPlanting() != 100)
        labelPlanting->setColor(Color3B::WHITE);
    else
        labelPlanting->setColor(Color3B::RED);
    labelPlanting->setPosition(Vec2(590 - 200, 445 - 175));
    menuLayer->addChild(labelPlanting);

    auto labelBreeding = Label::createWithTTF("Breeding_Skills: " + std::to_string(this->getBreeding()) + " / 100", "fonts/Marker Felt.ttf", 50);
    if (this->getBreeding() != 100)
        labelBreeding->setColor(Color3B::WHITE);
    else
        labelBreeding->setColor(Color3B::RED);
    labelBreeding->setPosition(Vec2(590 - 200, 385 - 175));
    menuLayer->addChild(labelBreeding);

    auto labelMining = Label::createWithTTF("Mining_Skills: " + std::to_string(this->getMining()) + " / 100", "fonts/Marker Felt.ttf", 50);
    if (this->getMining() != 100)
        labelMining->setColor(Color3B::WHITE);
    else
        labelMining->setColor(Color3B::RED);
    labelMining->setPosition(Vec2(590 - 200, 320 - 175));
    menuLayer->addChild(labelMining);

    auto labelFishing = Label::createWithTTF("Fishing_Skills: " + std::to_string(this->getFishing()) + " / 100", "fonts/Marker Felt.ttf", 50);
    if (this->getFishing() != 100)
        labelFishing->setColor(Color3B::WHITE);
    else
        labelFishing->setColor(Color3B::RED);
    labelFishing->setPosition(Vec2(590 - 200, 260 - 175));
    menuLayer->addChild(labelFishing);

    // 将菜单层添加到场景
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(menuLayer, 20); // 设置一个合适的层级，确保菜单层在最上面

}

void Player::closePlayerMenu(Ref* sender) {
    // 关闭菜单的逻辑
    if (menuLayer) {
        menuLayer->removeFromParentAndCleanup(true); // 移除菜单层
        menuLayer = nullptr; // 清除引用
    }
}