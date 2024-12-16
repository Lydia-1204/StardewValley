/****************************************************************
 * Project Name:  StardewValley
 * File Name:     AnimalManager.cpp
 * File Function: ʵ��AnimalManager�࣬��ɶ�����ֳ����
 * Author:        ���庬 2351591
 * Update Date:   2024/12/
 ****************************************************************/

#include "AnimalManager.h"
#include "Animal.h"
USING_NS_CC;

AnimalManager* AnimalManager::_instance = nullptr;

AnimalManager* AnimalManager::getInstance() {
    // ȷ��ֻ����һ��ʵ��
    if (_instance == nullptr) {
        _instance = new AnimalManager();
    }
    return _instance;
}


void AnimalManager::openAnimalMenu() {
    //Animal* chicken = getInstance()->getAnimal("chicken");
    //Animal* cow = getInstance()->getAnimal("cow");
    //Animal* cat = getInstance()->getAnimal("cat");
    //Animal* dog = getInstance()->getAnimal("dog");
    //Animal* pig = getInstance()->getAnimal("pig");
    //Animal* sheep = getInstance()->getAnimal("sheep");
    Animal* chicken = getAnimal("chicken");
    Animal* cow = getAnimal("cow");
    Animal* cat = getAnimal("cat");
    Animal* dog = getAnimal("dog");
    Animal* pig = getAnimal("pig");
    Animal* sheep = getAnimal("sheep");


    // ��ȡ��ǰ��Ļ�Ŀ��������С
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // ����һ������Ϊ�˵�������
    menuLayer = Layer::create();
    menuLayer->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));

    // ��������ͼƬ���飬������ê��Ϊ(0.5, 0.5)�Ծ�����ʾ
    auto background = Sprite::create("an_menu.png");
    background->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(background);

    // ����һ����ǩ����ʾ����
    auto label = Label::createWithTTF("Animal Information", "fonts/Marker Felt.ttf", 30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4 + 196)); // ������ǩλ��
    menuLayer->addChild(label);

    // ����һ����ť�������رղ˵�
    auto closeBtn = MenuItemImage::create(
        "close.png", // ����״̬��ͼƬ
        "close.png", // ѡ��״̬��ͼƬ
        CC_CALLBACK_1(AnimalManager::closeAnimalMenu, this));
    closeBtn->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    closeBtn->setPosition(Vec2(visibleSize.width / 4 + 65, visibleSize.height / 4 + 32)); // �����رհ�ťλ��

    // ����һ���˵����󣬲���ӹرհ�ť
    auto menu = Menu::create(closeBtn, NULL);
    menu->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    menuLayer->addChild(menu);

    // ��ӱ����߿����ʾ
    auto boardImage1 = Sprite::create("detail.png");
    boardImage1->setPosition(Vec2(visibleSize.width / 4 - 130, visibleSize.height / 4 + 100)); // ������ϸ��ϢͼƬλ��
    boardImage1->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage1->setScale(1.25); // ����ͼ���СΪԭ����1.5��
    boardImage1->setScaleY(1); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(boardImage1);

    auto boardImage2 = Sprite::create("detail.png");
    boardImage2->setPosition(Vec2(visibleSize.width / 4 - 130, visibleSize.height / 4)); // ������ϸ��ϢͼƬλ��
    boardImage2->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage2->setScale(1.25); // ����ͼ���СΪԭ����1.5��
    boardImage2->setScaleY(1); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(boardImage2);

    auto boardImage3 = Sprite::create("detail.png");
    boardImage3->setPosition(Vec2(visibleSize.width / 4 - 130, visibleSize.height / 4 - 100)); // ������ϸ��ϢͼƬλ��
    boardImage3->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage3->setScale(1.25); // ����ͼ���СΪԭ����1.5��
    boardImage3->setScaleY(1); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(boardImage3);

    auto boardImage4 = Sprite::create("detail.png");
    boardImage4->setPosition(Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 + 100)); // ������ϸ��ϢͼƬλ��
    boardImage4->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage4->setScale(1.25); // ����ͼ���СΪԭ����1.5��
    boardImage4->setScaleY(1); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(boardImage4);

    auto boardImage5 = Sprite::create("detail.png");
    boardImage5->setPosition(Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4)); // ������ϸ��ϢͼƬλ��
    boardImage5->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage5->setScale(1.25); // ����ͼ���СΪԭ����1.5��
    boardImage5->setScaleY(1); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(boardImage5);

    auto boardImage6 = Sprite::create("detail.png");
    boardImage6->setPosition(Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 - 100)); // ������ϸ��ϢͼƬλ��
    boardImage6->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage6->setScale(1.25); // ����ͼ���СΪԭ����1.5��
    boardImage6->setScaleY(1); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(boardImage6);

    // ��Ӷ���ͼ�����ʾ
    auto chickenImage = Sprite::create("chicken_.png");
    chickenImage->setPosition(Vec2(visibleSize.width / 4 - 275, visibleSize.height / 4 + 100)); // ������ϸ��ϢͼƬλ��
    chickenImage->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��Ϊͼ������
    chickenImage->setScale(3.75); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(chickenImage);

    auto pigImage = Sprite::create("pig_.png");
    pigImage->setPosition(Vec2(visibleSize.width / 4 + 40, visibleSize.height / 4 + 100)); // ������ϸ��ϢͼƬλ��
    pigImage->setAnchorPoint(Vec2(0.5, 0.5));
    pigImage->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(pigImage);

    auto catImage = Sprite::create("cat_.png");
    catImage->setPosition(Vec2(visibleSize.width / 4 - 275, visibleSize.height / 4)); // ������ϸ��ϢͼƬλ��
    catImage->setAnchorPoint(Vec2(0.5, 0.5));
    catImage->setScale(3.75); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(catImage);

    auto dogImage = Sprite::create("dog_.png");
    dogImage->setPosition(Vec2(visibleSize.width / 4 + 40, visibleSize.height / 4)); // ������ϸ��ϢͼƬλ��
    dogImage->setAnchorPoint(Vec2(0.5, 0.5));
    dogImage->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(dogImage);

    auto sheepImage = Sprite::create("sheep_.png");
    sheepImage->setPosition(Vec2(visibleSize.width / 4 - 275, visibleSize.height / 4 - 100)); // ������ϸ��ϢͼƬλ��
    sheepImage->setAnchorPoint(Vec2(0.5, 0.5));
    sheepImage->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(sheepImage);

    auto cowImage = Sprite::create("cow_.png");
    cowImage->setPosition(Vec2(visibleSize.width / 4 + 40, visibleSize.height / 4 - 100)); // ������ϸ��ϢͼƬλ��
    cowImage->setAnchorPoint(Vec2(0.5, 0.5));
    cowImage->setScale(2.5); // ����ͼ���СΪԭ����1.5��
    menuLayer->addChild(cowImage);

    // ��Ӿ�����ֵ����ʾ
    addAnimalInfoH(chicken, Vec2(visibleSize.width / 4 - 132, visibleSize.height / 4 + 120));
    addAnimalInfoM(chicken, Vec2(visibleSize.width / 4 - 133, visibleSize.height / 4 + 100));
    addAnimalInfoA(chicken, Vec2(visibleSize.width / 4 - 131, visibleSize.height / 4 + 80));

    addAnimalInfoH(cat, Vec2(visibleSize.width / 4 - 132, visibleSize.height / 4 + 20));
    addAnimalInfoM(cat, Vec2(visibleSize.width / 4 - 133, visibleSize.height / 4 + 0));
    addAnimalInfoA(cat, Vec2(visibleSize.width / 4 - 131, visibleSize.height / 4 - 20));

    addAnimalInfoH(dog, Vec2(visibleSize.width / 4 - 132, visibleSize.height / 4 - 80));
    addAnimalInfoM(dog, Vec2(visibleSize.width / 4 - 133, visibleSize.height / 4 - 100));
    addAnimalInfoA(dog, Vec2(visibleSize.width / 4 - 131, visibleSize.height / 4 - 120));

    addAnimalInfoH(pig, Vec2(visibleSize.width / 4 + 189, visibleSize.height / 4 + 120));
    addAnimalInfoM(pig, Vec2(visibleSize.width / 4 + 188, visibleSize.height / 4 + 100));
    addAnimalInfoA(pig, Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 + 80));

    addAnimalInfoH(cow, Vec2(visibleSize.width / 4 + 189, visibleSize.height / 4 + 20));
    addAnimalInfoM(cow, Vec2(visibleSize.width / 4 + 188, visibleSize.height / 4));
    addAnimalInfoA(cow, Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 - 20));

    addAnimalInfoH(sheep, Vec2(visibleSize.width / 4 + 189, visibleSize.height / 4 - 80));
    addAnimalInfoM(sheep, Vec2(visibleSize.width / 4 + 188, visibleSize.height / 4 - 100));
    addAnimalInfoA(sheep, Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 - 120));

    // ���˵�����ӵ�����
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(menuLayer, 10); // ����һ�����ʵĲ㼶��ȷ���˵�����������
}

