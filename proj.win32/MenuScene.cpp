
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LoadingScene.cpp
 * File Function: ʵ��MenuScene�࣬�����Ϸ���˵�����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/6
 ****************************************************************/
// ��������
#include "MenuScene.h"
#include "NewSelectScene.h"        // ����Ϸ����
#include "SaveSelectScene.h"    // �浵ѡ�����
#include "cocos2d.h"
#include"CreateErrorScene.h"
USING_NS_CC;

Scene* MenuScene::createScene()
{
    // ������������
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    if (layer)
        scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
    // ��ʼ������
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for MenuScene!!");
        return false;
    }

    // ��ȡ��Ļ��С��ԭ��
    const auto screenSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
   
    // 1. ��ӱ���ͼ
    auto background = Sprite::create("../Resources/LooseSprites-73/stardewPanorama.png");

    //�����ߴ�
    const Size spriteSize = background->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    float scaleX =screenSize.width / spriteSize.width;
    float scaleY = screenSize.height / spriteSize.height;

    // ѡ���������ű�����ȷ��ͼƬ����������Ļ
    float scale = std::max(scaleX, scaleY);
    

    if (background) {
        background->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));
        background->setScale(scale); // ������Ҫ��������
        this->addChild(background);
    }
    else {
        CCLOG("Failed  to load Menu background image!");
        throw std::runtime_error("Failed to load MenuScene Image!!");
        return false;
    }//ͼƬ���ʧ���׳��쳣
  
    // 2. ��� New ��ť
    auto newGameButton = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            // �л��� NewSelectScene (����Ϸ����)
            try {
                auto newScene = NewSelectScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, newScene, Color3B::BLACK));
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);

            }
        });

    if (newGameButton == NULL)
        throw std::runtime_error("Failed to generate newGameButton!!");


    auto newGameLabel = Label::createWithTTF("NEW GAME", "../Resources/fonts/Marker Felt.ttf", 30);
    newGameLabel->setPosition(newGameButton->getContentSize() / 2); // �������־���
    newGameButton->addChild(newGameLabel); // ��������Ϊ��ť���ӽڵ�

    newGameButton->setPosition(Vec2(screenSize.width / 4, screenSize.height / 4 ));
    //��ť�ߴ�
    const auto buttonSize = newGameButton->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    scaleX = (screenSize.width/10) / buttonSize.width;
    scaleY = (screenSize.height/10)/ buttonSize.height*0.8;
   //scale = std::min(scaleX, scaleY);

    newGameButton->setScale(scaleX,scaleY);
    // 3. ��� Load ��ť
    auto loadButton = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            try {
                // �л��� SaveSelectScene (�浵ѡ�����)
                auto saveScene = SaveSelectScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, saveScene, Color3B::GRAY));
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });
    if (loadButton == NULL)
        throw std::runtime_error("Failed to generate  loadButton!!");


    auto loadGameLabel = Label::createWithTTF(" LOAD ", "../Resources/fonts/Marker Felt.ttf",30);
    loadGameLabel->setPosition(loadButton->getContentSize() / 2); // �������־���
    loadButton->addChild(loadGameLabel); // ��������Ϊ��ť���ӽڵ�

    loadButton->setPosition(Vec2(screenSize.width / 2, screenSize.height/4));
    loadButton->setScale(scaleX, scaleY);

    // 4. ��� Exit ��ť
    auto exitButton = MenuItemImage::create(
        "../Resources/LooseSprites-73/textBox..png",
        "../Resources/LooseSprites-73/textBox..png",
        [](Ref* sender) {
            // �˳���Ϸ
            Director::getInstance()->end();
        });
    if (exitButton == NULL)
        throw std::runtime_error("Failed to generate exitButton!!");

    auto exitLabel = Label::createWithTTF(" EXIT ", "../Resources/fonts/Marker Felt.ttf", 30);
    exitLabel->setPosition(exitButton->getContentSize() / 2); // �������־���
    exitButton->addChild(exitLabel); // ��������Ϊ��ť���ӽڵ�

    exitButton->setPosition(Vec2(screenSize.width /4*3, screenSize.height / 4));
    exitButton->setScale(scaleX, scaleY);

    // 5. �������ͼƬ�� �����ƶ���ͼƬ����**

    // **1. ����һ�����ڵ�**
    auto  movingTitleSprite = Node::create();

    // **2. ���ͼƬ�����ڵ�**
    auto sprite1 = Sprite::create("../Resources/LooseSprites-73/logo..png");
    auto sprite2 = Sprite::create("../Resources/Characters-102/Mariner..png");
    sprite2->setScale(2.0f);

    auto sprite3 = Sprite::create("../Resources/Characters-102/Bouncer..png");
    sprite3->setScale(2.0f);
    if (sprite1 && sprite2 && sprite3) {
        // �������λ��
        sprite1->setPosition(Vec2(0, 0)); //�м�
        // ��ȡ sprite1 �Ŀ�Ⱥ͸߶�
        float offsetX = sprite1->getContentSize().width / 2;  // ����ƫ��
        float offsetY = sprite1->getContentSize().height / 2; // ����ƫ��

        // ��̬���� sprite2 �� sprite3 �����λ��
        sprite2->setPosition(Vec2(-offsetX, -offsetY)); // ���½�
        sprite3->setPosition(Vec2(offsetX, -offsetY));  // ���½�

        // ��ͼƬ��ӵ����ڵ�
        movingTitleSprite->addChild(sprite1);
        movingTitleSprite->addChild(sprite2);
        movingTitleSprite->addChild(sprite3);
    }
    else {
        throw std::runtime_error("Failed to generate MenuMovingTitle!!");
    }
    if (movingTitleSprite) {
        // ���þ����ʼλ��
        movingTitleSprite->setPosition(Vec2(screenSize.width / 2, screenSize.height * 0.618));
        this->addChild(movingTitleSprite);

        // ���������ƶ����յ�λ��
        auto moveLeft = MoveTo::create(2.0f, Vec2(screenSize.width * 0.4f, screenSize.height * 0.618));
        auto moveRight = MoveTo::create(2.0f, Vec2(screenSize.width * 0.6f, screenSize.height * 0.618));

        // ���������ƶ��Ķ�������
        auto moveSequence = Sequence::create(moveLeft, moveRight, nullptr);

        // �����ظ�����
        auto repeatAction = RepeatForever::create(moveSequence);

        // ���ж���
        movingTitleSprite->runAction(repeatAction);
    }
    // 6. �����˵�����Ӱ�ť
    auto menu = Menu::create(newGameButton, loadButton, exitButton, nullptr);
    menu->setPosition(Vec2::ZERO); // �˵�λ������Ϊ (0, 0)����ťʹ������λ��
    this->addChild(menu);

    return true;
}