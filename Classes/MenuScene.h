
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MenuScene.cpp
 * File Function: MenuScene类的定义，Menu界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/6
 ****************************************************************/
#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
/*
 * Class Name:     MenuScene
 * Class Function: 实现菜单场景
 */
class MenuScene : public cocos2d::Scene
{
public:

    // 创建场景
    static cocos2d::Scene* createScene();
    
    // 初始化场景
    virtual bool init();

    void menuCloseCallback(Ref* pSender);
    // 实现 StartupScene 类的 create 方法
    CREATE_FUNC(MenuScene);

};
#endif