/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.cpp
 * File Function: ʵ��Player�࣬ʵ����ҵ����ߡ������������µȶ���
 * Author:        ������ 2351273
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

Player* Player::instance = nullptr;     // ����ʵ��


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

    // ���ݽ�ɫ���ز�ͬ����Դ
    std::string texturePath = (_selectedCharacter == 1)
        ? "../Resources/Amily.png"
        : "../Resources/Harvey.png";

    // ���������ɫ���ĸ�����
    _sprite[0] = Sprite::create(texturePath, Rect(0, 0, 17, 32));
    _sprite[1] = Sprite::create(texturePath, Rect(0, 33, 17, 32));
    _sprite[2] = Sprite::create(texturePath, Rect(0, 65, 17, 32));
    _sprite[3] = Sprite::create(texturePath, Rect(0, 98, 17, 32));

    // �����������  δ��ӳɹ��ͱ���  �ˣ�
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

    // ��ʼ��ʾ���澫��
    _sprite[0]->setVisible(true);

    // ��ӹ��߾���
    _toolSprite = Sprite::create();
    this->addChild(_toolSprite);

    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // ����������¼�������
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this); // ����������¼��ص�
    listener->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this); // ����������¼��ص�
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);


    // ��������ƶ�������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseMove = CC_CALLBACK_1(Player::onMouseMove, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // ��ʱ�������״̬
    schedule(CC_SCHEDULE_SELECTOR(Player::update), 1.0f / 60.0f);

    return true;
}

void Player::updateToolSprite() {
    toolManager = ToolManager::getInstance(_selectedCharacter, _nickname);
    // ���ݹ����������ö�Ӧ��ͼƬ
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
            _toolSprite->setTexture(""); // �������
            break;
        }
    }
}

void Player::setPlayerPosition(cocos2d::Vec2& position) {
    _sprite[_currentDirection]->setPosition(position);
    Node::setPosition(position);  // ���ø���� setPosition ���� Player �ڵ�λ��

    // ͬ����ǰ����ľ���λ�õ� Player �ڵ��λ��
    if (_sprite[_currentDirection]) {
        _sprite[_currentDirection]->setPosition(Vec2::ZERO);  // ��������뵽 Player �ڵ�
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
        // ������߼�����¹��ߵ�λ��
        _toolSprite->setPosition(_sprite[_currentDirection]->getPosition());
    }
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    _keysPressed.insert(keyCode);

    if (keyCode == EventKeyboard::KeyCode::KEY_SHIFT) {
        _shiftKeyPressed = true;  // Shift ������
    }

    // ���ݰ��������ٶ�
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
        _shiftKeyPressed = false;  // Shift ���ͷ�
    }

    // ֹͣ�ƶ�����
    if (keyCode == EventKeyboard::KeyCode::KEY_W || keyCode == EventKeyboard::KeyCode::KEY_S) {
        _velocity.y = 0;
    }
    if (keyCode == EventKeyboard::KeyCode::KEY_A || keyCode == EventKeyboard::KeyCode::KEY_D) {
        _velocity.x = 0;
    }
}

