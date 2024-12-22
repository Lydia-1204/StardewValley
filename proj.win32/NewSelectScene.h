
/****************************************************************
 * Project Name:  StardewValley
 * File Name:    NewSelectScene.h
 * File Function: NewSelectScene��Ķ��壬�������Ϸ����ѡ���봴������
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/8
 ****************************************************************/
#pragma once
#ifndef NEWSELECTSCENE_H
#define NEWSELECTSCENE_H

#include "cocos2d.h"

class NewSelectScene : public cocos2d::Scene {
public:
    static cocos2d::Scene* createScene(); // ��������

    virtual bool init();                  // ��ʼ��

    CREATE_FUNC(NewSelectScene);          // ��������
private:
    int selectedCharacter = -1;
    std::string nickname="default guest";
};

#endif // NEWSELECTSCENE_H