#include "Player.h"

USING_NS_CC;

Player::Player()
    : _sprite(nullptr), _velocity(Vec2::ZERO), _energy(100), _currentTool(0), _speed(100.0f), _isMoving(false) {}

Player::~Player() {}

bool Player::init() {
    if (!Node::init()) {
        return false;
    }

    // ������Ҿ���
    _sprite = Sprite::create("../Resources/Characters-102/Mariner..png");
    if (_sprite) {
        this->addChild(_sprite);
    }

    // ��ʼ���¼�������
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // ��ʱ�������״̬
    schedule(CC_SCHEDULE_SELECTOR(Player::update), 1.0f / 60.0f);

    return true;
}

Player* Player::create(int selectedCharacter) {
    Player* player = new (std::nothrow) Player();
    if (player && player->init()) {
        player->autorelease();
        return player;
    }
    delete player;
    return nullptr;
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
}

void Player::onKeyPressed(EventKeyboard::KeyCode keyCode, Event* event) {
    _keysPressed.insert(keyCode);

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
        // ִ�й���ʹ���߼�
        CCLOG("Tool %d used!", _currentTool);
    }
}

void Player::move(float delta) {
    if (_velocity != Vec2::ZERO) {
        Vec2 displacement = _velocity.getNormalized() * _speed * delta;
        this->setPosition(this->getPosition() + displacement);

        // ��ײ����߽���
        interactWithMap();
    }
}

void Player::interactWithMap() {
    // ��ͼ�����߼�ʵ��
    CCLOG("Interacting with map...");
}

void Player::setCurrentTool(int toolId) {
    _currentTool = toolId;
}

void Player::updateEnergy(float delta) {
    if (_isMoving) {
        _energy = std::max(0, _energy - static_cast<int>(delta * 10));  // �ƶ����پ���
    }
}