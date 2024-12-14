
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LoadingScene.h
 * File Function: LoadingScene��Ķ��壬Loading����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/4
 ****************************************************************/
#ifndef __Loding_SCENE_H__
#define __Loding_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
class LoadingScene : public cocos2d::Scene
{
public:

    // ��������
    static cocos2d::Scene* createScene();

    // ��ʼ������
    virtual bool init();

    // ʵ�� StartupScene ��� create ����
    CREATE_FUNC(LoadingScene);

};
#endif