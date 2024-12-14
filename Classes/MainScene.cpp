#include "MainScene.h"
#include "Animal.h"
#include "Cow.h"
#include "Chicken.h"
#include "cocos2d.h"


USING_NS_CC;

Scene* MainScene::createScene()
{
    return MainScene::create();
}

// 如果文件不存在，打印有用的错误信息而不是出现段错误。
static void problemLoading(const char* filename)
{
    printf("加载错误：%s\n", filename);
    printf("根据你的编译方式，你可能需要在HelloWorldScene.cpp中的文件名前加上'Resources/'\n");
}

// 在"init"中你需要初始化你的实例
bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 设置背景图片
    auto background = Sprite::create("farm.png");
    if (background == nullptr)
    {
        problemLoading("'farm.png'");
    }
    else
    {
        // 放大背景图片
        background->setScale(visibleSize.width / background->getContentSize().width);

        // 调整锚点到中心，确保背景图片居中显示
        background->setAnchorPoint(Vec2(0.5f, 0.5f));
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        this->addChild(background, -1);
    }

    // 添加动物
    auto chicken = Sprite::create("chicken.png");
    if (chicken == nullptr)
    {
        problemLoading("'chicken.png'");
    }
    else
    {
        chicken->setPosition(Vec2(origin.x + visibleSize.width / 4, origin.y + visibleSize.height / 2));
        this->addChild(chicken, 1);
    }

    auto cow = Sprite::create("cow.png");
    if (cow == nullptr)
    {
        problemLoading("'cow.png'");
    }
    else
    {
        cow->setPosition(Vec2(origin.x + 3 * visibleSize.width / 4, origin.y + visibleSize.height / 2));
        this->addChild(cow, 1);
    }

    return true;
}

void MainScene::menuCloseCallback(Ref* pSender)
{
    // 关闭cocos2d-x游戏场景并退出应用程序
    Director::getInstance()->end();

    /*要返回到原生iOS屏幕（如果存在）而不退出应用程序，不要使用上面的Director::getInstance()->end()，而是触发在RootViewController.mm中创建的自定义事件，如下*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);*/
}

MainScene::MainScene() : animalManager(new AnimalManager()) {}

MainScene::~MainScene() {
    delete animalManager;
}


/*USING_NS_CC;

MainScene::MainScene() : animalManager(new AnimalManager()) {}

MainScene::~MainScene() {
    delete animalManager;
}

MainScene* MainScene::create() {
    MainScene* ret = new MainScene();
    if (ret && ret->init()) {
        ret->autorelease();
        return ret;
    }
    else {
        delete ret;
        return nullptr;
    }
}

bool MainScene::init() {
    // 初始化场景
    if (!Scene::init()) {
        return false;
    }

    // 创建动物管理器并添加一些动物
    animalManager->setFeedSupply(10);

    Cow* cow = new Cow("Bessie", 100, 20, 80, 5);
    Chicken* chicken = new Chicken("Clucky", 80, 30, 70, 3);

    animalManager->addAnimal(cow);
    animalManager->addAnimal(chicken);

    // 渲染动物
    renderAnimals();

    // 启用触摸事件
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}*/

void MainScene::renderAnimals() {
    Cow* cow = new Cow("Bessie", 100, 20, 80, 5);   // 初始化 Cow 对象
    Chicken* chicken = new Chicken("Clucky", 80, 30, 70, 3);   // 初始化 Chicken 对象

    animalManager->addAnimal(cow);
    animalManager->addAnimal(chicken);

    // 渲染动物图标的位置和状态
    cow->setPosition(Vec2(100, 300));
    chicken->setPosition(Vec2(400, 300));

    this->addChild(cow);
    this->addChild(chicken);
}

bool MainScene::onTouchBegan(Touch* touch, Event* event) {
    Vec2 touchLocation = touch->getLocation();

    // 检查点击的位置，假设点击在牛和鸡的区域内
    if (touchLocation.distance(Vec2(100, 300)) < 50) {
        animalManager->feedAnimal("Bessie");
        animalManager->collectProduct("Bessie");
    }
    else if (touchLocation.distance(Vec2(400, 300)) < 50) {
        animalManager->feedAnimal("Clucky");
        animalManager->collectProduct("Clucky");
    }

    animalManager->updateAnimals();
    return true;
}