void Player::onMouseDown(Event* event) {
    // ��ȡ���� NPC
    auto npcManager = NPC::getInstance();
    const auto& npcs = npcManager->getAllNPCs();

    updateToolSprite();
    // ��ȡ UIManager ʵ��
    uiManager = UIManager::getInstance(_selectedCharacter, _nickname);
    // ��ȡ toolManager ʵ��
    toolManager = ToolManager::getInstance(_selectedCharacter, _nickname);

    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    //NPC����****************************************************************************************//
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {

        auto now = std::chrono::steady_clock::now();

        // ����Ƿ���˫��
        if (wasDoubleClick) {
            wasDoubleClick = false; // ����˫�����
        }
        else {
            // ���Ϊ˫������
            wasDoubleClick = true;

            // ������ε����ʱ������ȷ���Ƿ�Ϊ˫��
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count() < 500) {
                wasDoubleClick = true; // ʱ����С��500���룬��Ϊ˫��
            }
            else {
                wasDoubleClick = false; // ʱ��������500���룬����˫��
            }

            // ���ʱ����������500���룬ִ���������߼�
            if (wasDoubleClick) {
                for (auto npc : npcs) {
                    if (npc && npc->getAffection() < 100 &&
                        getPlayerPosition().distance(npc->getPosition()) < 50.0f) {
                        int currentAffection = npc->getAffection();
                        npc->setAffection(currentAffection + 10);

                        // ���ƺøж����ֵΪ100
                        if (npc->getAffection() > 100) npc->setAffection(100);

                        CCLOG("give gift!!!!!!!��%d", npc->getAffection());
                        return; // ����������˳�
                    }
                }
            }

            // �����ϴε��ʱ��
            lastClickTime = now;
        }


        if (uiManager && uiManager->chatSprite && uiManager->dialogTextLabel) {
            if (uiManager->chatSprite->isVisible() && uiManager->dialogTextLabel->isVisible()) {
                // ����Ի����Ի��ı��ɼ��������Ļ��������
                uiManager->chatSprite->setVisible(false);
                uiManager->dialogTextLabel->setVisible(false);
                CCLOG("Dialog and text label hidden.");
                return; // ֱ�ӷ��أ����ⴥ�������߼�
            }
        }

    
        // ��ȡ�������λ��
        Vec2 mousePosition = mouseEvent->getLocation();

        // ����Ƿ����� NPC(�������죩
        for (auto npc : npcs) {
            if (!npc) continue;

            Rect npcBoundingBox = npc->getBoundingBox();
            if (getPlayerPosition().distance(npc->getPosition()) < 50.0f) {
                CCLOG("click!!!");
                showDialogBox(npc, "general");
                break;
            }
        }

 // ��갴��ʱ��ʼ���߼���
        _isToolActive = true;
        if (toolManager && toolManager->selectedToolIndex >= 0 &&
            toolManager->selectedToolIndex < toolManager->tools.size())
            toolManager->useTool();

        // ���� tool �����ʼλ��
        _toolSprite->setPosition(_sprite[_currentDirection]->getPosition());
        _toolSprite->setVisible(true); // ��ʾ���߾���

        // ��ʼ��ʱ��ʾ�����ع��߾���
        schedule([this](float dt) {
            if (_isToolActive) {
                bool isVisible = _toolSprite->isVisible();
                _toolSprite->setVisible(!isVisible); // �л���ʾ/����

                toolManager->useTool();
                _energy = uiManager->getCurrentEnergy();

                // ÿ�ε��ü��� 1 �㾫��
                if (_toolSprite->isVisible() && _toolSprite)
                    uiManager->setCurrentEnergy(_energy - 1);

                CCLOG("Current Energy: %d", _energy);
            }
            }, 0.4f, "toggle_axe_key"); // ÿ 0.4 ���л�һ��
    }
    //youjina
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        // ��ȡ���������Ļ���꣨����һ�� Vec2 ����
        Vec2 mousePosition = mouseEvent->getLocation();

        // �ж��Ƿ����� NPC
        for (NPC* npc : npcs) {
            float distance = mousePosition.distance(npc->getPosition());
            if (distance < 80.0f) {
                // ������� NPC����ʾ�øж����
                npc->showAffectionBox();

                // �����ù��߼���ֵĴ��룬ֱ�ӷ���
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
        // ����ͷ�ʱֹͣ���߼���
        _isToolActive = false;
        // ֹͣ��ͷ��ʾ
        _toolSprite->setVisible(false);
        // ֹͣ��ʱ��
        unschedule("toggle_axe_key");
    }
}

void Player::onMouseMove(Event* event) {
    // ��ȡ UIManager ʵ��
    uiManager = UIManager::getInstance(_selectedCharacter, _nickname);

    EventMouse* mouseEvent = dynamic_cast<EventMouse*>(event);
    if (!mouseEvent) return;

    // ��ȡ��������ϵ�е����λ��
    Vec2 mousePosInView = mouseEvent->getLocationInView();

    // ת��Ϊ���½�ԭ�������ϵ
    float screenHeight = Director::getInstance()->getOpenGLView()->getFrameSize().height;
    Vec2 mousePosWithBottomLeftOrigin = Vec2(mousePosInView.x, screenHeight - mousePosInView.y);

    // ת��Ϊ��Ʒֱ����µ��߼�����
    Vec2 mousePosInDesign = Director::getInstance()->convertToGL(mousePosWithBottomLeftOrigin);

    // ת��Ϊ��ͼ�ϵ�ʵ������
    auto mapNode = this->getParent(); // ��ȡ��ͼ�򳡾��Ľڵ�
    Vec2 mousePosInMap = mapNode->convertToNodeSpace(mousePosInDesign);

    bool foundNpcUnderMouse = false;

    // ��ȡ���� NPC
    auto npcManager = NPC::getInstance();
    const auto& npcs = npcManager->getAllNPCs();

    for (auto npc : npcs) {
        if (!npc) continue;

        // ��ȡ NPC �İ�Χ��
        Rect npcWorldBoundingBox = npc->getBoundingBox();

        // �ж�����Ƿ��� NPC ��Χ����
        if (npcWorldBoundingBox.containsPoint(mousePosInDesign)) {
            // �����NPC��   
            updateDialogPosition(npc);
            float distance = getPlayerPosition().distance(npc->getPosition());
            if (distance < 50.0f) {
                // ��ý�����͸��
                uiManager->dialogSprite->setOpacity(NO_TRANSPARENT);
            }
            else {
                // ���Զ����͸��
                uiManager->dialogSprite->setOpacity(HALF_TRANSPARENT);
            }

            foundNpcUnderMouse = true;
            break;
        }
    }
    for (auto npc : npcs)
        if (!foundNpcUnderMouse) {
            uiManager->dialogSprite->setOpacity(0); // ���ضԻ���
        }
}

