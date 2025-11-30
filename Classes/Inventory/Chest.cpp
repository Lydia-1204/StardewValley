/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    item.h
 * File Function: 实现Chest类实现，实现工具箱的的创造使用
 * Author:        王小萌 2351882
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#include "Inventory/Chest.h"
#include "Inventory/ToolManager.h"
#include "Inventory/ItemManager.h"

USING_NS_CC;
Chest* Chest::instance = nullptr;


Chest* Chest::getInstance( ) {
    if (instance == nullptr) {  // 如果实例不存在，则创建
        instance = new (std::nothrow) Chest();
        if (instance && instance->init( )) {
            instance->autorelease();  // 添加到内存管理系统
            instance->setupItemsAndTools();  // 初始化箱子物品和工具
            instance->initMouseListener();//监听事件
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // 返回唯一实例
}



void Chest::setupItemsAndTools() {
    // 获取屏幕的可见尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 计算背景精灵的大小，为屏幕的八分之一
    auto bgWidth = visibleSize.width / 5.6f;
    auto bgHeight = visibleSize.height / 5.6f;

    // 计算每个格子的宽度
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    // 创建物品栏背景精灵
    itemBarBg = Sprite::create("../Resources/tools/Chest_bg..png");
    itemBarBg->setScaleX(bgWidth / itemBarBg->getContentSize().width);
    itemBarBg->setScaleY(bgHeight / itemBarBg->getContentSize().height);

    Size mapSize = MapManager::getInstance()->getCurrentMapSize(1);
    itemBarBg->setPosition(mapSize.width / 2, 0);
    //itemBarBg->setPosition(Vec2(visibleSize.width / 2 - bgWidth / 2, visibleSize.height / 2));
    this->addChild(itemBarBg, 1);

    // 创建工具栏背景精灵
    toolBarBg = Sprite::create("../Resources/tools/Chest_bg..png");
    toolBarBg->setScaleX(bgWidth / toolBarBg->getContentSize().width);
    toolBarBg->setScaleY(bgHeight / toolBarBg->getContentSize().height);
    toolBarBg->setPosition(mapSize.width / 2+ gridWidth*7, 0);
    this->addChild(toolBarBg, 1);
    //工具格
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 7; j++) {
            itemgrids[i][j] = Sprite::create("../Resources/tools/tools_bg.png");
            itemgrids[i][j]->setPosition(itemBarBg->getPositionX() - bgWidth / 2 + 16 + j * gridWidth,
                itemBarBg->getPositionY() + bgHeight / 2 - gridWidth - i * gridWidth + 16);
            this->addChild(itemgrids[i][j], 2);
        }
    // 初始化物品和工具，并放置在相应的背景精灵上
    for (int i = 0; i < 7; i++) {
        Item* item;
        switch (i) {
        case 0: item = Item::create(Item::ItemType::SEED); break;
        case 1: item = Item::create(Item::ItemType::FISH); break;
        case 2: item = Item::create(Item::ItemType::FRUIT); break;
        case 3: item = Item::create(Item::ItemType::BONE); break;
        case 4: item = Item::create(Item::ItemType::WOOL); break;
        case 5: item = Item::create(Item::ItemType::WOODEN); break;
        case 6: item = Item::create(Item::ItemType::EGG); break;
        }
        chestItems.push_back(item);
        // 根据背景精灵的位置和格子的大小计算物品的位置
        auto itemPosition = Vec2(itemBarBg->getPositionX() - bgWidth / 2 + 16 + i * gridWidth,
            itemBarBg->getPositionY() + bgHeight / 2 - gridWidth + 16);
        item->setPosition(itemPosition);
        this->addChild(item, 3);
        addItem(item);
    }


    //工具格
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 7; j++) {
            toolgrids[i][j] = Sprite::create("../Resources/tools/tools_bg.png");

            toolgrids[i][j]->setPosition(toolBarBg->getPositionX() - bgWidth / 2 + 16 + j * gridWidth,
                toolBarBg->getPositionY() + bgHeight / 2 - gridWidth - i * gridWidth + 16);
            this->addChild(toolgrids[i][j], 2);
        }
    // 初始化工具
    for (int i = 0; i < 4; i++) {
        Tool* tool;
        switch (i) {
        case 0: tool = Tool::create(Tool::ToolType::HOE); break;
        case 1: tool = Tool::create(Tool::ToolType::AXE); break;
        case 2: tool = Tool::create(Tool::ToolType::WATERING_CAN); break;
        case 3: tool = Tool::create(Tool::ToolType::FISHING_ROD); break;
        }
        chestTools.push_back(tool);
        auto toolPosition = Vec2(toolBarBg->getPositionX() - bgWidth / 2 + 16 + i * gridWidth,
            toolBarBg->getPositionY() + bgHeight / 2 - gridWidth + 16);
        tool->setPosition(toolPosition);
        this->addChild(tool, 2);
    }
    //标签
    itemLabel = Label::createWithSystemFont("Items", "../Resources/fonts/Marker Felt.ttf", 20);
    itemLabel->setTextColor(Color4B::WHITE);
    itemLabel->setPosition(Vec2(itemBarBg->getPositionX(), itemBarBg->getPositionY()-32.0*2));  // 调整位置
    this->addChild(itemLabel, 4);

    // 工具栏标签
   toolLabel = Label::createWithSystemFont("Tools", "../Resources/fonts/Marker Felt.ttf", 20);
    toolLabel->setTextColor(Color4B::WHITE);
    toolLabel->setPosition(Vec2(toolBarBg->getPositionX() , toolBarBg->getPositionY() - 32.0 * 2));  // 调整位置
    this->addChild(toolLabel, 4);

    this->setVisible(false);
}


