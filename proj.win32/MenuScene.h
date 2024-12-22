
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     MenuScene.cpp
 * File Function: MenuScene��Ķ��壬Menu����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/6
 ****************************************************************/
#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"
#include"SimpleAudioEngine.h"
/*
 * Class Name:     MenuScene
 * Class Function: ʵ�ֲ˵�����
 */
class MenuScene : public cocos2d::Scene
{
public:

    // ��������
    static cocos2d::Scene* createScene();
    
    // ��ʼ������
    virtual bool init();

    void menuCloseCallback(Ref* pSender);
    // ʵ�� StartupScene ��� create ����
    CREATE_FUNC(MenuScene);

};
#endif