/****************************************************************
 * Project Name:  StardewValley
 * File Name:     CreatErrorScene.h
 * File Function: ErrorSceneHelper��Ķ��壬ʵ���쳣�������ת���쳣����
 * Author:        ��С��
 * Update Date:   2024/12/4
 ****************************************************************/

#pragma once
#ifndef ERROR_SCENE_HELPER_H
#define ERROR_SCENE_HELPER_H

#include "cocos2d.h"

class ErrorSceneHelper {
public:
    // �������󳡾��ľ�̬����
    static cocos2d::Scene* createErrorScene(const std::string& errorMessage);
};

#endif // ERROR_SCENE_HELPER_H