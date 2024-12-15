/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: 实现tool类，实现工具的管理 升级
 * Author:        王小萌 2351882
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

    // 根据工具类型加载不同的资源
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

    // 工具栏背景
     /*
    auto toolBarBg = Sprite::create("../ResourcesL/ooseSprites-73/DialogBoxGreen..png");
    toolBarBg->setPosition(visibleSize.width / 2, visibleSize.height * 0.1);
    this->addChild(toolBarBg);*/

    // 初始化工具栏
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    float startX = (visibleSize.width - gridWidth * 10) / 2.0f; // 居中工具栏
    float startY = visibleSize.height * 0.1f;

    for (int i = 0; i < 10; i++) {
        auto grid = Sprite::create("../Resources/tools/tools_bg.png");
        grid->setPosition(startX + i * gridWidth, startY);
        this->addChild(grid);

        tools.push_back(nullptr); // 初始化工具栏为空
    }

    // 选中框
    selectionBox = Sprite::create("../Resources/tools/tools_selected.png");
    selectionBox->setVisible(false);
    this->addChild(selectionBox,10);

    // 初始化状态
    selectedToolIndex = -1;

    // 添加事件监听器
   // 添加事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
        CCLOG("mouseDown");
        auto locationInWorld = event->getLocationInView();  // 获取屏幕视图中的坐标
        auto locationInToolsBg = this->convertToNodeSpace(locationInWorld); // 转换到 tools_bg 的坐标系
        CCLOG("Mouse position in tools_bg space: %f, %f", locationInToolsBg.x, locationInToolsBg.y);

        // 检测是否点击工具
   
        CCLOG("Adjusted Mouse Position: %f, %f", locationInToolsBg.x, locationInToolsBg.y);
    
        // 判断鼠标是否点击工具栏
        for (int i = 0; i < tools.size(); i++) {
            if (tools[i] && tools[i]->getBoundingBox().containsPoint(locationInToolsBg)) {
                if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                    CCLOG("Tool[%d] selected and used", i);
                    selectTool(i);  // 选中工具
                    useTool();     // 直接使用工具
                }
                else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                    discardTool();  // 右键丢弃工具
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
            float gridWidth = 32.0f;//工具栏宽度
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
    // 添加工具使用逻辑
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
            useTool(); // 使用当前工具
            break;
        case EventKeyboard::KeyCode::KEY_V:
            discardTool(); // 丢弃当前工具
            break;
        default:
            break;
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}