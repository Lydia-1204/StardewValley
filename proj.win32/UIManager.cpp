/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SysttemScene.cpp
 * File Function: ʵ��UIManager�࣬չʾ��Ϸ������ͻ������ܣ�ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#include "UIManager.h"
#include "GameScene.h"

UIManager* UIManager::instance = nullptr;

UIManager::UIManager() : uiLayer(nullptr), energyLabel(nullptr), clockLabel(nullptr), playerNameLabel(nullptr), toolBar(nullptr) {
    setupUIElements();

    // ע����̼�����
    auto listener = EventListenerKeyboard::create();
    listener->onKeyPressed = CC_CALLBACK_2(UIManager::handleShortcuts, this);
    Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
}

UIManager* UIManager::getInstance() {
    if (!instance) {
        instance = new (std::nothrow) UIManager();
    }
    return instance;
}

Layer* UIManager::getLayer() {
    return uiLayer;
}

void UIManager::setupUIElements() {
    uiLayer = Layer::create();

    // ��ʼ������ǳ���ʾ
    playerNameLabel = Label::createWithTTF("Player: Xiao Yuan", "fonts/Marker Felt.ttf", 18);
    playerNameLabel->setPosition(Vec2(100, 680));
    uiLayer->addChild(playerNameLabel);

    // ��ʼ��ʱ����ʾ
    clockLabel = Label::createWithTTF("Day 1, 08:00", "fonts/Marker Felt.ttf", 18);
    clockLabel->setPosition(Vec2(1100, 680));
    uiLayer->addChild(clockLabel);

    // ��ʼ����������ʾ
    energyLabel = Label::createWithTTF("Energy: 100%", "fonts/Marker Felt.ttf", 18);
    energyLabel->setPosition(Vec2(100, 650));
    uiLayer->addChild(energyLabel);

    // ��ʼ��������
    auto toolBarLabel = Label::createWithTTF("Toolbar (1-5)", "fonts/Marker Felt.ttf", 18);
    toolBarLabel->setPosition(Vec2(640, 40));
    uiLayer->addChild(toolBarLabel);
}

void UIManager::handleShortcuts(EventKeyboard::KeyCode keyCode, Event* event) {
    auto director = Director::getInstance();
    auto currentScene = director->getRunningScene();

    switch (keyCode) {
    case EventKeyboard::KeyCode::KEY_E: // E �� ESC ����ͣ�˵�
    case EventKeyboard::KeyCode::KEY_ESCAPE:
        togglePauseMenu();
        break;

    case EventKeyboard::KeyCode::KEY_M: // M �����Ե�ͼ
        openMiniMap();
        break;

    case EventKeyboard::KeyCode::KEY_F: // F ������״̬
        showPlayerStats();
        break;

    default:
        log("Unhandled key: %d", static_cast<int>(keyCode));
        break;
    }
}

void UIManager::update(float dt) {
    // ����ʱ��
    // ���磺����ÿ������Ϸʱ���һ����
    static int gameMinutes = 0;
    gameMinutes += static_cast<int>(dt * 60);
    int hours = (gameMinutes / 60) % 24;
    int minutes = gameMinutes % 60;

    char buffer[32];
    snprintf(buffer, sizeof(buffer), "Day 1, %02d:%02d", hours, minutes);
    clockLabel->setString(buffer);

    // ���¾����������� UI
}

void UIManager::togglePauseMenu() {
    if (Director::getInstance()->isPaused()) {
        Director::getInstance()->resume();
        log("Game Resumed");
    }
    else {
        Director::getInstance()->pause();
        log("Game Paused");
    }
}

void UIManager::openMiniMap() {
    log("Opening Mini-Map...");
    // ��Ӵ����Ե�ͼ���߼�
}

void UIManager::showPlayerStats() {
    log("Showing Player Stats...");
    // �����ʾ���״̬���߼�
}