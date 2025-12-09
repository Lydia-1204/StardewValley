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

class Tool;

class IToolBehavior
{
public:
    virtual ~IToolBehavior() = default;
    virtual void use(Tool &tool) = 0;
    virtual void useAt(Tool &tool, const cocos2d::Vec2 &targetPos);
};

class HoeBehavior : public IToolBehavior
{
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

// Decorator base class
class ToolDecorator : public IToolBehavior
{
public:
    explicit ToolDecorator(ToolBehaviorPtr baseBehavior);
    void use(Tool &tool) override;

protected:
    ToolBehaviorPtr baseBehavior;
};

// Provides enhanced power effect for PLUS tools
class PowerUpDecorator : public ToolDecorator
{
public:
    explicit PowerUpDecorator(ToolBehaviorPtr baseBehavior);
    void use(Tool &tool) override;
};

// Provides wider range effect for PLUS tools
class WideRangeDecorator : public ToolDecorator
{
public:
    explicit WideRangeDecorator(ToolBehaviorPtr baseBehavior);
    void use(Tool &tool) override;
};

ToolBehaviorPtr makeToolBehavior(Tool::ToolType type);

#endif // TOOL_BEHAVIOR_H
