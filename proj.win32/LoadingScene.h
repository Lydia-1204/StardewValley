
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LoadingScene.h
 * File Function: LoadingScene类的定义，Loading界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/4
 ****************************************************************/
#ifndef __Loding_SCENE_H__
#define __Loding_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
class LoadingScene : public cocos2d::Scene
{
public:

    // 创建场景
    static cocos2d::Scene* createScene();

    // 初始化场景
    virtual bool init();

    // 实现 StartupScene 类的 create 方法
    CREATE_FUNC(LoadingScene);

};
#endif