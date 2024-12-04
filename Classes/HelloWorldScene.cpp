/*#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// 如果文件不存在，打印有用的错误信息而不是出现段错误。
static void problemLoading(const char* filename)
{
    printf("加载错误：%s\n", filename);
    printf("根据你的编译方式，你可能需要在HelloWorldScene.cpp中的文件名前加上'Resources/'\n");
}

// 在"init"中你需要初始化你的实例
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. 首先调用父类的init
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. 添加一个带有"X"图标的菜单项，点击可以退出程序
    //    你可以修改它。

    // 添加一个“关闭”图标以退出进程。它是一个自动释放的对象
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' 和 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // 创建菜单，它是一个自动释放的对象
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. 在下面添加你的代码...

    // 添加一个显示“Hello World”的标签
    // 创建并初始化一个标签

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // 将标签定位在屏幕中央
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // 将标签作为子节点添加到这个层
        this->addChild(label, 1);
    }

    // 添加“HelloWorld”启动画面"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // 将精灵定位在屏幕中央
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // 将精灵作为子节点添加到这个层
        this->addChild(sprite, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    // 关闭cocos2d-x游戏场景并退出应用程序
    Director::getInstance()->end();

    /*要返回到原生iOS屏幕（如果存在）而不退出应用程序，不要使用上面的Director::getInstance()->end()，而是触发在RootViewController.mm中创建的自定义事件，如下*/

    //EventCustom customEndEvent("game_scene_close_event");
    //_eventDispatcher->dispatchEvent(&customEndEvent);*/
//}

/*#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "Cow.h"
#include "Chicken.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

bool HelloWorld::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 创建关闭按钮
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
    float y = origin.y + closeItem->getContentSize().height / 2;
    closeItem->setPosition(Vec2(x, y));

    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    // 初始化动物管理器
    animalManager = new AnimalManager();

    // 添加动物
    Animal* cow = new Cow("Bessie", 100, 50, 70, "Milk");
    Animal* chicken = new Chicken("Cluck", 80, 60, 80, "Egg");
    animalManager->addAnimal(cow);
    animalManager->addAnimal(chicken);

    // 显示动物状态
    animalInfoLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
    animalInfoLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50));
    this->addChild(animalInfoLabel, 1);
    updateAnimalInfo();

    // 显示状态信息
    statusLabel = Label::createWithTTF("Welcome to the farm!", "fonts/Marker Felt.ttf", 24);
    statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, 50));
    this->addChild(statusLabel, 1);

    // 创建动物图标
    auto cowSprite = Sprite::create("Bessie.png");
    auto chickenSprite = Sprite::create("Cluck.png");
    if (cowSprite)
    {
        cowSprite->setPosition(Vec2(visibleSize.width / 4, visibleSize.height / 2));
        this->addChild(cowSprite);
        animalSprites.insert("Bessie", cowSprite);
    }
    if (chickenSprite)
    {
        chickenSprite->setPosition(Vec2(3 * visibleSize.width / 4, visibleSize.height / 2));
        this->addChild(chickenSprite);
        animalSprites.insert("Cluck", chickenSprite);
    }

    // 添加触摸事件监听器
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

// 更新动物信息显示
void HelloWorld::updateAnimalInfo()
{
    std::string info = "Animals:\n";
    for (Animal* animal : animalManager->getAnimals())
    {
        info += animal->getAnimalName() + ": Hunger=" + std::to_string(animal->getHunger())
            + ", Mood=" + std::to_string(animal->getMood()) + "\n";
    }
    animalInfoLabel->setString(info);
}

// 触摸事件处理喂养和收集操作
bool HelloWorld::onTouchBegan(Touch* touch, Event* event)
{
    Vec2 location = touch->getLocation();

    for (auto it = animalSprites.begin(); it != animalSprites.end(); ++it)
    {
        auto sprite = it->second;
        if (sprite->getBoundingBox().containsPoint(location))
        {
            std::string animalName = it->first;
            if (animalName == "Bessie")
            {
                animalManager->feedAnimal("Bessie");
                statusLabel->setString("Fed Bessie the cow!");
            }
            else if (animalName == "Cluck")
            {
                animalManager->collectProduct("Cluck");
                statusLabel->setString("Collected an egg from Cluck!");
            }
            updateAnimalInfo();
            return true;
        }
    }

    return false;
}

// 关闭按钮回调
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}*/
