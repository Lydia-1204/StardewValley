
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SaveSelectScene.h
 * File Function: SaveSelectScene类的声明，存档选择界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

#pragma once
#ifndef __SAVECSELECT_SCENE_H__
#define __SAVESELECT_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
/*
 * Class Name:     MenuScene
 * Class Function: 实现菜单场景
 */
class SaveSelectScene : public cocos2d::Scene
{
public:

    // 创建场景
    static cocos2d::Scene* createScene();

    // 初始化场景
    virtual bool init();

    void saveCloseCallback(Ref* pSender);
    // 实现 StartupScene 类的 create 方法
    CREATE_FUNC(SaveSelectScene);

};
#endif