void AnimalManager::addAnimalInfoH(Animal* animal, const Vec2& position) {
    if (animal) {
        auto labelHealth = Label::createWithTTF("Health: " + std::to_string(animal->getHealth()) + " / 100", "fonts/Marker Felt.ttf", 20);
        labelHealth->setPosition(position);
        menuLayer->addChild(labelHealth);
    }
}

void AnimalManager::addAnimalInfoM(Animal* animal, const Vec2& position) {
    if (animal) {
        auto labelHealth = Label::createWithTTF("Mood : " + std::to_string(animal->getMood()) + " / 300", "fonts/Marker Felt.ttf", 20);
        labelHealth->setPosition(position);
        menuLayer->addChild(labelHealth);
    }
}

void AnimalManager::addAnimalInfoA(Animal* animal, const Vec2& position) {
    if (animal) {
        auto labelHealth = Label::createWithTTF("Affection : " + std::to_string(animal->getAffection()) + "/1000", "fonts/Marker Felt.ttf", 20);
        labelHealth->setPosition(position);
        menuLayer->addChild(labelHealth);
    }
}

void AnimalManager::closeAnimalMenu(cocos2d::Ref* sender) {
    // �رղ˵����߼�
    if (menuLayer) {
        menuLayer->removeFromParentAndCleanup(true); // �Ƴ��˵���
        menuLayer = nullptr; // �������
    }
}

