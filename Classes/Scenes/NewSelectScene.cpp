
/****************************************************************
 * Project Name:  StardewValley
 * File Name:    NewSelectScene.cpp
 * File Function: 实现NewSelectScene类，完成新游戏人物选择与创建界面
 * Author:        王小萌 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

// 创建场景
#include "Scenes/MenuScene.h"
#include "Scenes/NewSelectScene.h"        
#include "Scenes/GameScene.h"    
#include "cocos2d.h"
#include "Scenes/CreateErrorScene.h"
#include "App/SceneRouter.h"
#include "App/SceneUIFacade.h"
#include "ui/CocosGUI.h"
#include "App/ScaleSpriteToReference.h" 
USING_NS_CC;

Scene* NewSelectScene::createScene() {
    auto scene = Scene::create();
    auto layer = NewSelectScene::create();
    scene->addChild(layer);
    return scene;
}

bool NewSelectScene::init() {
    if (!Scene::init()) {
        throw std::runtime_error("Failed to initialize base layer for NewSelectScene!");
        return false;
    }
   selectedCharacter = 1;
   nickname = "  ";
    const auto screenSize = Director::getInstance()->getVisibleSize();
    const Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 1. 添加背景
    auto backgroundSheet = Sprite::create("../Resources/LooseSprites-73/letterBG..png");
    const Size spriteSize = backgroundSheet->getContentSize();

    auto background = Sprite::create("../Resources/LooseSprites-73/letterBG..png", Rect(0, 0, spriteSize.width , spriteSize.height ));
    if (background) {
       
        //背景尺寸 
        const Size spriteSize = backgroundSheet->getContentSize();
        // 计算宽度和高度的缩放比例
        float scaleX = screenSize.width / spriteSize.width;
        float scaleY = screenSize.height / spriteSize.height;

        // 选择更大的缩放比例，确保图片覆盖整个屏幕
        float scale = std::max(scaleX, scaleY);
        background->setScale(scale);
        background->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));

        this->addChild(background, 0);
    }
    else {
        CCLOG("Failed to load CharacterSelect background image!");
        throw std::runtime_error("Failed to load CharacterSelectBackground.png!");
    }
    // 2. 添加返回按钮
    auto backButton = SceneUIFacade::getInstance()->createLabeledButton(
        "../Resources/LooseSprites-73/DialogBoxGreen..png",
        " Back ",
        "../Resources/fonts/Marker Felt.ttf",
        40,
        screenSize,
        0.10f,
        0.10f,
        Vec2(screenSize.width * 0.1f, screenSize.height * 0.9f),
        [](Ref* sender) {
            try {
                SceneRouter::getInstance()->goTo("Menu");
            }
            catch (const std::exception& e) {
                CCLOG("Exception caught : %s", e.what());
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
    );

    if (!backButton) {
        throw std::runtime_error("Failed to load BackButton resources!");
    }
   
    // 3. 添加角色 1
    //角色1
    auto spriteSheet = Sprite::create("../Resources/Amily.png");
    if(!spriteSheet)
        throw std::runtime_error("Failed to load Character 1 image for NewSelectScene!");
    //截取站立与跳跃人物外表
        Size spriteSize1 = spriteSheet->getContentSize();
        float characterwidth = spriteSize1.width / 4;
        float characterheight = spriteSize1.height / 14;
   
    auto character1 = Sprite::create("../Resources/Amily.png", Rect(0, 0, characterwidth, characterheight));
    character1->setScale(2.5f);
    if (!character1) {
        CCLOG("Error: Failed to load Character 1 image!");
        throw std::runtime_error("Failed to load Character 1 image for NewSelectScene!");
    }

    character1->setPosition(Vec2(screenSize.width * 0.4 + origin.x, screenSize.height * 0.6 + origin.y));
    this->addChild(character1);

    auto box1 = Sprite::create("../Resources/LooseSprites-73/DialogBoxGreen..png");
    if (!box1) {
        CCLOG("Error: Failed to load Box image for Character 1!");
        throw std::runtime_error("Failed to load Box image for Character 1 in NewSelectScene!");
    }
    scaleSpriteToReference(box1, character1, 2.0f);
    box1->setPosition(Vec2(character1->getPositionX(), character1->getPositionY() - 80));
    this->addChild(box1);

    auto check1 = Sprite::create("../Resources/LooseSprites-73/Check.png");
    if (!check1) {
        CCLOG("Error: Failed to load Check image for Character 1!");
        throw std::runtime_error("Failed to load Check image for Character 1 in NewSelectScene!");
    }
    check1->setPosition(box1->getPosition());
    scaleSpriteToReference(check1, character1, 2.0f);
    check1->setVisible(false); // 初始隐藏
    this->addChild(check1);

    // 4. 添加角色 2
    //站立与跳跃两个画面
   
    auto character2 = Sprite::create("../Resources/Harvey.png", Rect(0, 0, characterwidth, characterheight));
    character2->setScale(2.5f);
    if (!character2) {
        CCLOG("Error: Failed to load Character 2 image!");
        throw std::runtime_error("Failed to load Character 2 image for NewSelectScene!");
    }
    character2->setPosition(Vec2(screenSize.width * 0.6 + origin.x, screenSize.height * 0.6 + origin.y));
    this->addChild(character2);

    auto box2 = Sprite::create("../Resources/LooseSprites-73/DialogBoxGreen..png");
    if (!box2) {
        CCLOG("Error: Failed to load Box image for Character 2!");
        throw std::runtime_error("Failed to load Box image for Character 2 in NewSelectScene!");
    }
    box2->setPosition(Vec2(character2->getPositionX(), character2->getPositionY() - 80));
    scaleSpriteToReference(box2, character2, 2.0f);
    this->addChild(box2);

    auto check2 = Sprite::create("../Resources/LooseSprites-73/Check.png");
    if (!check2) {
        CCLOG("Error: Failed to load Check image for Character 2!");
        throw std::runtime_error("Failed to load Check image for Character 2 in NewSelectScene!");
    }
    check2->setPosition(box2->getPosition());
    scaleSpriteToReference(check2, character2, 2.0f);
    check2->setVisible(false); // 初始隐藏
    this->addChild(check2);

    // 5. 添加触摸监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) mutable {
        Vec2 touchLocation = touch->getLocation();

        // 判断触摸到哪个角色
        if (character1->getBoundingBox().containsPoint(touchLocation)|| box1->getBoundingBox().containsPoint(touchLocation)) {
            selectedCharacter = 1;
            check1->setVisible(true);
            check2->setVisible(false);
            character1->setTexture("../Resources/Amily.png");
            Rect cropArea(characterwidth*3, characterheight * 4, characterwidth, characterheight);
            character1->setTextureRect(cropArea);
            CCLOG("Character 1 selected!");
            return true;
        }
        else if (character2->getBoundingBox().containsPoint(touchLocation) || box2->getBoundingBox().containsPoint(touchLocation)) {
            selectedCharacter = 2;
            check1->setVisible(false);
            check2->setVisible(true);
            character2->setTexture("../Resources/Harvey.png");
            Rect cropArea(characterwidth*3, characterheight*5, characterwidth, characterheight);
            character2->setTextureRect(cropArea);
            CCLOG("Character 2 selected!");
            return true;
        }
        return false;
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);


   // 6.选择与输入提示lable
    auto hintLabelBackground = Sprite::create("../Resources/LooseSprites-73/textBox..png"); // 提示框背景图片
    if (!hintLabelBackground) {
        CCLOG("Error: Failed to load input field background!");
        throw std::runtime_error("Failed to load input field background in NewSelectScene!");
    }

    // 设置背景位置
    hintLabelBackground->setPosition(screenSize.width / 2, screenSize.height * 0.9f);
    hintLabelBackground->setScale(4.0f,1.8f);// 根据需要调整背景大小
    this->addChild(hintLabelBackground);

    auto hintLabel = Label::createWithTTF("Please choose the character and input your nickname.\n Then press the 'Enter' to start the game!", "../Resources/fonts/Marker Felt.ttf", 25);
    if (!hintLabel) {
        CCLOG("Error: Failed to create hint label!");
        throw std::runtime_error("Failed to create hint label in NewSelectScene!");
    }
    hintLabel->setPosition(screenSize.width / 2,screenSize.height*0.9f); // 在提示板背景上居中
    hintLabel->setTextColor(Color4B::WHITE); // 设置字体颜色
    this->addChild(hintLabel);

    // 7. 添加昵称输入框
    auto nicknameBackground = Sprite::create("../Resources/LooseSprites-73/chatBox..png"); // 输入框背景图片
    if (!nicknameBackground) {
        CCLOG("Error: Failed to load input field background!");
        throw std::runtime_error("Failed to load input field background in NewSelectScene!");
    }

    // 设置背景位置
    nicknameBackground->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.3 + origin.y));
    nicknameBackground->setScale(1.2f); // 根据需要调整背景大小
    this->addChild(nicknameBackground);

    // 创建昵称输入框
    ui::TextField* nicknameInput = ui::TextField::create("please enter your name here ", "../Resources/fonts/arial.ttf", 22);
    if (!nicknameInput) {
        CCLOG("Error: Failed to create nickname input field!");
        throw std::runtime_error("Failed to create nickname input field in NewSelectScene!");
    }

    // 设置输入框位置（与背景对齐）
    nicknameInput->setPosition(nicknameBackground->getPosition());
    nicknameInput->setMaxLength(8); // 限制长度
    nicknameInput->setMaxLengthEnabled(true);

    // 设置输入框文本颜色和光标样式
    nicknameInput->setTextColor(Color4B::WHITE); // 输入文字为白色
    nicknameInput->setCursorEnabled(true);       // 启用光标显示（鼠标选中后光标闪烁）

    // 8.输入提示2（仅支持英文）*/
    auto hintLabel2 = Label::createWithTTF("Only Supports English !", "../Resources/fonts/Marker Felt.ttf", 25);
    if (!hintLabel2) {
        CCLOG("Error: Failed to create hint label 2!");
        throw std::runtime_error("Failed to create hint label 2 in NewSelectScene!");
    }
    hintLabel2->setPosition(screenSize.width / 2, screenSize.height * 0.23f); // 在提示板背景上居中
    hintLabel2->setTextColor(Color4B::RED); // 设置字体颜色
    this->addChild(hintLabel2);
    // 监听输入事件
    
    nicknameInput->addEventListener([this](Ref* sender, ui::TextField::EventType type) {
        if (type == ui::TextField::EventType::INSERT_TEXT || type == ui::TextField::EventType::DELETE_BACKWARD) {
            auto textField = static_cast<ui::TextField*>(sender);
            nickname = textField->getString(); // 更新成员变量
            CCLOG("Nickname updated: %s", nickname.c_str());
        }
        });

    // 添加到场景
    this->addChild(nicknameInput);

    // 9. 按 Enter 进入游戏
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) mutable {
        if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            // 检查是否选择了角色和输入了昵称
            try {
                if (selectedCharacter == -1) {
                    CCLOG("Error: No character selected!");
                    throw std::runtime_error("No character selected in Character Choose Module!");
                }
                else if (nickname.empty()) {
                    CCLOG("Error: No nickname entered!");
                    throw std::runtime_error("No nickname entered in New_SelectScene!");
                }
                else {
                    CCLOG("Entering game with Character %d and nickname: %s", selectedCharacter, nickname.c_str());

                    // 进入游戏场景
                    try {
                        SceneRouter::getInstance()->goTo("Game", selectedCharacter, nickname);
                    }
                    catch (const std::exception& e) {
                        //捕获异常 记录日志
                        CCLOG("Exception caught : %s", e.what());
                        //加载备用错误界面
                        SceneRouter::getInstance()->goToError(e.what());
                    }
                }
            }
            catch (const std::exception& e) {
                //捕获异常 记录日志
                CCLOG("selectedCharacter : %s", selectedCharacter);
                CCLOG("nickname : %s", nickname);
                CCLOG("Exception caught : %s", e.what());
                //加载备用错误界面
                nickname.clear();
                selectedCharacter = -1;
                SceneRouter::getInstance()->goToError(e.what());
            }
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 9. 创建菜单并添加按钮
    auto menu = Menu::create(backButton, nullptr);
    menu->setPosition(Vec2::ZERO); // 菜单位置设置为 (0, 0)，按钮使用自身位置
    this->addChild(menu);
    return true;
}
