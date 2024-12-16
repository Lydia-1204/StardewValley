/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.cpp
 * File Function: ʵ��Player�࣬ʵ����ҵ����ߡ������������µȶ���
 * Author:        ������ 2351273
 * Update Date:   2024/12/16
 *********************************************************************************************************/
#include "Player.h"

USING_NS_CC;
Player* Player:: instance=nullptr;     // ����ʵ��
Player::Player()
    : _sprite{ nullptr }, _velocity(Vec2::ZERO), _energy(100), _currentTool(0),
    _speed(100.0f), _isMoving(false), _nickname("Player"), _selectedCharacter(1),
    _toolSprite(nullptr), _fishing_rodSprite(nullptr), _watering_canSprite(nullptr),
    _scytheSprite(nullptr), _hoeSprite(nullptr), _shiftKeyPressed(false), _isToolActive(false) {}

Player::~Player() {}

Player* Player::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
        instance = new (std::nothrow) Player();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();  // ��ӵ��ڴ����ϵͳ
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // ����Ψһʵ��
}

bool Player::init(int selectedCharacter, const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

  
    // ��������ǳƺ�ѡ��Ľ�ɫ
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
    this->addChild(_sprite[0]);
    this->addChild(_sprite[1]);
    _sprite[1]->setVisible(0);
    this->addChild(_sprite[2]);
    _sprite[2]->setVisible(0);
    this->addChild(_sprite[3]);
    _sprite[3]->setVisible(0);

    // ��ʼ��ʾ���澫��
    _sprite[0]->setVisible(true);

    // ������ͷ���飨��ʼ���ɼ���
    _toolSprite = Sprite::create("../Resources/tools/AXE.png");
    _toolSprite->setVisible(false);  // Ĭ�ϲ���ʾ
    this->addChild(_toolSprite);

    // ��ʼ���¼�������
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // ����������¼�������
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this); // ����������¼��ص�
    listener->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this); // ����������¼��ص�
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ��ʱ�������״̬
    schedule(CC_SCHEDULE_SELECTOR(Player::update), 1.0f / 60.0f);


    return true;
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
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        // ��갴��ʱ��ʼ���߼���
        _isToolActive = true;

        // ����tool�����ʼλ��
        _toolSprite->setPosition(_sprite[_currentDirection]->getPosition());
        _toolSprite->setVisible(true);  // ��ʾ��ͷ

        // ��ʼ��ʱ��ʾ�����ظ�ͷ����
        schedule([this](float dt) {
            if (_isToolActive) {
                // �л���ͷ�Ŀɼ���
                bool isVisible = _toolSprite->isVisible();
                isVisible = !isVisible;
                _toolSprite->setVisible(isVisible);  // �л���ʾ/����

                // ��ȡ UIManager ʵ��
                uiManager = UIManager::getInstance(_selectedCharacter, _nickname);

                // ��ȡ��ǰ����ֵ
                _energy = uiManager->getCurrentEnergy();

                // �������߼�����
                // ���磺��������ֵ��һЩ����
                CCLOG("Current Energy: %d", _energy);

                // ������ϣ�����پ���
                uiManager->setCurrentEnergy(_energy - 1);  // ÿ�ε��ü��� 1
            }
            }, 0.2f, "toggle_axe_key");  // ÿ0.2���л�һ��
    }
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

        // �ƶ�����
        Vec2 displacement = _velocity.getNormalized() * moveSpeed * delta;
        setPlayerPosition(currentPosition + displacement);

        // ��ʾ��ǰ����
        if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
            _sprite[_currentDirection]->setVisible(true);
        }

        // ��ײ����߽���
        interactWithMap();
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