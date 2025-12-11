#pragma once
#ifndef CROP_STATE_H
#define CROP_STATE_H

#include "cocos2d.h"

// 前向声明，防止循环引用
class Crop;
class GameScene;

USING_NS_CC;

// 抽象状态基类
class CropState
{
public:
    virtual ~CropState() {}

    

    // 【使用状态模式重构】状态行为接口：进入状态
    virtual void enter(Crop* crop) = 0;

    // 【使用状态模式重构】状态行为接口：每帧逻辑更新
    virtual void execute(Crop* crop, float dt) = 0;

    // 【使用状态模式重构】状态行为接口：离开状态
    virtual void exit(Crop* crop) = 0;

    // 【使用状态模式重构】状态行为接口：处理交互事件
    virtual void onClick(Crop* crop, EventMouse* event) = 0;

    // 【使用状态模式重构】状态行为接口：处理生长逻辑
    virtual void grow(Crop* crop) = 0;
};

// ==========================================
// 【使用状态模式重构】具体状态类 (Concrete States)
// 作用：实现不同状态下特有的行为逻辑，替代原有的 if-else/switch 分支
// ==========================================

// 1. 种子状态
class SeedState : public CropState
{
public:
    void enter(Crop *crop) override;
    void execute(Crop *crop, float dt) override;
    void exit(Crop *crop) override;
    void onClick(Crop *crop, EventMouse *event) override;
    void grow(Crop *crop) override;
};

// 2. 生长中状态
class GrowingState : public CropState
{
public:
    void enter(Crop *crop) override;
    void execute(Crop *crop, float dt) override;
    void exit(Crop *crop) override;
    void onClick(Crop *crop, EventMouse *event) override;
    void grow(Crop *crop) override;
};

// 3. 成熟状态
class MatureState : public CropState
{
public:
    void enter(Crop *crop) override;
    void execute(Crop *crop, float dt) override;
    void exit(Crop *crop) override;
    void onClick(Crop *crop, EventMouse *event) override;
    void grow(Crop *crop) override;
};

// 4. 枯死状态
class DeadState : public CropState
{
public:
    void enter(Crop *crop) override;
    void execute(Crop *crop, float dt) override;
    void exit(Crop *crop) override;
    void onClick(Crop *crop, EventMouse *event) override;
    void grow(Crop *crop) override;
};

#endif // CROP_STATE_H