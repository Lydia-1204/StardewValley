/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: ʵ��tool�࣬ʵ�ֹ��ߵĹ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#include "Tool.h"

USING_NS_CC;

Tool* Tool::create(ToolType type) {
    Tool* tool = new (std::nothrow) Tool();
    if (tool && tool->init(type)) {
        tool->autorelease();
        return tool;
    }
    CC_SAFE_DELETE(tool);
    return nullptr;
}

bool Tool::init(ToolType type) {
    if (!Sprite::init()) {
        return false;
    }

    this->type = type;

    // ���ݹ������ͼ��ز�ͬ����Դ
    switch (type) {
    case ToolType::HOE:
        this->initWithFile("../Resources/tools/HOE.png");
        break;
    case ToolType::AXE:
        this->initWithFile("../Resources/tools/AXE.png");
        break;
    case ToolType::WATERING_CAN:
        this->initWithFile("../Resources/tools/WATERING_CAN.png");
        break;
    case ToolType::FISHING_ROD:
        this->initWithFile("../Resources/tools/FISHING_ROD.png");
        break;
    }
    return true;
}

Tool::ToolType Tool::getType() const {
    return type;
}

// ----------------------------- ToolManager -----------------------------
ToolManager* ToolManager::create() {
    ToolManager* manager = new (std::nothrow) ToolManager();
    if (manager && manager->init()) {
        manager->autorelease();
        return manager;
    }
    CC_SAFE_DELETE(manager);
    return nullptr;
}

bool ToolManager::init() {
    if (!Node::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ����������
     /*
    auto toolBarBg = Sprite::create("../ResourcesL/ooseSprites-73/DialogBoxGreen..png");
    toolBarBg->setPosition(visibleSize.width / 2, visibleSize.height * 0.1);
    this->addChild(toolBarBg);*/

    // ��ʼ��������
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    float startX = (visibleSize.width - gridWidth * 10) / 2.0f; // ���й�����
    float startY = visibleSize.height * 0.1f;

    for (int i = 0; i < 10; i++) {
        auto grid = Sprite::create("../Resources/tools/tools_bg.png");
        grid->setPosition(startX + i * gridWidth, startY);
        this->addChild(grid);

        tools.push_back(nullptr); // ��ʼ��������Ϊ��
    }

    // ѡ�п�
    selectionBox = Sprite::create("../Resources/tools/tools_selected.png");
    selectionBox->setVisible(false);
    this->addChild(selectionBox,10);

    // ��ʼ��״̬
    selectedToolIndex = -1;

    // ����¼�������
   // ����¼�������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
        CCLOG("mouseDown");
        auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����
        auto locationInToolsBg = this->convertToNodeSpace(locationInWorld); // ת���� tools_bg ������ϵ
        CCLOG("Mouse position in tools_bg space: %f, %f", locationInToolsBg.x, locationInToolsBg.y);

        // ����Ƿ�������
   
        CCLOG("Adjusted Mouse Position: %f, %f", locationInToolsBg.x, locationInToolsBg.y);
    
        // �ж�����Ƿ���������
        for (int i = 0; i < tools.size(); i++) {
            if (tools[i] && tools[i]->getBoundingBox().containsPoint(locationInToolsBg)) {
                if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                    CCLOG("Tool[%d] selected and used", i);
                    selectTool(i);  // ѡ�й���
                    useTool();     // ֱ��ʹ�ù���
                }
                else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                    discardTool();  // �Ҽ���������
                }
                return;
            }
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    initKeyboardListener();
    return true;
}

void ToolManager::addTool(Tool::ToolType type) {
    for (int i = 0; i < tools.size(); i++) {
        if (tools[i] == nullptr) {
            auto tool = Tool::create(type);
            float gridWidth = 32.0f;//���������
            float startX = (Director::getInstance()->getVisibleSize().width - gridWidth * 10) / 2.0f;
            float startY = Director::getInstance()->getVisibleSize().height * 0.1f;

            tool->setPosition(startX + i * gridWidth, startY);
            tools[i] = tool;
            this->addChild(tools[i] );
            auto location = tools[i]->getPosition();
            CCLOG("tools [%d]: %f,%f", i,location.x, location.y);
            return;
        }
    }
    CCLOG("ToolBar is full, cannot add more tools.");
}

void ToolManager::selectTool(int index) {
    if (index < 0 || index >= tools.size() || tools[index] == nullptr) {
        CCLOG("Invalid tool selection");
        return;
    }

    selectedToolIndex = index;
    updateSelectionBox();
    CCLOG("Tool selected: %d", index);
}

void ToolManager::useTool() {
    if (selectedToolIndex < 0 || selectedToolIndex >= tools.size() || tools[selectedToolIndex] == nullptr) {
        CCLOG("No tool selected to use");
        return;
    }

    auto tool = tools[selectedToolIndex];
    CCLOG("Using tool: %d", static_cast<int>(tool->getType()));
    // ��ӹ���ʹ���߼�
}

void ToolManager::discardTool() {
    if (selectedToolIndex < 0 || selectedToolIndex >= tools.size() || tools[selectedToolIndex] == nullptr) {
        CCLOG("No tool selected to discard");
        return;
    }

    auto tool = tools[selectedToolIndex];
    this->removeChild(tool);
    tools[selectedToolIndex] = nullptr;
    selectedToolIndex = -1;
    selectionBox->setVisible(false);
    CCLOG("Tool discarded: %d", static_cast<int>(tool->getType()));
    
}

void ToolManager::updateSelectionBox() {
    if (selectedToolIndex < 0 || selectedToolIndex >= tools.size() || tools[selectedToolIndex] == nullptr) {
        selectionBox->setVisible(false);
        return;
    }
    selectionBox->setVisible(true);
    selectionBox->setPosition(tools[selectedToolIndex]->getPosition());
}

void ToolManager::initKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_C:
            useTool(); // ʹ�õ�ǰ����
            break;
        case EventKeyboard::KeyCode::KEY_V:
            discardTool(); // ������ǰ����
            break;
        default:
            break;
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}