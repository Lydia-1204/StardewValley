/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: 实现tool类与toolmanager，实现工具的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#include "Tool.h"

USING_NS_CC;

 ToolManager* ToolManager:: instance=nullptr;
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


ToolManager* ToolManager::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {  // 如果实例不存在，则创建
        instance = new (std::nothrow) ToolManager();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();  // 添加到内存管理系统
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // 返回唯一实例
}





bool ToolManager::init(int _selectedCharacter, const std::string& _nickname) {
    if (!Node::init()) {
        return false;
    }
    selectedCharacter = _selectedCharacter;
    nickname = _nickname;
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
    float gridWidth = 32.0f;//工具栏宽度
    float startX = (Director::getInstance()->getVisibleSize().width - gridWidth * 10) / 2.0f;
    float startY = Director::getInstance()->getVisibleSize().height * 0.1f;
    for (int i = 0; i < tools.size(); i++) {
        if (tools[i] == nullptr) {
            auto tool = Tool::create(type);
            

            tool->setPosition(startX + i * gridWidth, startY);
            tools[i] = tool;
            this->addChild(tools[i] );
            auto location = tools[i]->getPosition();
            CCLOG("tools [%d]: %f,%f", i,location.x, location.y);
            return;
        }
    }
    CCLOG("ToolBar is full, cannot add more tools.");
    auto fullLabel = Label::createWithTTF("ToolBar is full!", "fonts/Marker Felt.ttf", 24);
    fullLabel->setPosition(startX, startY+64.0f);
    this->addChild(fullLabel);
    fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
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
    tool->usetool();
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

void Tool::usetool() {
    auto playerPos = Player::getInstance(selectedCharacter, nickname)->getPosition();
    auto direction = Player::getInstance(selectedCharacter, nickname)->_currentDirection;
   Vec2 dstPos;
    switch (direction) {
         
    case 0://下
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 1://右
        dstPos = Vec2(playerPos.x+16, playerPos.y);
        break;
    case 2 ://上
        dstPos = Vec2(playerPos.x , playerPos.y-16);
        break;
    case 3://左
        dstPos = Vec2(playerPos.x - 16, playerPos.y);
        break;
    }
    switch (type) {
    case ToolType::HOE: // 锄头功能：挖坑
        CCLOG("Using HOE: Digging a hole...");
      
        /*
                // 获取当前地图的地块坐标
        Vec2 tileCoord = MapManager::getInstance()->convertToTileCoord(dstPos);
                // 检查该地块是否可挖掘
        if (MapManager::getInstance()->isDiggable(tileCoord)) {
            MapManager::getInstance()->setTileState(tileCoord, "dug"); // 修改地块状态为“已挖掘”
            CCLOG("Hole dug at: %f, %f", tileCoord.x, tileCoord.y);
        }
        else {
            CCLOG("This tile cannot be dug.");
        }*/
        break;

    case ToolType::AXE: // 斧头功能：砍树
        CCLOG("Using AXE: Chopping a tree...");
     
      /*
        auto tree = MapManager::getInstance()->getTreeAtPosition(dstPos);

        if (tree) {
            tree->decreaseHealth(10); // 砍树，减少生命值
            if (tree->isFallen()) {
                CCLOG("Tree has fallen!");
                MapManager::getInstance()->removeTree(tree);
            }
        }
        else {
            CCLOG("No tree nearby.");
        }
     */
        break;

    case ToolType::WATERING_CAN: // 水壶功能：浇水
        CCLOG("Using WATERING_CAN: Watering the crops...");
       /*
        auto crop = MapManager::getInstance()->getCropAtPosition(dstPos);
        if (crop) {
            crop->increaseWaterLevel(1); // 增加水分值
            CCLOG("Crops watered successfully!");
        }
        else {
            CCLOG("No crops nearby to water.");
        }
        */
        break;

    case ToolType::FISHING_ROD: // 鱼竿功能：钓鱼
        CCLOG("Using FISHING_ROD: Fishing...");
        /*
            if (MapManager::getInstance()->isNearWater(dstPos)) {
                FishingManager::getInstance()->startFishing();
            }
            else {
                CCLOG("You are not near water.");
            }
            */
        break;

    default:
        CCLOG("Invalid tool type or tool not equipped.");
        break;
    }
    
}