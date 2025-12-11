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
#include <utility>

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

const std::vector<Vec2> &treePositions()
{
    static const std::vector<Vec2> positions = {
        {810, 465}, {875, 630}, {900, 385}, {950, 540}, {1050, 440}, {1100, 610}, {1170, 365}};
    return positions;
}

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

Tool::ToolType normalizeToolType(Tool::ToolType type)
{
    switch (type)
    {
    case Tool::ToolType::HOEPLUS:
        return Tool::ToolType::HOE;
    case Tool::ToolType::AXEPLUS:
        return Tool::ToolType::AXE;
    case Tool::ToolType::WATERING_CANPLUS:
        return Tool::ToolType::WATERING_CAN;
    case Tool::ToolType::FISHING_RODPLUS:
        return Tool::ToolType::FISHING_ROD;
    default:
        return type;
    }
}
} // namespace

void IToolBehavior::useAt(Tool &tool, const cocos2d::Vec2 &)
{
    use(tool);
}

void HoeBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);
    useAt(tool, ctx.targetPosition);
}

void HoeBehavior::useAt(Tool &tool, const cocos2d::Vec2 &targetPos)
{
    const auto ctx = makePlayerContext(tool);

    CCLOG("Using HOE: Digging a hole at (%0.1f, %0.1f)...", targetPos.x, targetPos.y);

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

    useAt(tool, ctx.targetPosition);
}

void AxeBehavior::useAt(Tool &tool, const cocos2d::Vec2 &)
{
    const auto ctx = makePlayerContext(tool);

    CCLOG("Using AXE: Chopping a tree...");

    if (MapManager::getInstance()->getCurrentBlockLabel() == 2)
    {
        if (isNearAny(ctx.position, treePositions(), 50.0f))
        {
            ItemManager::getInstance(tool.selectedCharacter, tool.nickname)->addItem(Item::ItemType::WOODEN);
        }
    }
}

void WateringCanBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);

    ctx.player->changePlanting();
    CCLOG("Using WATERING_CAN: Watering the crops...");
}

void FishingRodBehavior::use(Tool &tool)
{
    const auto ctx = makePlayerContext(tool);

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
    const auto normalizedType = normalizeToolType(type);

    switch (normalizedType)
    // 策略选择（Strategy）：根据工具类型创建对应的基础行为实现（工厂方法）
    // 返回的是基础行为对象，后续可由装饰器在外层进行包装扩展
    {
    case Tool::ToolType::HOE:
        return ToolBehaviorPtr(new HoeBehavior());
    case Tool::ToolType::AXE:
        return ToolBehaviorPtr(new AxeBehavior());
    case Tool::ToolType::WATERING_CAN:
        return ToolBehaviorPtr(new WateringCanBehavior());
    case Tool::ToolType::FISHING_ROD:
        return ToolBehaviorPtr(new FishingRodBehavior());
    case Tool::ToolType::FERTILIZER:
        return ToolBehaviorPtr(new FertilizerBehavior());
    case Tool::ToolType::ANIMALFOOD:
        return ToolBehaviorPtr(new AnimalFoodBehavior());
    default:
        return ToolBehaviorPtr(new NullBehavior());
    }
}
        // 基础策略实现：锄头的单格挖掘逻辑，仅作用于指定目标格
        // 装饰器（如 HOEPLUS 的 PowerUpDecorator）会通过多次调用此函数来实现范围或多次命中效果

// Decorator implementations

ToolDecorator::ToolDecorator(ToolBehaviorPtr baseBehavior) : baseBehavior(std::move(baseBehavior)) {}
    // 基础策略实现：斧头的单格/单次斩树逻辑
    // 不在这里添加 PLUS 行为，AXEPLUS 的额外掉落由 PowerUpDecorator 在外层实现

void ToolDecorator::use(Tool &tool)
{
    // 装饰器基类实现：默认将调用委托给被装饰的基础行为，保持行为链的传递
    if (baseBehavior)
    {
        baseBehavior->use(tool);
    }
}
        // 基础策略实现：浇水对单格生效。WideRangeDecorator 可在外层调用 useAt/或多次触发此逻辑以实现多格浇水（范围扩展）

