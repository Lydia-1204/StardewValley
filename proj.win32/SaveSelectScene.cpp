
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SaveSelectScene.cpp
 * File Function: ʵ��SaveSelectScene�࣬��ɴ浵ѡ�����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

// ��������
#include "MenuScene.h"
#include "NewSelectScene.h"        // ����Ϸ����
#include "SaveSelectScene.h"    // �浵ѡ�����
#include "cocos2d.h"
#include"CreateErrorScene.h"
USING_NS_CC;

Scene* SaveSelectScene::createScene()
{
    auto scene = Scene::create();
    auto layer = SaveSelectScene::create();
    scene->addChild(layer);
    return scene;
}

bool SaveSelectScene::init()
{
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for SaaveSelectScene!!");
        return false;
    }

    const auto screenSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. ��ӱ���ͼ
    auto background = Sprite::create("../Resources/LooseSprites-73/stardewPanorama.png");
  
    if (background) {
    //�����ߴ�
        const Size spriteSize = background->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
        float scaleX = screenSize.width / spriteSize.width;
        float scaleY = screenSize.height / spriteSize.height;

    // ѡ���������ű�����ȷ��ͼƬ����������Ļ
        float scale = std::max(scaleX, scaleY);
        background->setScale(scale);
        background->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));
        this->addChild(background);
    }
    else {
        CCLOG("Failed  to load Sava Select background image!");
        throw std::runtime_error("Failed to load SaveSelectScene Image!!");
        return false;
    }

   

    // 2. ��ӷ��ذ�ť
    auto backButton = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            // �������˵�
            try {
                auto menuScene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene, Color3B::BLACK));
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });
  
    // ������ֵ���ť��
    auto backLabel = Label::createWithTTF("Back", "../Resources/fonts/Marker Felt.ttf", 24);
    backLabel->setPosition(backButton->getContentSize() / 2); // �������־���
    backButton->addChild(backLabel); // ��������Ϊ��ť���ӽڵ�

    //��ť�ߴ�
    const auto buttonSize = backButton->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    auto scaleX = (screenSize.width / 10) / buttonSize.width*1.2;
    auto scaleY = (screenSize.height / 10) / buttonSize.height * 0.8;
    //scale = std::min(scaleX, scaleY);
    backButton->setScale(scaleX, scaleY);
    backButton->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.8f));


    // 3. ��Ӵ浵1��ť��ʾ����
    auto saveSlot1 = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            try {
                // ���ش浵2�߼� (������ת����Ϸ����)
                CCLOG("Load Save Slot 1...");
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });
   
    auto saveLabel = Label::createWithTTF("Saving1", "../Resources/fonts/Marker Felt.ttf", 24);
    saveLabel->setPosition(saveSlot1->getContentSize() / 2); // �������־���
    saveSlot1->addChild(saveLabel); // ��������Ϊ��ť���ӽڵ�
    saveSlot1->setScale(scaleX, scaleY);
    saveSlot1->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.5f));


  // 3. ��Ӵ浵2��ť��ʾ����
    auto saveSlot2 = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            try {
                // ���ش浵2�߼� (������ת����Ϸ����)
                CCLOG("Load Save Slot 1...");
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });

    auto saveLabel2 = Label::createWithTTF("Saving2", "../Resources/fonts/Marker Felt.ttf", 24);
    saveLabel2->setPosition(saveSlot2->getContentSize() / 2); // �������־���
    saveSlot2->addChild(saveLabel2); // ��������Ϊ��ť���ӽڵ�
    saveSlot2->setScale(scaleX, scaleY);
    saveSlot2->setPosition(Vec2(screenSize.width * 0.5f, screenSize.height * 0.2f));



    auto menu = Menu::create(backButton, saveSlot1, saveSlot2, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu);

    return true;
}