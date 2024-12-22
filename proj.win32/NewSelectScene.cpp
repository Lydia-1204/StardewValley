
/****************************************************************
 * Project Name:  StardewValley
 * File Name:    NewSelectScene.cpp
 * File Function: ʵ��NewSelectScene�࣬�������Ϸ����ѡ���봴������
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/8
 ****************************************************************/

// ��������
#include "MenuScene.h"
#include "NewSelectScene.h"        // ����Ϸ����
#include "GameScene.h"    // �浵ѡ�����
#include "cocos2d.h"
#include"CreateErrorScene.h"
#include "ui/CocosGUI.h"
#include"scaleSpriteToReference.h" //��Է�������
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

    // 1. ��ӱ���
    auto backgroundSheet = Sprite::create("../Resources/LooseSprites-73/letterBG..png");
    const Size spriteSize = backgroundSheet->getContentSize();

    auto background = Sprite::create("../Resources/LooseSprites-73/letterBG..png", Rect(0, 0, spriteSize.width , spriteSize.height ));
    if (background) {
       
        //�����ߴ� 
        const Size spriteSize = backgroundSheet->getContentSize();
        // �����Ⱥ͸߶ȵ����ű���
        float scaleX = screenSize.width / spriteSize.width;
        float scaleY = screenSize.height / spriteSize.height;

        // ѡ���������ű�����ȷ��ͼƬ����������Ļ
        float scale = std::max(scaleX, scaleY);
        background->setScale(scale);
        background->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));

        this->addChild(background, 0);
    }
    else {
        CCLOG("Failed to load CharacterSelect background image!");
        throw std::runtime_error("Failed to load CharacterSelectBackground.png!");
    }
    // 2. ��ӷ��ذ�ť
    auto backButton = MenuItemImage::create(
        "../Resources/LooseSprites-73/DialogBoxGreen..png",
        "../Resources/LooseSprites-73/DialogBoxGreen..png",
        [](Ref* sender) {
            try {
                auto menuScene = MenuScene::createScene();
                Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene, Color3B::WHITE));
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        });

    if (!backButton) {
        throw std::runtime_error("Failed to load BackButton resources!");
    }
    //��ť�ߴ�
    const auto buttonSize = backButton->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    auto scaleX = (screenSize.width / 10) / buttonSize.width;
    auto scaleY = (screenSize.height / 10) / buttonSize.height;
    //scale = std::min(scaleX, scaleY);

    backButton->setScale(scaleX, scaleY);
    backButton->setPosition(Vec2(screenSize.width * 0.1f, screenSize.height * 0.9f));


    auto backLabel = Label::createWithTTF(" Back ", "../Resources/fonts/Marker Felt.ttf", 40);
    backLabel->setPosition(backButton->getContentSize() / 2); // �������־���
    backButton->addChild(backLabel); // ��������Ϊ��ť���ӽڵ�
   
    // 3. ��ӽ�ɫ 1
    //��ɫ1
    auto spriteSheet = Sprite::create("../Resources/Amily.png");
    if(!spriteSheet)
        throw std::runtime_error("Failed to load Character 1 image for NewSelectScene!");
    //��ȡվ������Ծ�������
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
    check1->setVisible(false); // ��ʼ����
    this->addChild(check1);

    // 4. ��ӽ�ɫ 2
    //վ������Ծ��������
   
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
    check2->setVisible(false); // ��ʼ����
    this->addChild(check2);

    // 5. ��Ӵ���������
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = [=](Touch* touch, Event* event) mutable {
        Vec2 touchLocation = touch->getLocation();

        // �жϴ������ĸ���ɫ
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


   // 6.ѡ����������ʾlable
    auto hintLabelBackground = Sprite::create("../Resources/LooseSprites-73/textBox..png"); // ��ʾ�򱳾�ͼƬ
    if (!hintLabelBackground) {
        CCLOG("Error: Failed to load input field background!");
        throw std::runtime_error("Failed to load input field background in NewSelectScene!");
    }

    // ���ñ���λ��
    hintLabelBackground->setPosition(screenSize.width / 2, screenSize.height * 0.9f);
    hintLabelBackground->setScale(4.0f,1.8f);// ������Ҫ����������С
    this->addChild(hintLabelBackground);

    auto hintLabel = Label::createWithTTF("Please choose the character and input your nickname.\n Then press the 'Enter' to start the game!", "../Resources/fonts/Marker Felt.ttf", 25);
    if (!hintLabel) {
        CCLOG("Error: Failed to create hint label!");
        throw std::runtime_error("Failed to create hint label in NewSelectScene!");
    }
    hintLabel->setPosition(screenSize.width / 2,screenSize.height*0.9f); // ����ʾ�屳���Ͼ���
    hintLabel->setTextColor(Color4B::WHITE); // ����������ɫ
    this->addChild(hintLabel);

    // 7. ����ǳ������
    auto nicknameBackground = Sprite::create("../Resources/LooseSprites-73/chatBox..png"); // ����򱳾�ͼƬ
    if (!nicknameBackground) {
        CCLOG("Error: Failed to load input field background!");
        throw std::runtime_error("Failed to load input field background in NewSelectScene!");
    }

    // ���ñ���λ��
    nicknameBackground->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height * 0.3 + origin.y));
    nicknameBackground->setScale(1.2f); // ������Ҫ����������С
    this->addChild(nicknameBackground);

    // �����ǳ������
    ui::TextField* nicknameInput = ui::TextField::create("please enter your name here ", "../Resources/fonts/arial.ttf", 22);
    if (!nicknameInput) {
        CCLOG("Error: Failed to create nickname input field!");
        throw std::runtime_error("Failed to create nickname input field in NewSelectScene!");
    }

    // ���������λ�ã��뱳�����룩
    nicknameInput->setPosition(nicknameBackground->getPosition());
    nicknameInput->setMaxLength(8); // ���Ƴ���
    nicknameInput->setMaxLengthEnabled(true);

    // ����������ı���ɫ�͹����ʽ
    nicknameInput->setTextColor(Color4B::WHITE); // ��������Ϊ��ɫ
    nicknameInput->setCursorEnabled(true);       // ���ù����ʾ�����ѡ�к�����˸��

    // 8.������ʾ2����֧��Ӣ�ģ�*/
    auto hintLabel2 = Label::createWithTTF("Only Supports English !", "../Resources/fonts/Marker Felt.ttf", 25);
    if (!hintLabel2) {
        CCLOG("Error: Failed to create hint label 2!");
        throw std::runtime_error("Failed to create hint label 2 in NewSelectScene!");
    }
    hintLabel2->setPosition(screenSize.width / 2, screenSize.height * 0.23f); // ����ʾ�屳���Ͼ���
    hintLabel2->setTextColor(Color4B::RED); // ����������ɫ
    this->addChild(hintLabel2);
    // ���������¼�
    
    nicknameInput->addEventListener([this](Ref* sender, ui::TextField::EventType type) {
        if (type == ui::TextField::EventType::INSERT_TEXT || type == ui::TextField::EventType::DELETE_BACKWARD) {
            auto textField = static_cast<ui::TextField*>(sender);
            nickname = textField->getString(); // ���³�Ա����
            CCLOG("Nickname updated: %s", nickname.c_str());
        }
        });

    // ��ӵ�����
    this->addChild(nicknameInput);

    // 9. �� Enter ������Ϸ
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyReleased = [=](EventKeyboard::KeyCode keyCode, Event* event) mutable {
        if (keyCode == EventKeyboard::KeyCode::KEY_ENTER) {
            // ����Ƿ�ѡ���˽�ɫ���������ǳ�
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

                    // ������Ϸ����
                    try {
                        auto gameScene = GameScene::createScene(selectedCharacter, nickname);
                        Director::getInstance()->replaceScene(TransitionFade::create(0.5f, gameScene, Color3B::BLACK));
                    }
                    catch (const std::exception& e) {
                        //�����쳣 ��¼��־
                        CCLOG("Exception caught : %s", e.what());
                        //���ر��ô������
                        auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                        Director::getInstance()->replaceScene(errorScene);
                    }
                }
            }
            catch (const std::exception& e) {
                //�����쳣 ��¼��־
                CCLOG("selectedCharacter : %s", selectedCharacter);
                CCLOG("nickname : %s", nickname);
                CCLOG("Exception caught : %s", e.what());
                //���ر��ô������
                nickname.clear();
                selectedCharacter = -1;
                auto errorScene = ErrorSceneHelper::createErrorScene(e.what());
                Director::getInstance()->replaceScene(errorScene);
            }
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);

    // 9. �����˵�����Ӱ�ť
    auto menu = Menu::create(backButton, nullptr);
    menu->setPosition(Vec2::ZERO); // �˵�λ������Ϊ (0, 0)����ťʹ������λ��
    this->addChild(menu);
    return true;
}