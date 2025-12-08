/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: 实现tool类，实现工具的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#include "Inventory/Tool.h"
#include "Inventory/ToolFactory.h"
#include "Characters/Player.h"
#include "Scenes/GameScene.h"

USING_NS_CC;

// 私有构造函数，支持工厂模式
Tool::Tool() : type(ToolType::NONE), price(0), selectedCharacter(1), nickname("guest")
{
}

Tool *Tool::create(ToolType type)
{
    Tool *tool = new (std::nothrow) Tool();
    if (tool && tool->init(type))
    {
        tool->autorelease();
        return tool;
    }
    CC_SAFE_DELETE(tool);
    return nullptr;
}

bool Tool::init(ToolType type)
{
    if (!Sprite::init())
    {
        return false;
    }

    this->type = type;

    // 根据工具类型加载不同的资源
    switch (type)
    {
    case ToolType::HOE:
        this->initWithFile("../Resources/tool/hoe.png");
        price = 100;
        break;
    case ToolType::AXE:
        this->initWithFile("../Resources/tool/axe.png");
        price = 150;
        break;
    case ToolType::WATERING_CAN:
        this->initWithFile("../Resources/tool/watering_can.png");
        price = 120;
        break;
    case ToolType::FISHING_ROD:
        this->initWithFile("../Resources/tool/fishing_rod.png");
        price = 200;
        break;
    case ToolType::FERTILIZER:
        this->initWithFile("../Resources/tool/fertilizer.png");
        price = 80;
        break;
    case ToolType::HOEPLUS:
        this->initWithFile("../Resources/tool/hoe_plus.png");
        price = 300;
        break;
    case ToolType::AXEPLUS:
        this->initWithFile("../Resources/tool/axe_plus.png");
        price = 350;
        break;
    case ToolType::ANIMALFOOD:
        this->initWithFile("../Resources/tool/animal_food.png");
        price = 50;
        break;
    default:
        break;
    }

    return true;
}

// 新增：对象池重置方法
void Tool::reset()
{
    // 重置工具到默认状态，用于对象池回收
    this->type = ToolType::NONE;
    this->price = 0;
    this->setVisible(false);
    this->setPosition(0, 0);

    // 停止所有动作
    this->stopAllActions();

    // 从父节点移除
    if (this->getParent())
    {
        this->removeFromParent();
    }
}

Tool::ToolType Tool::getType() const
{
    return type;
}

void Tool::usetool()
{
    // 保持原有的工具使用逻辑
    switch (type)
    {
    case ToolType::HOE:
        CCLOG("Using HOE: Tilling soil...");
        break;
    case ToolType::AXE:
        CCLOG("Using AXE: Chopping wood...");
        break;
    case ToolType::WATERING_CAN:
        CCLOG("Using WATERING_CAN: Watering crops...");
        break;
    case ToolType::FISHING_ROD:
        CCLOG("Using FISHING_ROD: Fishing...");
        break;
    case ToolType::FERTILIZER:
        CCLOG("Using FERTILIZER: Fertilizing soil...");
        break;
    case ToolType::HOEPLUS:
        CCLOG("Using HOEPLUS: Advanced tilling...");
        break;
    case ToolType::AXEPLUS:
        CCLOG("Using AXEPLUS: Advanced chopping...");
        break;
    case ToolType::ANIMALFOOD:
        CCLOG("Using ANIMALFOOD: Feeding animals...");
        break;
    default:
        CCLOG("Invalid Tool type or Tool not equipped.");
        break;
    }
}

void Tool::buytool()
{
    CCLOG("Buying tool of type %d", static_cast<int>(type));
}