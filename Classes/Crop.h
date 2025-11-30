/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.h
 * File Function: 实现Crop类，完成农作物种植功能
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/22
 ****************************************************************/

#pragma once
#ifndef CROP_H
#define CROP_H

#include "cocos2d.h"
#include <string>
#include <vector>
#include <functional>
USING_NS_CC;
class GameScene; // 前向声明 GameScene 类

class Crop : public Sprite {
public:
    enum class CropState {
        Seed, Growing, Mature, Dead
    };

    Crop();  // 默认构造函数
    static Crop* create(GameScene* scene, const std::string& nickname);
    void setGameScene(GameScene* scene);
    bool init(const std::string& nickname);
    void onMouseDown(EventMouse* event);

    void grow();  // 更新生长状态
    void water();  // 浇水
    void fertilize();    // 施肥
    void update(float dt);  // 每帧更新，根据时间更新生长状态
    void changeTexture(const std::string& texturePath);
    void removeSprite(float dt);
    void deleteDead(Sprite* deadSprite);
    void getGameScene(GameScene* scene);
    CropState getState() const;

    // 定义回调函数类型
    using TextureChangedCallback = std::function<void(const std::string&)>;

    // 设置回调函数
    void setTextureChangedCallback(const TextureChangedCallback& callback);
   
private:
    TextureChangedCallback textureChangedCallback;
    GameScene* gameScene;  // 用于存储对GameScene的引用
    Sprite* myCrop;
    CropState state;   // 当前状态
    int age;           // 当前生长时间（以游戏中的秒为单位）
    int newAge = 0;
    int lastWateredTime;
    float timeElapsed;
    Sprite* deadSprite;
    std::vector<std::string> stageTextures;  // 不同生长阶段的图片路径
    bool watered;      // 是否浇水
};

#endif // CROP_H