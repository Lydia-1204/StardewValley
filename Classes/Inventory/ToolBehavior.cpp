/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ToolBehavior.cpp
 * File Function: 工具策略实现
 * Author:        刘彦含 2351591
 * Update Date:   2025/12/09
 *********************************************************************************************************/
#include "Inventory/ToolBehavior.h"
#include "Characters/Player.h"
#include "Inventory/ItemManager.h"
#include "World/Map.h"

#include <vector>

using cocos2d::Vec2;

namespace
{
struct PlayerContext
{
    Player *player;
    Vec2 position;
    int direction;
    Vec2 targetPosition;
};

Vec2 computeTargetPosition(const Vec2 &playerPos, int direction)
{
    switch (direction)
    {
    case 0: // 下
        return Vec2(playerPos.x, playerPos.y - 16);
    case 1: // 右
        return Vec2(playerPos.x + 16, playerPos.y);
    case 2: // 上
        return Vec2(playerPos.x, playerPos.y - 16);
    case 3: // 左
        return Vec2(playerPos.x - 16, playerPos.y);
    default:
        return playerPos;
    }
}

PlayerContext makePlayerContext(const Tool &tool)
{
    Player *player = Player::getInstance(tool.selectedCharacter, tool.nickname);
    Vec2 playerPos = player->getPosition();
    int direction = player->_currentDirection;
    Vec2 targetPos = computeTargetPosition(playerPos, direction);
    return {player, playerPos, direction, targetPos};
}

bool isNearAny(const Vec2 &playerPos, const std::vector<Vec2> &targets, float radius)
{
    for (const auto &target : targets)
    {
        if (playerPos.distance(target) < radius)
        {
            return true;
        }
    }
    return false;
}
} // namespace

void HoeBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);

    if (type == Tool::ToolType::HOEPLUS)
    {
        CCLOG("Using HOE++: Digging a hole...");
    }
    CCLOG("Using HOE: Digging a hole...");

    if (MapManager::getInstance()->getCurrentBlockLabel() == 7)
    {
        if (ctx.position.x < 600 && ctx.position.x > 460 && ctx.position.y < 400 && ctx.position.y > 300)
        {
            ItemManager::getInstance(tool.selectedCharacter, tool.nickname)->addItem(Item::ItemType::MINERAL);
            ctx.player->changeMining();
        }
    }
}

void AxeBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);

    if (type == Tool::ToolType::AXEPLUS)
    {
        CCLOG("Using AXE++: Chopping a tree...");
    }
    CCLOG("Using AXE: Chopping a tree...");

    if (MapManager::getInstance()->getCurrentBlockLabel() == 2)
    {
        const std::vector<Vec2> treePositions = {
            {810, 465}, {875, 630}, {900, 385}, {950, 540}, {1050, 440}, {1100, 610}, {1170, 365}};
        if (isNearAny(ctx.position, treePositions, 50.0f))
        {
            ItemManager::getInstance(tool.selectedCharacter, tool.nickname)->addItem(Item::ItemType::WOODEN);
        }
    }
}

void WateringCanBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);

    if (type == Tool::ToolType::WATERING_CANPLUS)
    {
        CCLOG("Using WATERING_CAN++: Watering the crops...");
    }
    ctx.player->changePlanting();
    CCLOG("Using WATERING_CAN: Watering the crops...");
}

void FishingRodBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);

    if (type == Tool::ToolType::FISHING_RODPLUS)
    {
        CCLOG("Using FISHING_ROD++: Fishing...");
    }
    CCLOG("Using FISHING_ROD: Fishing...");

    if (MapManager::getInstance()->getCurrentBlockLabel() == 4)
    {
        if (ctx.position.distance(Vec2(1100, 110)) < 100)
        {
            ItemManager::getInstance(tool.selectedCharacter, tool.nickname)->addItem(Item::ItemType::FISH);
            ctx.player->changeFishing();
        }
    }
}

void FertilizerBehavior::use(Tool &tool)
{
    CCLOG("Using FERTILIZER: Applying fertilizer...");
}

void AnimalFoodBehavior::use(Tool &tool)
{
    auto *player = Player::getInstance(tool.selectedCharacter, tool.nickname);
    player->changeBreeding();
    CCLOG("Using ANIMALFOOD: Feeding animals...");
}

void NullBehavior::use(Tool &)
{
    CCLOG("Invalid tool type or tool not equipped.");
}

ToolBehaviorPtr makeToolBehavior(Tool::ToolType type)
{
    switch (type)
    {
    case Tool::ToolType::HOE:
    case Tool::ToolType::HOEPLUS:
        return ToolBehaviorPtr(new HoeBehavior(type));
    case Tool::ToolType::AXE:
    case Tool::ToolType::AXEPLUS:
        return ToolBehaviorPtr(new AxeBehavior(type));
    case Tool::ToolType::WATERING_CAN:
    case Tool::ToolType::WATERING_CANPLUS:
        return ToolBehaviorPtr(new WateringCanBehavior(type));
    case Tool::ToolType::FISHING_ROD:
    case Tool::ToolType::FISHING_RODPLUS:
        return ToolBehaviorPtr(new FishingRodBehavior(type));
    case Tool::ToolType::FERTILIZER:
        return ToolBehaviorPtr(new FertilizerBehavior());
    case Tool::ToolType::ANIMALFOOD:
        return ToolBehaviorPtr(new AnimalFoodBehavior());
    default:
        return ToolBehaviorPtr(new NullBehavior());
    }
}