Vec2 Player::getPlayerPosition() const {
    // ���ص�һ�������λ�ã�������ҵ�λ���ɸþ����λ����ȷ��
  //  CCLOG("before back position,%f,%f", currentPosition.x, currentPosition.y);
    return this->getPosition();  // ֱ�ӷ��� Player �ڵ������
}

void Player::move(float delta) {
    Vec2 currentPosition = getPlayerPosition();
    float moveSpeed = _shiftKeyPressed ? _speed / 2 : _speed;  // ���Shift���£��ٶȼ���

    if (_velocity != Vec2::ZERO) {
        // ��������л�
        if (_velocity.x > 0) {
            if (_currentDirection != 1) {
                // ���ص�ǰ����
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // ���µ�ǰ����Ϊ��
                _currentDirection = 1;
            }
        }
        else if (_velocity.x < 0) {
            if (_currentDirection != 3) {
                // ���ص�ǰ����
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // ���µ�ǰ����Ϊ��
                _currentDirection = 3;
            }
        }
        else if (_velocity.y > 0) {
            if (_currentDirection != 2) {
                // ���ص�ǰ����
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // ���µ�ǰ����Ϊ����
                _currentDirection = 2;
            }
        }
        else if (_velocity.y < 0) {
            if (_currentDirection != 0) {
                // ���ص�ǰ����
                if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
                    _sprite[_currentDirection]->setVisible(false);
                }
                // ���µ�ǰ����Ϊ����
                _currentDirection = 0;
            }
        }

        //��ײ���
        Vec2 displacement = _velocity.getNormalized() * moveSpeed * delta;
        if (!MapManager::getInstance()->isObstacleAtPosition(currentPosition + displacement)) {
            //�����ϰ���ſ����ƶ�
            // �ƶ�����
            setPlayerPosition(currentPosition + displacement);


        }
        // ��ʾ��ǰ����
        if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
            _sprite[_currentDirection]->setVisible(true);
        }
    }
}

void Player::interactWithMap() {
    // ��ͼ�����߼�ʵ��
    //CCLOG("Interacting with map...");
}

void Player::setCurrentTool(int toolId) {
    _currentTool = toolId;
}

void Player::updateEnergy(float delta) {
    if (_isMoving) {
        _energy = std::max(0, _energy - static_cast<int>(delta * 10));  // �ƶ����پ���
    }
}

const std::string& Player::getNickname() const {
    return _nickname;
}

int Player::getSelectedCharacter() const {
    return _selectedCharacter;
}

// ����Ի����λ��
void Player::updateDialogPosition(NPC* npc) {
    // ��ȡ NPC ��λ�ã��������꣩
    Vec2 npcWorldPosition = npc->getParent()->convertToWorldSpace(npc->getPosition());

    // ��ȡ NPC �Ĵ�С
    Size npcSize = npc->getContentSize();

    // ����Ի����λ�ã�������������꣩
    Vec2 dialogPosition = npcWorldPosition;

    // ����Ի���������ڳ������ͼ�ģ�ֱ�����öԻ����λ��
    uiManager->dialogSprite->setPosition(dialogPosition);
}

