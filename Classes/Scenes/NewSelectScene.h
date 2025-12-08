
/****************************************************************
 * Project Name:  StardewValley
 * File Name:    NewSelectScene.h
 * File Function: NewSelectScene类的定义，完成新游戏人物选择与创建界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/8
 ****************************************************************/
#pragma once
#ifndef NEWSELECTSCENE_H
#define NEWSELECTSCENE_H

#include "cocos2d.h"

class NewSelectScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(); // 创建场景

    virtual bool init();                  // 初始化

    CREATE_FUNC(NewSelectScene);          // 创建函数
private:
    int selectedCharacter = -1;
    std::string nickname="default guest";
};

#endif // NEWSELECTSCENE_H
