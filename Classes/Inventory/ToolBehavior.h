/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ToolBehavior.h
 * File Function: 定义工具策略接口与具体实现
 * Author:        刘彦含 2351591
 * Update Date:   2025/12/09
 *********************************************************************************************************/
#pragma once
#ifndef TOOL_BEHAVIOR_H
#define TOOL_BEHAVIOR_H

#include <memory>
#include "cocos2d.h"
#include "Inventory/Tool.h"

/*
 * 重构说明：使用策略模式（Strategy）和装饰器模式（Decorator）对工具行为进行重构。
 * - 策略模式：`IToolBehavior` 定义工具的基础行为接口，具体实现由 HoeBehavior/AxeBehavior/
 *   WateringCanBehavior/FishingRodBehavior 等提供，便于按需替换或创建基础行为实例。
 * - 装饰器模式：通过 `ToolDecorator` 及其子类 `PowerUpDecorator`、`WideRangeDecorator` 在不修改
 *   基础行为的情况下，为 PLUS 工具动态叠加升级效果（例如多格命中、额外掉落、范围扩展）。
 */

class Tool;

class IToolBehavior
{
public:
    virtual ~IToolBehavior() = default;
    // 策略接口：定义工具的基础行为。
    virtual void use(Tool &tool) = 0;
    // 钩子：允许装饰器在指定坐标调用行为（支持范围/多次作用）。
    virtual void useAt(Tool &tool, const cocos2d::Vec2 &targetPos);
};

class HoeBehavior : public IToolBehavior
{
    // 策略模式：定义工具的基础行为，便于不同工具行为可替换。
public:
    void use(Tool &tool) override;
    void useAt(Tool &tool, const cocos2d::Vec2 &targetPos) override;
};

class AxeBehavior : public IToolBehavior
{
public:
    void use(Tool &tool) override;
    void useAt(Tool &tool, const cocos2d::Vec2 &targetPos) override;
};

class WateringCanBehavior : public IToolBehavior
{
public:
    void use(Tool &tool) override;
};

class FishingRodBehavior : public IToolBehavior
{
public:
    void use(Tool &tool) override;
};

class FertilizerBehavior : public IToolBehavior
{
public:
    void use(Tool &tool) override;
};

class AnimalFoodBehavior : public IToolBehavior
{
public:
    void use(Tool &tool) override;
};

class NullBehavior : public IToolBehavior
{
public:
    void use(Tool &tool) override;
};

// 装饰器基类：包装策略以动态扩展行为
// 装饰器模式：包装策略以动态扩展工具行为（不改基础实现）
class ToolDecorator : public IToolBehavior
{
public:
    explicit ToolDecorator(ToolBehaviorPtr baseBehavior);
    void use(Tool &tool) override;

protected:
    ToolBehaviorPtr baseBehavior;
};

// 为 PLUS 工具提供强化效果
class PowerUpDecorator : public ToolDecorator
{
public:
    explicit PowerUpDecorator(ToolBehaviorPtr baseBehavior);
    void use(Tool &tool) override;
};

// 为 PLUS 工具提供范围扩展效果
class WideRangeDecorator : public ToolDecorator
{
public:
    explicit WideRangeDecorator(ToolBehaviorPtr baseBehavior);
    void use(Tool &tool) override;
};

ToolBehaviorPtr makeToolBehavior(Tool::ToolType type);

#endif // TOOL_BEHAVIOR_H
