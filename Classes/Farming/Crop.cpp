/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.cpp
 * File Function: Crop Context 实现，基于状态模式重构
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/22 (Refactored)
 ****************************************************************/

#include "Farming/Crop.h"
#include "Farming/CropState.h" // 包含具体状态定义
#include "Scenes/GameScene.h"
#include "World/Map.h"

#include "Inventory/ToolManager.h"
#include "Inventory/ItemManager.h"


USING_NS_CC;

// ================= 构造与析构 =================

Crop::Crop()
    : m_state(nullptr)
    , gameScene(nullptr)
    , myCrop(nullptr)
    , deadSprite(nullptr)
    , age(0)
    , newAge(0)
    , watered(false)
    , lastWateredTime(0)
    , timeElapsed(0)
    , deadTimes(0)
{
    // 初始化不同生长阶段的图片路径
    stageTextures = {
        "../Resources/seed.png",     // 0: 种子
        "../Resources/stage1.png",   // 1
        "../Resources/stage2.png",   // 2
        "../Resources/stage3.png",   // 3
        "../Resources/stage4.png",   // 4
        "../Resources/stage5.png",   // 5
        "../Resources/mature.png"    // 6: 成熟
    };
}

Crop::~Crop() {
    // 【重要】释放状态对象内存
    if (m_state) {
        delete m_state;
        m_state = nullptr;
    }
}

Crop* Crop::create(GameScene* scene, const std::string& nickname) {
    Crop* crop = new (std::nothrow) Crop();
    crop->getGameScene(scene);
    if (crop && crop->init(nickname)) {
        crop->autorelease();
        return crop;
    }
    delete crop;
    return nullptr;
}

// ================= 初始化 =================

bool Crop::init(const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

    // 1. 创建基础 Sprite
    // 先创建一个空的或默认的 Sprite，具体纹理通过状态设置
    myCrop = Sprite::create(stageTextures[0]);
    if (!myCrop) {
        // 如果图片加载失败，尝试创建一个空节点防止崩溃
        myCrop = Sprite::create();
        CCLOG("Warning: Failed to create base sprite with texture %s", stageTextures[0].c_str());
    }
    this->addChild(myCrop);

    // 2. 初始化事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Crop::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // 3. 定时更新
    schedule(CC_SCHEDULE_SELECTOR(Crop::update), 1.0f / 60.0f);

    // 4. 【核心】设置初始状态为种子状态
    // ==========================================
    // 【使用状态模式重构】设置初始状态
    // ==========================================
    changeState(new SeedState());

    return true;
}

void Crop::getGameScene(GameScene* scene) {
    gameScene = scene;
}

// ================= 状态机核心逻辑 =================
// ==========================================
// 【使用状态模式重构】实现状态切换逻辑
// 作用：处理旧状态的清理(Exit)和新状态的初始化(Enter)
// ==========================================
void Crop::changeState(CropState* newState) {
    // 1. 退出旧状态
    if (m_state) {
        m_state->exit(this);
        delete m_state; // 销毁旧状态对象
    }

    // 2. 切换新状态
    m_state = newState;

    // 3. 进入新状态
    if (m_state) {
        m_state->enter(this);
    }
}

void Crop::update(float dt) {
    timeElapsed += dt;

    // 1. 地图可见性管理 (保持原有逻辑)
    if (MapManager::getInstance()->currentMapLabel == 1) {
        this->setVisible(true);
    }
    else {
        this->setVisible(false);
        // 如果不可见，是否还需要更新逻辑？通常建议继续更新数据，但不渲染
        // 这里保持原逻辑继续运行
    }

    // 2. 计时器更新 (1秒更新一次数据)
    if (timeElapsed >= 1.0f) {
        age += timeElapsed;           // 增加经过的时间
        lastWateredTime += timeElapsed; // 增加干旱时间
        timeElapsed = 0.0f;

        // Debug 日志 (可选)
        // CCLOG("Crop Update: Age=%d, LastWatered=%d", age, lastWateredTime);
    }

    // 3. 【核心】委托给当前状态处理每帧逻辑 (检查枯死等)
    // ==========================================
    // 【使用状态模式重构】行为委托 (Delegation)
    // 作用：将每帧的更新逻辑委托给当前状态对象处理
    // ==========================================
    if (m_state) {
        m_state->execute(this, dt);
    }

    // 4. 生长周期检查 (50秒)
    if (age >= 50) {
        CCLOG("Crop Growth Cycle Triggered!");
        grow();   // 调用自身的 grow，内部再委托给状态
        age = 0;  // 重置周期
    }
}

void Crop::onMouseDown(EventMouse* event) {
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);

    // 获取位置
    auto playerPos = Player::getInstance(1, "guest")->getPosition();
    auto locationInWorld = event->getLocationInView(); // 注意：这里可能需要转换坐标系，保持原逻辑

    // 距离检测 (保持原有逻辑：点击位置和玩家位置都必须靠近作物)
    // distance < 30 像素
    if (locationInWorld.distance(this->getPosition()) < 30 &&
        playerPos.distance(this->getPosition()) < 30) {

        // 【核心】委托给当前状态处理交互
        // ==========================================
        // 【使用状态模式重构】行为委托 (Delegation)
        // 作用：将点击交互逻辑委托给当前状态对象，消除原来的 if(state == xxx) 判断
        // ==========================================
        if (m_state) {
            m_state->onClick(this, mouseEvent);
        }
    }
}

void Crop::grow() {
    // 【核心】委托给当前状态处理生长
    if (m_state) {
        m_state->grow(this);
    }
}

// ================= 行为与表现 (供状态类调用) =================

void Crop::water() {
    watered = true;        // 标记为已浇水
    lastWateredTime = 0;   // 重置干旱计时器

    // 视觉表现
    std::string imagePath = "../Resources/watered.png";
    Sprite* happySprite = Sprite::create(imagePath);
    if (happySprite) {
        happySprite->setPosition(myCrop->getPosition()); // 或者 offset 一点
        this->addChild(happySprite, 1, "watered");
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 1.0f);
    }
    CCLOG("Crop watered.");
}

void Crop::fertilize() {
    // 施肥逻辑 (目前主要是视觉表现，数值逻辑可在 State 中扩展)
    std::string imagePath = "../Resources/fertilize.png";
    Sprite* happySprite = Sprite::create(imagePath);
    if (happySprite) {
        happySprite->setPosition(myCrop->getPosition());
        this->addChild(happySprite, 1, "fertilize");
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 1.0f);
    }
    CCLOG("Crop fertilized.");
}

void Crop::changeTexture(const std::string& texturePath) {
    if (!myCrop) return;

    // 加载并更换显示的图片
    auto texture = Director::getInstance()->getTextureCache()->addImage(texturePath);
    if (texture) {
        myCrop->setTexture(texture);
        // 如果图片尺寸变化，可能需要调整 rect，通常 setTexture 会自动处理
    }
    else {
        CCLOG("Error: Failed to load texture: %s", texturePath.c_str());
    }

    // 调用回调 (如果有外部 UI 监听)
    if (textureChangedCallback) {
        textureChangedCallback(texturePath);
    }
}

void Crop::removeSprite(float dt) {
    // 移除各种临时状态图标
    this->removeChildByName("matureSprite");
    this->removeChildByName("fertilize");
    this->removeChildByName("watered");
}

void Crop::setTextureChangedCallback(const TextureChangedCallback& callback) {
    textureChangedCallback = callback;
}