
/****************************************************************
 * Project Name:  StardewValley
 * File Name:     LoadingScene.cpp
 * File Function: 实现MenuScene类，完成游戏主菜单界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/6
 ****************************************************************/
// 创建场景
#include "Scenes/MenuScene.h"
#include "Scenes/NewSelectScene.h"        // ����Ϸ����
#include "Scenes/SaveSelectScene.h"    // �浵ѡ�����
#include "cocos2d.h"
#include "Scenes/CreateErrorScene.h"
#include "App/SceneRouter.h"
#include "App/SceneUIFacade.h"
USING_NS_CC;

Scene* MenuScene::createScene()
{
    // 创建场景对象
    auto scene = Scene::create();
    auto layer = MenuScene::create();
    if (layer)
        scene->addChild(layer);
    return scene;
}

bool MenuScene::init()
{
    // 初始化父类
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for MenuScene!!");
        return false;
    }

    // 获取屏幕大小和原点
    const auto screenSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();
   

    if (!SceneUIFacade::getInstance()->applyBackground(this, "../Resources/LooseSprites-73/stardewPanorama.png")) {
        CCLOG("Failed  to load Menu background image!");
        throw std::runtime_error("Failed to load MenuScene Image!!");
        return false;
    }
  
    // 2. 添加 New 按钮
    auto newGameButton = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/textBox..png",
        "NEW GAME",
        "../Resources/fonts/Marker Felt.ttf",
        30,
        screenSize,
        0.1f,
        0.1f * 0.8f,
        Vec2(screenSize.width / 4, screenSize.height / 4),
        [](Ref* sender) {
            try {
                SceneRouter::getInstance()->goTo("NewSelect");
            }
            catch (const std::exception& e) {
                CCLOG("Exception caught : %s", e.what());
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
    );

    if (newGameButton == NULL)
        throw std::runtime_error("Failed to generate newGameButton!!");


    
    // 3. 添加 Load 按钮
    auto loadButton = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/textBox..png",
        " LOAD ",
        "../Resources/fonts/Marker Felt.ttf",
        30,
        screenSize,
        0.1f,
        0.1f * 0.8f,
        Vec2(screenSize.width / 2, screenSize.height/4),
        [](Ref* sender) {
            try {
                SceneRouter::getInstance()->goTo("SaveSelect");
            }
            catch (const std::exception& e) {
                CCLOG("Exception caught : %s", e.what());
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
    );
    if (loadButton == NULL)
        throw std::runtime_error("Failed to generate  loadButton!!");


    

    // 4. 添加 Exit 按钮
    auto exitButton = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/textBox..png",
        " EXIT ",
        "../Resources/fonts/Marker Felt.ttf",
        30,
        screenSize,
        0.1f,
        0.1f * 0.8f,
        Vec2(screenSize.width /4*3, screenSize.height / 4),
        [](Ref* sender) {
            Director::getInstance()->end();
        }
    );
    if (exitButton == NULL)
        throw std::runtime_error("Failed to generate exitButton!!");

    

    // 5. 组合三个图片并 创建移动的图片精灵**

    // **1. 创建一个父节点**
    auto  movingTitleSprite = Node::create();

    // **2. 添加图片到父节点**
    auto sprite1 = Sprite::create("../Resources/LooseSprites-73/logo..png");
    auto sprite2 = Sprite::create("../Resources/Characters-102/Mariner..png");
    sprite2->setScale(2.0f);

    auto sprite3 = Sprite::create("../Resources/Characters-102/Bouncer..png");
    sprite3->setScale(2.0f);
    if (sprite1 && sprite2 && sprite3) {
        // 设置相对位置
        sprite1->setPosition(Vec2(0, 0)); //中间
        // 获取 sprite1 的宽度和高度
        float offsetX = sprite1->getContentSize().width / 2;  // 横向偏移
        float offsetY = sprite1->getContentSize().height / 2; // 纵向偏移

        // 动态设置 sprite2 和 sprite3 的相对位置
        sprite2->setPosition(Vec2(-offsetX, -offsetY)); // 左下角
        sprite3->setPosition(Vec2(offsetX, -offsetY));  // 右下角

        // 将图片添加到父节点
        movingTitleSprite->addChild(sprite1);
        movingTitleSprite->addChild(sprite2);
        movingTitleSprite->addChild(sprite3);
    }
    else {
        throw std::runtime_error("Failed to generate MenuMovingTitle!!");
    }
    if (movingTitleSprite) {
        // 设置精灵初始位置
        movingTitleSprite->setPosition(Vec2(screenSize.width / 2, screenSize.height * 0.618));
        this->addChild(movingTitleSprite);

        // 定义左右移动的终点位置
        auto moveLeft = MoveTo::create(2.0f, Vec2(screenSize.width * 0.4f, screenSize.height * 0.618));
        auto moveRight = MoveTo::create(2.0f, Vec2(screenSize.width * 0.6f, screenSize.height * 0.618));

        // 创建左右移动的动作序列
        auto moveSequence = Sequence::create(moveLeft, moveRight, nullptr);

        // 无限重复动作
        auto repeatAction = RepeatForever::create(moveSequence);

        // 运行动作
        movingTitleSprite->runAction(repeatAction);
    }
    // 6. 创建菜单并添加按钮
    auto menu = Menu::create(newGameButton, loadButton, exitButton, nullptr);
    menu->setPosition(Vec2::ZERO); // 菜单位置设置为 (0, 0)，按钮使用自身位置
    this->addChild(menu);

    return true;
}
