/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CreatErrorScene.h
 * File Function: ErrorSceneHelper类的定义，实现异常捕获后跳转的异常界面
 * Author:        王小萌
 * Update Date:   2024/12/4
 ****************************************************************/

#pragma once
#ifndef ERROR_SCENE_HELPER_H
#define ERROR_SCENE_HELPER_H

#include "cocos2d.h"

class ErrorSceneHelper {
public:
    // 创建错误场景的静态方法
    static cocos2d::Scene* createErrorScene(const std::string& errorMessage);
};

#endif // ERROR_SCENE_HELPER_H