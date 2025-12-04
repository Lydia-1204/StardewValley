#pragma once
#ifndef CROP_STATE_H
#define CROP_STATE_H

#include "cocos2d.h"

// 前向声明，防止循环引用
class Crop;
class GameScene;

USING_NS_CC;

// 抽象状态基类
class CropState {
public:
    virtual ~CropState() {}

    // 进入该状态时触发（如：换图片）
    virtual void enter(Crop* crop) = 0;

    // 每一帧的更新逻辑（对应原本的 update）
    virtual void execute(Crop* crop, float dt) = 0;

    // 离开该状态时触发
    virtual void exit(Crop* crop) = 0;

    // 处理点击事件（浇水、收获、施肥）
    virtual void onClick(Crop* crop, EventMouse* event) = 0;

    // 生长周期触发（对应原本的 grow）
    virtual void grow(Crop* crop) = 0;
};

// ================= 具体状态定义 =================

// 1. 种子状态
class SeedState : public CropState {
public:
    void enter(Crop* crop) override;
    void execute(Crop* crop, float dt) override;
    void exit(Crop* crop) override;
    void onClick(Crop* crop, EventMouse* event) override;
    void grow(Crop* crop) override;
};

// 2. 生长中状态
class GrowingState : public CropState {
public:
    void enter(Crop* crop) override;
    void execute(Crop* crop, float dt) override;
    void exit(Crop* crop) override;
    void onClick(Crop* crop, EventMouse* event) override;
    void grow(Crop* crop) override;
};

// 3. 成熟状态
class MatureState : public CropState {
public:
    void enter(Crop* crop) override;
    void execute(Crop* crop, float dt) override;
    void exit(Crop* crop) override;
    void onClick(Crop* crop, EventMouse* event) override;
    void grow(Crop* crop) override;
};

// 4. 枯死状态
class DeadState : public CropState {
public:
    void enter(Crop* crop) override;
    void execute(Crop* crop, float dt) override;
    void exit(Crop* crop) override;
    void onClick(Crop* crop, EventMouse* event) override;
    void grow(Crop* crop) override;
};

#endif // CROP_STATE_H