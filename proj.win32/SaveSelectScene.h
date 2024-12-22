
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SaveSelectScene.h
 * File Function: SaveSelectScene����������浵ѡ�����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

#pragma once
#ifndef __SAVECSELECT_SCENE_H__
#define __SAVESELECT_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
/*
 * Class Name:     MenuScene
 * Class Function: ʵ�ֲ˵�����
 */
class SaveSelectScene : public cocos2d::Scene
{
public:

    // ��������
    static cocos2d::Scene* createScene();

    // ��ʼ������
    virtual bool init();

    void saveCloseCallback(Ref* pSender);
    // ʵ�� StartupScene ��� create ����
    CREATE_FUNC(SaveSelectScene);

};
#endif