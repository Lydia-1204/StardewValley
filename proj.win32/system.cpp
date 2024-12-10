/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SysttemScene.cpp
 * File Function: ʵ��SystemScene�࣬չʾ��Ϸ������ͻ������ܣ�ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 ****************************************************************/

#include "AppDelegate.h"
#include "GameScene.h"
#include "MenuScene.h"

USING_NS_CC;


int main(int argc, char** argv) {
    // ��ʼ��Ӧ�ó���
    AppDelegate app;
    Application::getInstance()->setAnimationInterval(1.0 / 60.0); // ����֡��

    // ��ʼ�� Director
    auto director = Director::getInstance();
    auto glView = director->getOpenGLView();
    if (!glView) {
        glView = GLViewImpl::create("Farm Simulation Game");
        director->setOpenGLView(glView);
    }

    // ���÷ֱ���
    glView->setDesignResolutionSize(1280, 720, ResolutionPolicy::SHOW_ALL);

    // �������˵�����
    auto mainMenuScene = MenuScene::createScene();
    director->runWithScene(mainMenuScene);

    // ������Ϸ��ѭ��
    return Application::getInstance()->run();
}