void Chest::showItemsAndTools() {
    // 显示箱子中的物品
    for (auto& item : chestItems) {
        item->setVisible(true);  // 显示物品
        itemBarBg->setVisible(true);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 7; j++)
                itemgrids[i][j]->setVisible(true);
    }
    for (auto& tool : chestTools) {
        tool->setVisible(true);  // 显示工具
        toolBarBg->setVisible(true);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 7; j++)
               toolgrids[i][j]->setVisible(true);
    }
}

// 初始化鼠标点击监听器
void Chest::initMouseListener() {
    // 添加鼠标监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
        auto locationInWorld = event->getLocationInView();  // 获取屏幕视图中的坐标
        auto locationInChest = this->convertToNodeSpace(locationInWorld);  // 转换到箱子节点的坐标系

        // 检查是否点击了物品格子
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 7; j++) {
                if (itemgrids[i][j]->getBoundingBox().containsPoint(locationInChest)) {
                    if (i * 7 + j < chestItems.size()) {  // 如果该格子有物品
                        onItemClicked(chestItems[i * 7 + j]);  // 触发物品点击事件
                    }
                    return;
                }
            }
        }

        // 检查是否点击了工具格子
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 7; j++) {
                if (toolgrids[i][j]->getBoundingBox().containsPoint(locationInChest)) {
                    if (i * 7 + j < chestTools.size()) {  // 如果该格子有工具
                        onToolClicked(chestTools[i * 7 + j]);  // 触发工具点击事件
                    }
                    return;
                }
            }
        }
        };
    //*******关闭按钮**********//
    auto closeItem = MenuItemImage::create(
        "../Resources/closeChest.png", // 正常状态
        "../Resources/closeChest.png", // 被点击状态
        CC_CALLBACK_1(Chest::closeChest, this)); // 按钮点击事件


    closeItem->setPosition(Vec2(toolgrids[0][6]->getPositionX() + 10.0f, toolgrids[0][6]->getPositionY() + 10.0f));  // 工具栏右上角
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);  // 设置菜单位置
    this->addChild(menu, 5);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Chest::addItem(Item* item) {
    for (int i = 0; i < 28; i++) {
        if (i< chestItems.size()&&chestItems[i] && chestItems[i]->getType() == item-> getType()) {
            // 如果物品栏中已经有这个类型的物品，增加数量
            chestItems[i]->increaseQuantity(1);
            return;
        }
        if (i == chestItems.size()) {
            // 如果有空位，创建新物品
            auto itemnew = Item::create(item->getType());
            int x = i / 7;
            int y = i % 7;
            itemnew->setPosition(itemgrids[x][y]->getPosition());
            chestItems.push_back( itemnew);
            this->addChild(chestItems[i],2);
            auto location = chestItems[i]->getPosition();
            CCLOG("Items [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }
}
void Chest::addTool(Tool* tool) {
  
    for (int i = 0; i < 28; i++) {
        if (i==chestTools.size()) {
            auto newtool = Tool::create(tool->getType());
            int x = i / 7;
            int y = i % 7;
            chestTools.push_back(newtool);
            chestTools[i]->setPosition(toolgrids[x][y]->getPosition());
            this->addChild(chestTools[i],2);
            //chestTools[i]->setVisible(true);
            auto location = chestTools[i]->getPosition();
            CCLOG("tools [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }
   
}


// 物品点击事件
void Chest::onItemClicked(Item* item) {
   
    
    if (item->getQuantity() > 0)
            ItemManager::getInstance(1, "nickname")->addItem(item->getType());
    // 从箱子里取出物品并加入玩家物品栏
        item->decreaseQuantity(1);  // 减少物品数量
        if (item->getQuantity() == 0) {
            item->setVisible(false);
        }
    // 将物品添加到玩家物品栏
     
    CCLOG("Item added to player inventory");
}

// 工具点击事件
void Chest::onToolClicked(Tool* tool) {

    tool->setVisible(false);  // 减少工具数量
    // 将工具添加到玩家的工具栏
        ToolManager::getInstance(1, "nickname")->addTool(tool->getType());
    CCLOG("Tool added to player toolbar");
}

void Chest::chestSetPosition(const Vec2& position) {
    this->setPosition(position);
}
/*
void Chest::closeChest() {
    if (isOpen) {
        isOpen = false;
        CCLOG("Chest closed");
        // 隐藏箱子中的物品和工具
        for (auto& item : chestItems) {
            item->setVisible(false);
            itemBarBg->setVisible(false);
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 4; j++)
                    itemgrids[i][j]->setVisible(false);
        }
        for (auto& tool : chestTools) {
            tool->setVisible(false);
            toolBarBg->setVisible(false);
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 4; j++)
                    toolgrids[i][j]->setVisible(false);
        }
    }
}
*/

void Chest::openChest() {
    if (!isOpen) {
        isOpen = true;
        CCLOG("Chest opened");
        this->setVisible(true);
        //showItemsAndTools();  // 显示物品和工具
    }
}

void Chest::closeChest(Ref* sender) {
    if (isOpen) {
        isOpen = false;
        CCLOG("Chest closed");
        this->setVisible(false);
        /*
        // 隐藏箱子中的物品和工具
        for (auto& item : chestItems) {
            item->setVisible(false);
            itemBarBg->setVisible(false);
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 7; j++)
                    itemgrids[i][j]->setVisible(false);
        }
        for (auto& tool : chestTools) {
            tool->setVisible(false);
            toolBarBg->setVisible(false);
            for (int i = 0; i < 4; i++)
                for (int j = 0; j <7; j++)
                    toolgrids[i][j]->setVisible(false);
        }
        */
       // itemLabel->setVisible(false);
       // toolLabel->setVisible(false);
    }
}
