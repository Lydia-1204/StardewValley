#pragma once
#ifndef CROP_H
#define CROP_H

#include "cocos2d.h"
#include <string>
#include <vector>
#include <functional>

// 包含状态定义
#include "CropState.h"

USING_NS_CC;

class GameScene;

class Crop : public Sprite {
public:
    Crop();
    virtual ~Crop(); // 记得析构函数中释放 m_state

    static Crop* create(GameScene* scene, const std::string& nickname);
    bool init(const std::string& nickname);

    // 核心循环
    void update(float dt) override;
    void onMouseDown(EventMouse* event);

    // 动作代理
    void grow();
    void water();
    void fertilize();

    // 状态机相关
    void changeState(CropState* newState);

    // 工具方法
    void changeTexture(const std::string& texturePath);
    void removeSprite(float dt); // 移除临时图标(如水滴)

    // Getters / Setters
    void getGameScene(GameScene* scene);
    GameScene* getSceneRef() const { return gameScene; }

    // 回调
    using TextureChangedCallback = std::function<void(const std::string&)>;
    void setTextureChangedCallback(const TextureChangedCallback& callback);

    // 声明友元，让状态类可以直接访问 Crop 的私有数据（简化代码）
    friend class SeedState;
    friend class GrowingState;
    friend class MatureState;
    friend class DeadState;

private:
    CropState* m_state; // 当前状态指针

    GameScene* gameScene;
    Sprite* myCrop;
    Sprite* deadSprite; // 枯死时的额外精灵

    // 数据变量
    int age;             // 当前周期累积时间
    int newAge;          // 总生长时间（决定阶段）
    int lastWateredTime; // 缺水计时器
    float timeElapsed;   // 帧时间累积
    int deadTimes;       // 枯死后的移除倒计时 (替代原来的全局 times)

    bool watered;        // 是否已浇水
    std::vector<std::string> stageTextures;

    TextureChangedCallback textureChangedCallback;
};

#endif // CROP_H