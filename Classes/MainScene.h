#pragma once
#ifndef MAINSCENE_H
#define MAINSCENE_H

#include "cocos2d.h"
#include "AnimalManager.h"

class MainScene : public cocos2d::Scene {
private:
    AnimalManager* animalManager;  // ���������

public:
    MainScene();
    ~MainScene();

    // ����������
    /*static MainScene* create();

    // ��ʼ������
    bool init();*/

    // ��Ӧ�����¼�
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);

    // ��Ⱦ����ʹ���������
    void renderAnimals();

    static cocos2d::Scene* createScene();

    virtual bool init();

    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(MainScene);
};

#endif // MAINSCENE_H
