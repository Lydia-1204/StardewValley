#pragma once
#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "cocos2d.h"
#include "AnimalManager.h"

class MainScene : public cocos2d::Scene {
private:
    AnimalManager* animalManager;  // 动物管理器

public:
    MainScene();
    ~MainScene();

    // 创建主场景
    /*static MainScene* create();

    // 初始化场景
    bool init();*/

    // 响应触摸事件
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    // 渲染动物和处理点击操作
    void renderAnimals();

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // MAINSCENE_H
