/****************************************************************
 * Project Name:  StardewValley
 * File Name:     AnimalManager.cpp
 * File Function: 实现AnimalManager类，完成动物养殖功能
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/
 ****************************************************************/

#include "AnimalManager.h"
#include "Animal.h"
USING_NS_CC;

AnimalManager* AnimalManager::_instance = nullptr;

AnimalManager* AnimalManager::getInstance() {
    // 确保只创建一个实例
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


    // 获取当前屏幕的可视区域大小
    Size visibleSize = Director::getInstance()->getVisibleSize();

    // 创建一个层作为菜单的容器
    menuLayer = Layer::create();
    menuLayer->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));

    // 创建背景图片精灵，并设置锚点为(0.5, 0.5)以居中显示
    auto background = Sprite::create("an_menu.png");
    background->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    background->setAnchorPoint(Vec2(0.5, 0.5));
    background->setScale(2.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(background);

    // 创建一个标签，显示标题
    auto label = Label::createWithTTF("Animal Information", "fonts/Marker Felt.ttf", 30);
    label->setColor(Color3B::BLACK);
    label->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4 + 196)); // 调整标签位置
    menuLayer->addChild(label);

    // 创建一个按钮，点击后关闭菜单
    auto closeBtn = MenuItemImage::create(
        "close.png", // 正常状态的图片
        "close.png", // 选中状态的图片
        CC_CALLBACK_1(AnimalManager::closeAnimalMenu, this));
    closeBtn->setScale(2.5); // 设置图像大小为原来的1.5倍
    closeBtn->setPosition(Vec2(visibleSize.width / 4 + 65, visibleSize.height / 4 + 32)); // 调整关闭按钮位置

    // 创建一个菜单对象，并添加关闭按钮
    auto menu = Menu::create(closeBtn, NULL);
    menu->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 4));
    menuLayer->addChild(menu);

    // 添加背景边框的显示
    auto boardImage1 = Sprite::create("detail.png");
    boardImage1->setPosition(Vec2(visibleSize.width / 4 - 130, visibleSize.height / 4 + 100)); // 调整详细信息图片位置
    boardImage1->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage1->setScale(1.25); // 设置图像大小为原来的1.5倍
    boardImage1->setScaleY(1); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(boardImage1);

    auto boardImage2 = Sprite::create("detail.png");
    boardImage2->setPosition(Vec2(visibleSize.width / 4 - 130, visibleSize.height / 4)); // 调整详细信息图片位置
    boardImage2->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage2->setScale(1.25); // 设置图像大小为原来的1.5倍
    boardImage2->setScaleY(1); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(boardImage2);

    auto boardImage3 = Sprite::create("detail.png");
    boardImage3->setPosition(Vec2(visibleSize.width / 4 - 130, visibleSize.height / 4 - 100)); // 调整详细信息图片位置
    boardImage3->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage3->setScale(1.25); // 设置图像大小为原来的1.5倍
    boardImage3->setScaleY(1); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(boardImage3);

    auto boardImage4 = Sprite::create("detail.png");
    boardImage4->setPosition(Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 + 100)); // 调整详细信息图片位置
    boardImage4->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage4->setScale(1.25); // 设置图像大小为原来的1.5倍
    boardImage4->setScaleY(1); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(boardImage4);

    auto boardImage5 = Sprite::create("detail.png");
    boardImage5->setPosition(Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4)); // 调整详细信息图片位置
    boardImage5->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage5->setScale(1.25); // 设置图像大小为原来的1.5倍
    boardImage5->setScaleY(1); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(boardImage5);

    auto boardImage6 = Sprite::create("detail.png");
    boardImage6->setPosition(Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 - 100)); // 调整详细信息图片位置
    boardImage6->setAnchorPoint(Vec2(0.5, 0.5));
    boardImage6->setScale(1.25); // 设置图像大小为原来的1.5倍
    boardImage6->setScaleY(1); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(boardImage6);

    // 添加动物图像的显示
    auto chickenImage = Sprite::create("chicken_.png");
    chickenImage->setPosition(Vec2(visibleSize.width / 4 - 275, visibleSize.height / 4 + 100)); // 调整详细信息图片位置
    chickenImage->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点为图像中心
    chickenImage->setScale(3.75); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(chickenImage);

    auto pigImage = Sprite::create("pig_.png");
    pigImage->setPosition(Vec2(visibleSize.width / 4 + 40, visibleSize.height / 4 + 100)); // 调整详细信息图片位置
    pigImage->setAnchorPoint(Vec2(0.5, 0.5));
    pigImage->setScale(2.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(pigImage);

    auto catImage = Sprite::create("cat_.png");
    catImage->setPosition(Vec2(visibleSize.width / 4 - 275, visibleSize.height / 4)); // 调整详细信息图片位置
    catImage->setAnchorPoint(Vec2(0.5, 0.5));
    catImage->setScale(3.75); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(catImage);

    auto dogImage = Sprite::create("dog_.png");
    dogImage->setPosition(Vec2(visibleSize.width / 4 + 40, visibleSize.height / 4)); // 调整详细信息图片位置
    dogImage->setAnchorPoint(Vec2(0.5, 0.5));
    dogImage->setScale(2.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(dogImage);

    auto sheepImage = Sprite::create("sheep_.png");
    sheepImage->setPosition(Vec2(visibleSize.width / 4 - 275, visibleSize.height / 4 - 100)); // 调整详细信息图片位置
    sheepImage->setAnchorPoint(Vec2(0.5, 0.5));
    sheepImage->setScale(2.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(sheepImage);

    auto cowImage = Sprite::create("cow_.png");
    cowImage->setPosition(Vec2(visibleSize.width / 4 + 40, visibleSize.height / 4 - 100)); // 调整详细信息图片位置
    cowImage->setAnchorPoint(Vec2(0.5, 0.5));
    cowImage->setScale(2.5); // 设置图像大小为原来的1.5倍
    menuLayer->addChild(cowImage);

    // 添加具体数值的显示
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

    // 将菜单层添加到场景
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(menuLayer, 10); // 设置一个合适的层级，确保菜单层在最上面
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
    // 关闭菜单的逻辑
    if (menuLayer) {
        menuLayer->removeFromParentAndCleanup(true); // 移除菜单层
        menuLayer = nullptr; // 清除引用
    }
}