void Player::showDialogBox(NPC* npc, const std::string& category) {
    // ��ȡ NPC ��λ�ã��������꣩
    Vec2 npcWorldPosition = npc->getParent()->convertToWorldSpace(npc->getPosition());

    // ��ȡ NPC �Ĵ�С
    Size npcSize = npc->getContentSize();

    // ��ȡ UIManager ʵ����ȷ�����ѳ�ʼ��
    if (!uiManager) {
        uiManager = UIManager::getInstance(_selectedCharacter, _nickname);  // ��ȡ UIManager ʵ��
    }

    std::string finalCategory = category; // Ĭ��ʹ�ô�������
    std::string dialogue;                 // �Ի�����

    // ��ȡ NPC �ĶԻ��б�
    const auto& taskDialogues = npc->getDialogues("task");
    const auto& generalDialogues = npc->getDialogues("general");
    const auto& relationshipFriendship = npc->getDialogues("relationship_friendship");
    const auto& relationshipRomantic = npc->getDialogues("relationship_romantic");

    // ��̬�����Ի����
    if (npc->getAffection() >= 90 && !relationshipRomantic.empty()) {
        finalCategory = "relationship_romantic";
        dialogue = npc->getRandomDialogue(finalCategory);
    }
    else if (npc->getAffection() >= 60 && !relationshipFriendship.empty()) {
        finalCategory = "relationship_friendship";
        dialogue = npc->getRandomDialogue(finalCategory);
    }
    else {
        // ��Ȩ���ѡ�� "task" �� "general"
        float taskWeight = 0.8f;  // ����Ի���Ȩ��
        srand(static_cast<unsigned>(time(0)));
        float randomValue = dis(gen);  // ���� [0, 1) �����ֵ
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
            // ��� general û�жԻ����� task ���ڣ�ʹ�� task �Ի�
            finalCategory = "task";
            dialogue = npc->getRandomDialogue(finalCategory);
        }
        else {
            dialogue = "I have nothing to say."; // ���޶Ի�����ʱ��Ĭ������
        }
    }

    // ���������Ի����Ƴ�����ʾ������
    if (finalCategory == "task") {
        CCLOG("remove");
        CCLOG("text:%s", dialogue);
        npc->removeDialogue("task", dialogue); // �������б����Ƴ���ǰ����
    }

    // ȷ�� uiManager �� dialogSprite��dialogTextLabel ����ȷ��ʼ��
    if (uiManager && uiManager->dialogSprite && uiManager->dialogTextLabel) {
        uiManager->chatSprite->setVisible(true); // ��ʾ�Ի���

        // ���öԻ����ı�
        uiManager->dialogTextLabel->setString(dialogue);  // ʹ�� setString �����ı�
        uiManager->dialogTextLabel->setVisible(true); // ��ʾ����
    }
    else {
        // ��� uiManager �� dialogSprite �� dialogTextLabel û�г�ʼ���������־
        CCLOG("uiManager or dialogSprite or dialogTextLabel is not properly initialized!");
    }
}

void Player::openPlayerMenu() {
    CCLOG("Into player menu!");

    // ��ȡ��ǰ��Ļ�Ŀ��������С
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // ����һ������Ϊ�˵�������
    menuLayer = Layer::create();
    menuLayer->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));

    // ��������ͼƬ���飬������ê��Ϊ(0.5, 0.5)�Ծ�����ʾ
    auto background = Sprite::create("an_menu.png");
    background->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(background);

    // ����һ����ǩ����ʾ����
    auto label = Label::createWithTTF("Player Information", "fonts/Marker Felt.ttf", 30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4 + 196)); // ������ǩλ��
    menuLayer->addChild(label);

    // ����һ����ť�������رղ˵�
    auto closeBtn = MenuItemImage::create(
        "close.png", // ����״̬��ͼƬ
        "close.png", // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(Player::closePlayerMenu, this));
    closeBtn->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    closeBtn->setPosition(Vec2(visibleSize.width / 4 + 65, visibleSize.height / 4 + 32)); // �����رհ�ťλ��

    // ����һ���˵����󣬲���ӹرհ�ť
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
    playerImage->setPosition(Vec2(380 - 300, 360 - 175)); // ������ϸ��ϢͼƬλ��
    playerImage->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊͼ������
    playerImage->setScale(4.5); // ����ͼ���СΪԭ����1.5��
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

    // ���˵�����ӵ�����
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(menuLayer, 20); // ����һ�����ʵĲ㼶��ȷ���˵�����������

}

void Player::closePlayerMenu(Ref* sender) {
    // �رղ˵����߼�
    if (menuLayer) {
        menuLayer->removeFromParentAndCleanup(true); // �Ƴ��˵���
        menuLayer = nullptr; // �������
    }
}