PowerUpDecorator::PowerUpDecorator(ToolBehaviorPtr baseBehavior) : ToolDecorator(std::move(baseBehavior)) {}

void PowerUpDecorator::use(Tool &tool)
{
    if (!baseBehavior)
    {
        return;
    }

        // 基础策略实现：钓鱼的单次检测/掉落逻辑。FISHING_RODPLUS 的额外掉落通过装饰器（PowerUpDecorator）在外层实现，不修改此处核心实现
    // 装饰器实现入口：根据工具的具体 PLUS 类型在外层添加额外效果
    switch (tool.getType())
    {
    // HOEPLUS 扩展：通过多次调用基础 useAt 实现多格/多次命中效果
    case Tool::ToolType::HOEPLUS:
    {
        const auto ctx = makePlayerContext(tool);
        const float step = 16.0f; // tile size
        // 作用在前向方向一条线上的 3 格
        std::vector<Vec2> targets = {
            ctx.targetPosition,
            ctx.targetPosition + Vec2(step, 0),
            ctx.targetPosition - Vec2(step, 0)};
        for (const auto &target : targets)
        {
            baseBehavior->useAt(tool, target);
        }
        CCLOG("Power-up HOE++ triple-hit effect applied (3 tiles).");
        break;
    }
    // AXEPLUS 扩展：在基础掉落之外增加一次木材掉落（不改变基础判定）
    case Tool::ToolType::AXEPLUS:
    {
        ToolDecorator::use(tool);

        const auto ctx = makePlayerContext(tool);
        if (MapManager::getInstance()->getCurrentBlockLabel() == 2 && isNearAny(ctx.position, treePositions(), 50.0f))
        {
            ItemManager::getInstance(tool.selectedCharacter, tool.nickname)->addItem(Item::ItemType::WOODEN);
            CCLOG("Power-up AXE++ extra wood drop.");
        }
        else
        {
            CCLOG("Power-up AXE++ effect applied (no extra drop outside tree range).");
        }
        break;
    }
    // FISHING_RODPLUS 扩展：在成功钓鱼时额外增加一条鱼
    case Tool::ToolType::FISHING_RODPLUS:
    {
        ToolDecorator::use(tool);

        const auto ctx = makePlayerContext(tool);
        if (MapManager::getInstance()->getCurrentBlockLabel() == 4 && ctx.position.distance(Vec2(1100, 110)) < 100)
        {
            ItemManager::getInstance(tool.selectedCharacter, tool.nickname)->addItem(Item::ItemType::FISH);
            CCLOG("Power-up FISHING_ROD++ extra fish caught.");
        }
        else
        {
            CCLOG("Power-up FISHING_ROD++ effect applied (extra fish skipped: not near water).");
        }
        break;
    }
    default:
        ToolDecorator::use(tool);
        CCLOG("Power-up effect applied.");
        break;
    }
}

WideRangeDecorator::WideRangeDecorator(ToolBehaviorPtr baseBehavior) : ToolDecorator(std::move(baseBehavior)) {}

void WideRangeDecorator::use(Tool &tool)
{
    if (!baseBehavior)
    {
        return;
    }

    switch (tool.getType())
    {
    case Tool::ToolType::WATERING_CANPLUS:
    {
        // WATERING_CANPLUS 扩展：扩大浇水范围，外层装饰器负责多格触发
        const auto ctx = makePlayerContext(tool);
        const float step = 16.0f;
        // 双倍面积：在原本目标与一侧偏移各浇一次
        std::vector<float> offsets = {0.0f, step};
        for (float offset : offsets)
        {
            Vec2 target = ctx.targetPosition;
            if (ctx.direction == 0 || ctx.direction == 2)
            {
                target.x += offset;
            }
            else
            {
                target.y += offset;
            }
            baseBehavior->useAt(tool, target);
        }
        CCLOG("Wide-range WATERING_CAN++ double-area effect applied (2 tiles).");
        break;
    }
    default:
        ToolDecorator::use(tool);
        CCLOG("Wide-range effect applied for PLUS tool.");
        break;
    }
}
