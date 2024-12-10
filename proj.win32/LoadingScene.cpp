
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LoadingScene.cpp
 * File Function: ʵ��LoadingScene�࣬�����Ϸ���ؽ���
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/4
 ****************************************************************/
#include"loadingScene.h"
#include "ui/CocosGUI.h"
#include "proj.win32/Constant.h"
#include "AudioEngine.h"
#include"CreateErrorScene.h"
USING_NS_CC;
using namespace cocos2d::experimental;
// ��������
Scene* LoadingScene::createScene()
{
    auto scene = Scene::create();
    auto layer = LoadingScene::create();
    scene->addChild(layer);
    return scene;
}

// ��ʼ������
bool LoadingScene::init()
{
    //��������
    AudioEngine::play2d("../Resources/Stardew Valley Overture.mp3", true, 0.7f);
    
    // ��������
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for LoadingScene!!");
        return false;
    }

    // ��ȡ��Ļ�ߴ� 
    const auto screen_Size = cocos2d::Director::getInstance()->getVisibleSize();
   // ���ر���
    const auto backGround = Sprite::create("../Resources/LoadingScene.png");
   
     //�����ߴ�
    const Size spriteSize = backGround->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    float scaleX = screen_Size.width / spriteSize.width;
    float scaleY = screen_Size.height / spriteSize.height;

    // ѡ���������ű�����ȷ��ͼƬ����������Ļ
    float scale = std::max(scaleX, scaleY);

 
    if (backGround) 
        // �������ű���
        backGround->setScale(scale);
    else {
        CCLOG("Failed  to load  LoadingScene background image!");
        throw std::runtime_error("Failed to load LoadingScene Image!!");
    }
    //ͼƬ��λ
    backGround->setPosition(Vec2(screen_Size.width / 2, screen_Size.height / 2));
    this->addChild(backGround);

    // ����ui������
    auto LoadingProgressBar = cocos2d::ui::LoadingBar::create("../Resources/LoadingBar.png");
    if(LoadingProgressBar==NULL)
        throw std::runtime_error("Failed to generate LoadingProgressBar !!");
    LoadingProgressBar->setPosition(Vec2(screen_Size.width / 2, screen_Size.height / 2 + LOADING_SCENE_LOADINGBAR_OFFSET_Y));
    LoadingProgressBar->setPercent(0);
    this->addChild(LoadingProgressBar);

    // ������ǩ����ʾ����
    auto progressLabel = Label::createWithTTF("0%", "../Resources/fonts/Marker Felt.ttf", LOADING_SCENE_FONT_SIZE);
    if (progressLabel == NULL)
        throw std::runtime_error("Failed to generate  progressLabel !!");
    progressLabel->setPosition(Vec2(LoadingProgressBar->getPosition().x - LoadingProgressBar->getContentSize().width / 2 + LOADING_SCENE_LOADING_LABEL_OFFSET_X, LoadingProgressBar->getPosition().y));
    progressLabel->setVisible(false);
    this->addChild(progressLabel);

    

   // ���½������ͱ�ǩ
   constexpr float intervalTime = LOADING_SCENE_DURATION_TIME / 100.0f; // ÿ 1% ����ʱ����
   for (int i = 0; i <= 100; i++) {
       this->scheduleOnce([LoadingProgressBar, progressLabel, i](float dt) {
           progressLabel->setVisible(i >= 10);
           LoadingProgressBar->setPercent(i);
           progressLabel->setString(std::to_string(i) + "%");
           float percentage = LoadingProgressBar->getPercent() / 100.0f;
           float x_Position = LoadingProgressBar->getPosition().x - LoadingProgressBar->getContentSize().width / 2 + LoadingProgressBar->getContentSize().width * percentage + LOADING_SCENE_LOADING_LABEL_OFFSET_X;
           progressLabel->setPosition(Vec2(x_Position, LoadingProgressBar->getPosition().y));
           }, intervalTime * i, "UpdateLoadingSceneLoadingBar" + std::to_string(i));
   }

   //���ü�ʱ��
   this->scheduleOnce([](float dt) {
       try {
       auto transition = cocos2d::TransitionFade::create(SCENE_TRANSITION_DURATION, MenuScene::createScene(), cocos2d::Color3B::WHITE);
       cocos2d::Director::getInstance()->replaceScene(transition);
       }catch(const std::exception& e) {
           //�����쳣 ��¼��־
           CCLOG("Exception caught : %s", e.what());
           //���ر��ô������
           auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
           Director::getInstance()->replaceScene(errorScene);
       }
       }, LOADING_SCENE_DURATION_TIME + SCENE_TRANSITION_DURATION, "StartupSceneToMenuScene");

    return true;
}