/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: 实现tool类，实现工具的管理 升级 (接入 ToolFactory)
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#include "Inventory/Tool.h"
#include "Inventory/ToolFactory.h" // 引入工厂
#include "Inventory/ToolBehavior.h"
#include "World/Map.h"
#include "Characters/Player.h"
#include "Inventory/ItemManager.h"

#include <utility>

USING_NS_CC;

namespace
{
// 装饰器模式：在基础策略外包裹 PLUS 专用装饰器。
ToolBehaviorPtr wrapPlusDecorators(ToolBehaviorPtr baseBehavior, Tool::ToolType type)
{
    switch (type)
    {
    case Tool::ToolType::HOEPLUS:
    case Tool::ToolType::AXEPLUS:
    case Tool::ToolType::FISHING_RODPLUS:
        return ToolBehaviorPtr(new PowerUpDecorator(std::move(baseBehavior)));
    case Tool::ToolType::WATERING_CANPLUS:
        return ToolBehaviorPtr(new WideRangeDecorator(std::move(baseBehavior)));
    default:
        return baseBehavior;
    }
}
} // namespace

Tool *Tool::create(ToolType type)
{
    // 委托给工厂创建
    return ToolFactory::getInstance()->createTool(type);
}

void IToolBehaviorDeleter::operator()(IToolBehavior *ptr) const
{
    delete ptr;
}

bool Tool::init(ToolType type)
{
    if (!Sprite::init())
    {
        return false;
    }

    this->type = type;
    selectedCharacter = 0;

    // 使用工厂配置贴图，替代 switch-case
    ToolFactory::getInstance()->configureTool(this, type);

    // 策略（基础）+ 装饰器（PLUS）的组合（Strategy + Decorator）
    auto baseBehavior = makeToolBehavior(type);
    behavior = wrapPlusDecorators(std::move(baseBehavior), type);

    return true;
}

Tool::ToolType Tool::getType() const
{
    return type;
}

Tool::~Tool() = default;

void Tool::usetool()
{
    if (behavior)
    {
        behavior->use(*this);
    }
    else
    {
        CCLOG("Invalid tool type or tool not equipped.");
    }
}
void Tool::buytool()
{

    ;
}

void Tool::setBehavior(ToolBehaviorPtr newBehavior)
{
    behavior = std::move(newBehavior);
}

void Tool::setOwnerContext(int ownerSelectedCharacter, const std::string &ownerNickname)
{
    selectedCharacter = ownerSelectedCharacter;
    nickname = ownerNickname;
}