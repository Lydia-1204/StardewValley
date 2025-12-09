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
};

class HoeBehavior : public IToolBehavior
{
public:
    explicit HoeBehavior(Tool::ToolType type) : type(type) {}
    void use(Tool &tool) override;

private:
    Tool::ToolType type;
};

class AxeBehavior : public IToolBehavior
{
public:
    explicit AxeBehavior(Tool::ToolType type) : type(type) {}
    void use(Tool &tool) override;

private:
    Tool::ToolType type;
};

class WateringCanBehavior : public IToolBehavior
{
public:
    explicit WateringCanBehavior(Tool::ToolType type) : type(type) {}
    void use(Tool &tool) override;

private:
    Tool::ToolType type;
};

class FishingRodBehavior : public IToolBehavior
{
public:
    explicit FishingRodBehavior(Tool::ToolType type) : type(type) {}
    void use(Tool &tool) override;

private:
    Tool::ToolType type;
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

ToolBehaviorPtr makeToolBehavior(Tool::ToolType type);

#endif // TOOL_BEHAVIOR_H
