/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Player.cpp
 * File Function: 实现Player类，实现玩家的行走、砍、精力更新等动作
 * Author:        邓语乐 2351273
 * Update Date:   2024/12/16
 *********************************************************************************************************/
#include "Player.h"

USING_NS_CC;
Player* Player:: instance=nullptr;     // 单例实例
Player::Player()
    : _sprite{ nullptr }, _velocity(Vec2::ZERO), _energy(100), _currentTool(0),
    _speed(100.0f), _isMoving(false), _nickname("Player"), _selectedCharacter(1),
    _toolSprite(nullptr), _fishing_rodSprite(nullptr), _watering_canSprite(nullptr),
    _scytheSprite(nullptr), _hoeSprite(nullptr), _shiftKeyPressed(false), _isToolActive(false) {}

Player::~Player() {}

Player* Player::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {  // 如果实例不存在，则创建
        instance = new (std::nothrow) Player();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();  // 添加到内存管理系统
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // 返回唯一实例
}

bool Player::init(int selectedCharacter, const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

  
    // 保存玩家昵称和选择的角色
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
    this->addChild(_sprite[0]);
    this->addChild(_sprite[1]);
    _sprite[1]->setVisible(0);
    this->addChild(_sprite[2]);
    _sprite[2]->setVisible(0);
    this->addChild(_sprite[3]);
    _sprite[3]->setVisible(0);

    // 初始显示正面精灵
    _sprite[0]->setVisible(true);

    // 创建斧头精灵（初始不可见）
    _toolSprite = Sprite::create("../Resources/tools/AXE.png");
    _toolSprite->setVisible(false);  // 默认不显示
    this->addChild(_toolSprite);

    // 初始化事件监听器
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = CC_CALLBACK_2(Player::onKeyPressed, this);
    keyboardListener->onKeyReleased = CC_CALLBACK_2(Player::onKeyReleased, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 设置鼠标点击事件监听器
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = CC_CALLBACK_1(Player::onMouseDown, this); // 设置鼠标点击事件回调
    listener->onMouseUp = CC_CALLBACK_1(Player::onMouseUp, this); // 设置鼠标点击事件回调
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 定时更新玩家状态
    schedule(CC_SCHEDULE_SELECTOR(Player::update), 1.0f / 60.0f);


    return true;
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
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        // 鼠标按下时开始工具激活
        _isToolActive = true;

        // 设置tool精灵初始位置
        _toolSprite->setPosition(_sprite[_currentDirection]->getPosition());
        _toolSprite->setVisible(true);  // 显示斧头

        // 开始定时显示和隐藏斧头精灵
        schedule([this](float dt) {
            if (_isToolActive) {
                // 切换斧头的可见性
                bool isVisible = _toolSprite->isVisible();
                isVisible = !isVisible;
                _toolSprite->setVisible(isVisible);  // 切换显示/隐藏

                // 获取 UIManager 实例
                uiManager = UIManager::getInstance(_selectedCharacter, _nickname);

                // 获取当前精力值
                _energy = uiManager->getCurrentEnergy();

                // 做其他逻辑处理
                // 比如：根据能量值做一些操作
                CCLOG("Current Energy: %d", _energy);

                // 假设你希望减少精力
                uiManager->setCurrentEnergy(_energy - 1);  // 每次调用减少 1
            }
            }, 0.2f, "toggle_axe_key");  // 每0.2秒切换一次
    }
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

        // 移动精灵
        Vec2 displacement = _velocity.getNormalized() * moveSpeed * delta;
        setPlayerPosition(currentPosition + displacement);

        // 显示当前精灵
        if (_sprite[_currentDirection] && _sprite[_currentDirection]->getParent()) {
            _sprite[_currentDirection]->setVisible(true);
        }

        // 碰撞检测或边界检测
        interactWithMap();
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