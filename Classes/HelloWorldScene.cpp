/*#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// ����ļ������ڣ���ӡ���õĴ�����Ϣ�����ǳ��ֶδ���
static void problemLoading(const char* filename)
{
    printf("���ش���%s\n", filename);
    printf("������ı��뷽ʽ���������Ҫ��HelloWorldScene.cpp�е��ļ���ǰ����'Resources/'\n");
}

// ��"init"������Ҫ��ʼ�����ʵ��
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. ���ȵ��ø����init
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    /////////////////////////////
    // 2. ���һ������"X"ͼ��Ĳ˵����������˳�����
    //    ������޸�����

    // ���һ�����رա�ͼ�����˳����̡�����һ���Զ��ͷŵĶ���
    auto closeItem = MenuItemImage::create(
        "CloseNormal.png",
        "CloseSelected.png",
        CC_CALLBACK_1(HelloWorld::menuCloseCallback, this));

    if (closeItem == nullptr ||
        closeItem->getContentSize().width <= 0 ||
        closeItem->getContentSize().height <= 0)
    {
        problemLoading("'CloseNormal.png' �� 'CloseSelected.png'");
    }
    else
    {
        float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
        float y = origin.y + closeItem->getContentSize().height / 2;
        closeItem->setPosition(Vec2(x, y));
    }

    // �����˵�������һ���Զ��ͷŵĶ���
    auto menu = Menu::create(closeItem, NULL);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    /////////////////////////////
    // 3. �����������Ĵ���...

    // ���һ����ʾ��Hello World���ı�ǩ
    // ��������ʼ��һ����ǩ

    auto label = Label::createWithTTF("Hello World", "fonts/Marker Felt.ttf", 24);
    if (label == nullptr)
    {
        problemLoading("'fonts/Marker Felt.ttf'");
    }
    else
    {
        // ����ǩ��λ����Ļ����
        label->setPosition(Vec2(origin.x + visibleSize.width / 2,
            origin.y + visibleSize.height - label->getContentSize().height));

        // ����ǩ��Ϊ�ӽڵ���ӵ������
        this->addChild(label, 1);
    }

    // ��ӡ�HelloWorld����������"
    auto sprite = Sprite::create("HelloWorld.png");
    if (sprite == nullptr)
    {
        problemLoading("'HelloWorld.png'");
    }
    else
    {
        // �����鶨λ����Ļ����
        sprite->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        // ��������Ϊ�ӽڵ���ӵ������
        this->addChild(sprite, 0);
    }
    return true;
}


void HelloWorld::menuCloseCallback(Ref* pSender)
{
    // �ر�cocos2d-x��Ϸ�������˳�Ӧ�ó���
    Director::getInstance()->end();

    /*Ҫ���ص�ԭ��iOS��Ļ��������ڣ������˳�Ӧ�ó��򣬲�Ҫʹ�������Director::getInstance()->end()�����Ǵ�����RootViewController.mm�д������Զ����¼�������*/

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

    // �����رհ�ť
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

    // ��ʼ�����������
    animalManager = new AnimalManager();

    // ��Ӷ���
    Animal* cow = new Cow("Bessie", 100, 50, 70, "Milk");
    Animal* chicken = new Chicken("Cluck", 80, 60, 80, "Egg");
    animalManager->addAnimal(cow);
    animalManager->addAnimal(chicken);

    // ��ʾ����״̬
    animalInfoLabel = Label::createWithTTF("", "fonts/Marker Felt.ttf", 24);
    animalInfoLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 50));
    this->addChild(animalInfoLabel, 1);
    updateAnimalInfo();

    // ��ʾ״̬��Ϣ
    statusLabel = Label::createWithTTF("Welcome to the farm!", "fonts/Marker Felt.ttf", 24);
    statusLabel->setPosition(Vec2(origin.x + visibleSize.width / 2, 50));
    this->addChild(statusLabel, 1);

    // ��������ͼ��
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

    // ��Ӵ����¼�������
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->onTouchBegan = CC_CALLBACK_2(HelloWorld::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);

    return true;
}

// ���¶�����Ϣ��ʾ
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

// �����¼�����ι�����ռ�����
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

// �رհ�ť�ص�
void HelloWorld::menuCloseCallback(Ref* pSender)
{
    Director::getInstance()->end();
}*/
