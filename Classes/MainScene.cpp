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

// ����ļ������ڣ���ӡ���õĴ�����Ϣ�����ǳ��ֶδ���
static void problemLoading(const char* filename)
{
    printf("���ش���%s\n", filename);
    printf("������ı��뷽ʽ���������Ҫ��HelloWorldScene.cpp�е��ļ���ǰ����'Resources/'\n");
}

// ��"init"������Ҫ��ʼ�����ʵ��
bool MainScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ���ñ���ͼƬ
    auto background = Sprite::create("farm.png");
    if (background == nullptr)
    {
        problemLoading("'farm.png'");
    }
    else
    {
        // �Ŵ󱳾�ͼƬ
        background->setScale(visibleSize.width / background->getContentSize().width);

        // ����ê�㵽���ģ�ȷ������ͼƬ������ʾ
        background->setAnchorPoint(Vec2(0.5f, 0.5f));
        background->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));

        this->addChild(background, -1);
    }

    // ��Ӷ���
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
    // �ر�cocos2d-x��Ϸ�������˳�Ӧ�ó���
    Director::getInstance()->end();

    /*Ҫ���ص�ԭ��iOS��Ļ��������ڣ������˳�Ӧ�ó��򣬲�Ҫʹ�������Director::getInstance()->end()�����Ǵ�����RootViewController.mm�д������Զ����¼�������*/

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
    // ��ʼ������
    if (!Scene::init()) {
        return false;
    }

    // ������������������һЩ����
    animalManager->setFeedSupply(10);

    Cow* cow = new Cow("Bessie", 100, 20, 80, 5);
    Chicken* chicken = new Chicken("Clucky", 80, 30, 70, 3);

    animalManager->addAnimal(cow);
    animalManager->addAnimal(chicken);

    // ��Ⱦ����
    renderAnimals();

    // ���ô����¼�
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = CC_CALLBACK_2(MainScene::onTouchBegan, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}*/

void MainScene::renderAnimals() {
    Cow* cow = new Cow("Bessie", 100, 20, 80, 5);   // ��ʼ�� Cow ����
    Chicken* chicken = new Chicken("Clucky", 80, 30, 70, 3);   // ��ʼ�� Chicken ����

    animalManager->addAnimal(cow);
    animalManager->addAnimal(chicken);

    // ��Ⱦ����ͼ���λ�ú�״̬
    cow->setPosition(Vec2(100, 300));
    chicken->setPosition(Vec2(400, 300));

    this->addChild(cow);
    this->addChild(chicken);
}

bool MainScene::onTouchBegan(Touch* touch, Event* event) {
    Vec2 touchLocation = touch->getLocation();

    // �������λ�ã���������ţ�ͼ���������
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
