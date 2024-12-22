/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Animal.cpp
 * File Function: ʵ��Animal�࣬��ɶ�����ֳ����
 * Author:        ���庬 2351591
 * Update Date:   2024/12/22
 ****************************************************************/

#include "Animal.h"
#include "GameScene.h"
#include"map.h"
USING_NS_CC;


Animal* Animal::create(int targetAnimal, GameScene* scene) {
    Animal* animal = new (std::nothrow) Animal();
    animal->getGameScene(scene);  // ����GameScene������
    if (animal && animal->init(targetAnimal)) {
        animal->autorelease();
        return animal;
    }
    delete animal;
    return nullptr;
}

bool Animal::init(int targetAnimal) {
    if (!Node::init()) {
        return false;
    }

    std::string texturePath;
    if (targetAnimal == 1) {
        texturePath = "../Resources/Chicken.png";
        _picturename = "Chicken.png";
        name = "chicken";
    }
    else if (targetAnimal == 2) {
        _picturename = "Cow.png";
        texturePath = "../Resources/Cow.png";
        name = "cow";

    }
    else if (targetAnimal == 3) {
        _picturename = "Cat.png";
        texturePath = "../Resources/Cat.png";
        name = "cat";

    }
    else if (targetAnimal == 4) {
        _picturename = "Dog.png";
        texturePath = "../Resources/Dog.png";
        name = "dog";

    }
    else if (targetAnimal == 5) {
        _picturename = "Pig.png";
        texturePath = "../Resources/Pig.png";
        name = "pig";

    }
    else if (targetAnimal == 6) {
        _picturename = "Sheep.png";
        texturePath = "../Resources/Sheep.png";
        name = "sheep";

    }
    else {
        throw("This animal didn't exist!!");
    }
    CCLOG("Loading texture from: %s", texturePath.c_str());



    an_sprite = Sprite::create(texturePath, Rect(0, 0, 18, 19));
    if (!an_sprite) {
        throw("Animal created failed!!");
    }
    else {
        this->addChild(an_sprite);
    }

    Vec2 waterBowlPosition = Vec2(25, 70); // ��Զ�Ӧ����ĳ�ʼ�����λ��
    placeWaterBowl(waterBowlPosition);

    // ����������¼�������
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = std::bind(&Animal::onMouseDown, this, std::placeholders::_1);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // ��ʱ���¶���״̬
    schedule(CC_SCHEDULE_SELECTOR(Animal::update), 1.0f / 60.0f);

    return true;
}

Vec2 Animal::getCurrentDirection() {
    // ��� an_sprite �Ƿ�Ϊ��ָ��
    if (an_sprite == nullptr) {
        throw("Animal direction error!!");
    }
    // ��ȡ��ǰλ��
    Vec2 currentPosition = an_sprite->getPosition();

    // ��� lastPosition δ����ʼ���������뵱ǰλ����ͬ������ Vec2(0, 0)
    if (lastPosition == currentPosition) {
        return Vec2(0, 0);
    }

    // ���㲢���ص�ǰ����
    Vec2 direction = currentPosition - lastPosition;
    lastPosition = currentPosition; // ���� lastPosition Ϊ��ǰλ��
    return direction;
}

// �� update �����е��� updateDirection
void Animal::update(float dt) {
    updateDirection(dt, _picturename);
    // ���������߼�...
    timeEscaped += dt;
    if (timeEscaped >= 10.0f) { // health ��ʱ��ݼ�
        timeEscaped = 0;
        health--;
    }
    if (MapManager::getInstance()->currentMapLabel == 1) {

        this->setVisible(true);
    }
    else {
        this->setVisible(false);
    }

}

void Animal::updateDirection(float dt, const std::string& picturename) {
    Vec2 direction = getCurrentDirection();

    // ��������ͼ����ֻ����һ�Σ�
    bool textureLoaded = false;
    Texture2D* texture;
    if (!textureLoaded) {
        // ���ش�ͼ����
        texture = Director::getInstance()->getTextureCache()->addImage(picturename.c_str());
        if (texture) {
            textureLoaded = true;  // ��������Ѽ���
        }
        else {
            CCLOG("Error: Failed to load sprite sheet texture.");
            return;
        }
    }

    // ȷ�����������仯ʱ�Ÿ��¾���֡
    if (direction.x > 0) { // �����ƶ�
        // �����ҷ���ľ�������
        Rect rectForRight;
        if (picturename == "Chicken.png") {
            rectForRight = Rect(16, 16, 16, 16);  // �������Ƶľ��������ǴӴ�ͼ�� (3*18, 3*19) ��ʼ����СΪ 18x19
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForRight = Rect(0, 32, 32, 32);  // �������Ƶľ��������ǴӴ�ͼ�� (3*18, 3*19) ��ʼ����СΪ 18x19
        }
        else {
            rectForRight = Rect(0, 32, 32, 32);  // �������Ƶľ��������ǴӴ�ͼ�� (3*18, 3*19) ��ʼ����СΪ 18x19
        }

        SpriteFrame* spriteFrameRight = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForRight);
        an_sprite->setSpriteFrame(spriteFrameRight);
        if (picturename == "Cow.png" || picturename == "Pig.png" || picturename == "Sheep.png") {
            an_sprite->setFlippedX(false);
        }

    }
    else if (direction.x < 0) { // �����ƶ�
        // ��������ľ�������
        Rect rectForLeft;
        if (picturename == "Chicken.png") {
            rectForLeft = Rect(16, 0, 16, 16);  // �������Ƶľ�������
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForLeft = Rect(0, 32 * 3, 32, 32);  // �������Ƶľ�������
        }
        else {
            // ��Ҫ���ҷ�ת
            rectForLeft = Rect(0, 32, 32, 32);

        }
        SpriteFrame* spriteFrameLeft = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForLeft);
        an_sprite->setSpriteFrame(spriteFrameLeft);
        if (picturename == "Cow.png" || picturename == "Pig.png" || picturename == "Sheep.png") {
            an_sprite->setFlippedX(true); // ����ʱ��ת
        }
    }
    else if (direction.y > 0) { // �����ƶ�
        // �����Ϸ���ľ�������
        Rect rectForUp;
        if (picturename == "Chicken.png") {
            rectForUp = Rect(16, 16 * 2, 16, 16);  // �������Ƶľ�������
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForUp = Rect(0, 32 * 2, 32, 32);  // �������Ƶľ�������
        }
        else {
            rectForUp = Rect(0, 32 * 2, 32, 32);  // �������Ƶľ��������ǴӴ�ͼ�� (3*18, 3*19) ��ʼ����СΪ 18x19
        }
        SpriteFrame* spriteFrameUp = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForUp);
        an_sprite->setSpriteFrame(spriteFrameUp);
    }
    else if (direction.y < 0) { // �����ƶ�
        // �����·���ľ�������
        Rect rectForDown;
        if (picturename == "Chicken.png") {
            rectForDown = Rect(16, 16 * 6, 16, 16);  // �������Ƶľ�������
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForDown = Rect(0, 0, 32, 32);  // �������Ƶľ�������
        }
        else {
            rectForDown = Rect(0, 0, 32, 32);  // �������Ƶľ��������ǴӴ�ͼ�� (3*18, 3*19) ��ʼ����СΪ 18x19
        }
        SpriteFrame* spriteFrameDown = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForDown);
        an_sprite->setSpriteFrame(spriteFrameDown);
    }

  //  CCLOG("Direction: x = %f, y = %f", direction.x, direction.y);
}

void Animal::moveAlongPath(const std::vector<Vec2>& path, const std::string& picturename) {
    // ʹ�� ActionInterval ��Ϊ action ����������
    ActionInterval* action = nullptr;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        // MoveTo::create ���ص��� FiniteTimeAction ���ͣ���Ҫת��Ϊ ActionInterval ����
        auto moveAction = static_cast<ActionInterval*>(MoveTo::create(2.0f, path[i + 1]));

        // ʹ�� Sequence::createWithTwoActions ���� Sequence����Ҫ���� FiniteTimeAction ����
        action = (i == 0) ? moveAction : Sequence::createWithTwoActions(static_cast<FiniteTimeAction*>(action), static_cast<FiniteTimeAction*>(moveAction));
    }

    // RepeatForever::create ��Ҫһ�� ActionInterval ����
    auto repeatAction = RepeatForever::create(action);
    an_sprite->runAction(repeatAction);

    // ��Ҫȷ���˶������в�ͣ�ظ��·��򣨼�����֡��
    schedule([this, picturename](float dt) {
        // ���˶�ʱ���·��򣬲�����ͼƬ����
        updateDirection(dt, picturename);
        }, 1.0f / 60.0f, "update_direction_key");
}

void Animal::initialmove(const std::string& picturename) {
    Vec2 startPosition = an_sprite->getPosition(); // ��¼��ʼλ��
    lastPosition = startPosition; // ����lastPositionΪ��ʼλ��

    std::vector<Vec2> path;
    // ����·����
    if (getAnimalType() == AnimalType::CAT || getAnimalType() == AnimalType::DOG) {
        path = {
         Vec2(startPosition.x, startPosition.y),
         Vec2(startPosition.x + 50, startPosition.y),
         Vec2(startPosition.x, startPosition.y)
        };
    }
    else {
        path = {
         Vec2(startPosition.x, startPosition.y),
         Vec2(startPosition.x + 50, startPosition.y),
         Vec2(startPosition.x + 50, startPosition.y + 50),
         Vec2(startPosition.x, startPosition.y + 50),
         Vec2(startPosition.x, startPosition.y)
        };

    }

    // ����·���ƶ�
    moveAlongPath(path, picturename);

}

// ʵ�ֻ�ȡ�������͵ĺ���
Animal::AnimalType Animal::getAnimalType() const {
    if (name == "chicken") return AnimalType::CHICKEN;
    else if (name == "cow") return AnimalType::COW;
    else if (name == "cat") return AnimalType::CAT;
    else if (name == "dog") return AnimalType::DOG;
    else if (name == "pig") return AnimalType::PIG;
    else if (name == "sheep") return AnimalType::SHEEP;
    return AnimalType::CHICKEN; // Ĭ������
}

void Animal::onMouseDown(EventMouse* event) {
    // ��ȡ���������Ļ����
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    Vec2 mouseLocation = mouseEvent->getLocation();

    // ��ӵ��������+�����Աߵ��ж��߼�
    auto playerPos = Player::getInstance(1, "guest")->getPosition();
    auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����
    
   // CCLOG("mouseDown");
  //  CCLOG("locationInWorld Mouse Position: %f, %f", locationInWorld.x, locationInWorld.y);
   // CCLOG("players locationInWorld: %f, %f", playerPos.x, playerPos.y);
  //  CCLOG("animals locationInWorld: %f, %f", this->getPosition().x, this->getPosition().y);
    // ��ҿ������ﲢ���ʱ������
    if (locationInWorld.distance(this->getPosition()) < 100&& playerPos.distance(this->getPosition()) < 100) {

        // ���������
        if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {

            // ��ȡ��ǰʱ���

            auto now = std::chrono::steady_clock::now();

            

            // ����Ƿ���˫��
            if (wasDoubleClick) {
               // ����ж��Ƿ�Я��ʳ��Ĵ���
                toolManager = ToolManager::getInstance(1,"guest");
                if (toolManager && toolManager->selectedToolIndex >= 0 &&
                    toolManager->selectedToolIndex < toolManager->tools.size() && toolManager->tools[toolManager->selectedToolIndex]->getType() == Tool::ToolType::ANIMALFOOD) {

                    // ����˫�����
                    wasDoubleClick = false;
                    // ִ��ιʳ����
                    feed();  // ����feed��������Ҫ���������ߴ����ʵ��Ĳ���
                    CCLOG("Double left click on Animal, animal fed.");
                }
            }

            else {
                // ���Ϊ˫������
                wasDoubleClick = true;
                // ������ε����ʱ������ȷ���Ƿ�Ϊ˫��
                if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count() < 500) {
                    // ʱ����С��500���룬��Ϊ˫��
                    wasDoubleClick = true;
                }
                else {

                    // ʱ��������500���룬����˫�������ñ��
                    wasDoubleClick = false;
                    // ��齡��������ͺøж��Ƿ񶼴ﵽ����ֵ
                    if (health == 100 && mood == 300 ) {
                        // ���ݶ������Ͳ�����Ӧ�Ĳ���
                        Item::ItemType product = produceProduct();
                        //CCLOG("Produced product: %s", product.c_str());
                     
                        ItemManager::getInstance(1,"guest")->addItem(product);
                    }
                    else {
                        // ���û�дﵽ��ֵ�����ºøжȺ�����
                        updateFavorability();
                        updateMood();
                        CCLOG("Left click on Animal, favorability and mood updated.");
                    }
                }
                // �����ϴε��ʱ��
                lastClickTime = now;
            }
        }
        else if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
            // ����Ҽ����
            openAnimalMenu();
            CCLOG("Right click on Animal, menu opened.");
        }
    }
}

void Animal::updateFavorability() {
    // ���ӻ���¶���ĺøж��߼�
    affection += 15; // ����ÿ�ε������15��øж�
    if (affection > 1000) affection = 1000;  // ���øж�Ϊ1000
}

void Animal::updateMood() {
    // ���¶������������߼�
    mood += 35; // ���������������Ϊ����
    if (mood > 300 || mood == 300)
    {
        mood = 300;
        // ������ֵ�ﵽ���ʱ��ʾһ��ͼƬ
        std::string imagePath = "../Resources/happy_animal.png";
        Sprite* happySprite = Sprite::create(imagePath);

        // ���ͼƬ�Ƿ���سɹ�
        if (happySprite) {
            // ����ͼƬ��λ�ã������Զ��ﾫ�������Ϊλ�òο�
            happySprite->setPosition(an_sprite->getPosition());

            // ��ͼƬ��ӵ��������ڵĽڵ�
            this->addChild(happySprite, 1, "happySprite"); // ���ñ�ǩ"happySprite"

            // ����һ����ʱ���Ƴ�ͼƬ����ֹ��һֱ��ʾ
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(Animal::removeHappySprite), 1.0f); // 2����Ƴ�ͼƬ
        }
    }
}

void Animal::removeHappySprite(float dt) {
    // ���ݱ�ǩ��ȡͼƬ���Ƴ���
    Node* happySprite = this->getChildByName("happySprite");
    if (happySprite) {
        this->removeChild(happySprite);
    }
}

void Animal::feed() {
    health += 15;
    if (health > 100) health = 100;
    CCLOG("Feeding the animal.");
    // ���ιʳ�߼�
}

std::string Animal::getAnimalName() const {
    return name;
}

int Animal::getHealth() const {
    return health;
}

int Animal::getMood() const {
    return mood;
}

int Animal::getAffection() const {
    return affection;
}

void Animal::placeWaterBowl(const Vec2& position) {
    // ˮ��ͼƬ��·��
    std::string waterBowlImagePath = "../Resources/water_bowl.png";

    // ����ˮ��ͼƬ
    Sprite* waterBowlSprite = Sprite::create(waterBowlImagePath);
    if (!waterBowlSprite) {
        CCLOG("ˮ��ͼƬ����ʧ��");
        return;
    }

    // ����ˮ��ĳ�ʼλ��
    waterBowlSprite->setPosition(position);
    waterBowlSprite->setName("waterBowl"); // ����һ��Ψһ��ʶ����

    // ��ˮ����ӵ���������ĸ��ڵ�
    this->addChild(waterBowlSprite);

    // ����״̬Ϊˮ����ˮ
    setHasWaterInBowl(true);
}

bool Animal::checkWaterBowlImage(Node* node) {
    // ˮ��ͼƬ��·�����ʶ��
    std::string waterBowlImagePath = "../Resources/water_bowl.png";

    // ���ڵ��Ƿ�����Լ��Ƿ��� Sprite
    Sprite* spriteNode = dynamic_cast<Sprite*>(node);
    if (!spriteNode) {
        setHasWaterInBowl(false);
        return false;
    }

    // ��ȡ Sprite �������ļ�·������������·�����ã�
    Texture2D* texture = spriteNode->getTexture();
    if (!texture) {
        setHasWaterInBowl(false);
        return false;
    }

    // �Ƚ�ͼƬ·���Ƿ�Ϊˮ��ͼƬ
    std::string currentImagePath = texture->getPath(); // ��������֧�ֻ�ȡ·��
    if (currentImagePath == waterBowlImagePath) {
        setHasWaterInBowl(true);
        return true;
    }

    setHasWaterInBowl(false);
    return false;
}

Item::ItemType Animal::produceProduct() {
    switch (getAnimalType()) {
    case AnimalType::CHICKEN:
        return Item::ItemType:: EGG;
    case AnimalType::COW:
        return Item::ItemType::MILK;
    case AnimalType::CAT:
        return Item::ItemType::FISH;
    case AnimalType::DOG:
        return Item::ItemType::BONE;
    case AnimalType::PIG:
        return Item::ItemType::FAT;
    case AnimalType::SHEEP:
        return Item::ItemType::WOOL;
    default:
        return Item::ItemType::NONE;
    }
}

void Animal::openAnimalMenu() {
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
        CC_CALLBACK_1(Animal::closeAnimalMenu, this));
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

    Animal* chicken = gameScene->getChicken();
    Animal* cat = gameScene->getCat();
    Animal* dog = gameScene->getDog();
    Animal* pig = gameScene->getPig();
    Animal* cow = gameScene->getCow();
    Animal* sheep = gameScene->getSheep();

    // ��Ӿ�����ֵ����ʾ
    addAnimalInfoH(chicken, Vec2(visibleSize.width / 4 - 132, visibleSize.height / 4 + 120));
    addAnimalInfoM(chicken, Vec2(visibleSize.width / 4 - 133, visibleSize.height / 4 + 100));
    addAnimalInfoA(chicken, Vec2(visibleSize.width / 4 - 131, visibleSize.height / 4 + 80));

    addAnimalInfoH(cat, Vec2(visibleSize.width / 4 - 132, visibleSize.height / 4 + 20));
    addAnimalInfoM(cat, Vec2(visibleSize.width / 4 - 133, visibleSize.height / 4 + 0));
    addAnimalInfoA(cat, Vec2(visibleSize.width / 4 - 131, visibleSize.height / 4 - 20));

    addAnimalInfoH(sheep, Vec2(visibleSize.width / 4 - 132, visibleSize.height / 4 - 80));
    addAnimalInfoM(sheep, Vec2(visibleSize.width / 4 - 133, visibleSize.height / 4 - 100));
    addAnimalInfoA(sheep, Vec2(visibleSize.width / 4 - 131, visibleSize.height / 4 - 120));

    addAnimalInfoH(pig, Vec2(visibleSize.width / 4 + 189, visibleSize.height / 4 + 120));
    addAnimalInfoM(pig, Vec2(visibleSize.width / 4 + 188, visibleSize.height / 4 + 100));
    addAnimalInfoA(pig, Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 + 80));

    addAnimalInfoH(dog, Vec2(visibleSize.width / 4 + 189, visibleSize.height / 4 + 20));
    addAnimalInfoM(dog, Vec2(visibleSize.width / 4 + 188, visibleSize.height / 4));
    addAnimalInfoA(dog, Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 - 20));

    addAnimalInfoH(cow, Vec2(visibleSize.width / 4 + 189, visibleSize.height / 4 - 80));
    addAnimalInfoM(cow, Vec2(visibleSize.width / 4 + 188, visibleSize.height / 4 - 100));
    addAnimalInfoA(cow, Vec2(visibleSize.width / 4 + 190, visibleSize.height / 4 - 120));

    // ���˵�����ӵ�����
    auto scene = Director::getInstance()->getRunningScene();
    scene->addChild(menuLayer, 10); // ����һ�����ʵĲ㼶��ȷ���˵�����������
}

void Animal::getGameScene(GameScene* scene) {
    gameScene = scene;
}


void Animal::addAnimalInfoH(Animal* animal, const Vec2& position) {
    if (animal) {
        auto labelHealth = Label::createWithTTF("Health: " + std::to_string(animal->getHealth()) + " / 100", "fonts/Marker Felt.ttf", 20);
        labelHealth->setPosition(position);
        menuLayer->addChild(labelHealth);
    }
}

void Animal::addAnimalInfoM(Animal* animal, const Vec2& position) {
    if (animal) {
        auto labelHealth = Label::createWithTTF("Mood : " + std::to_string(animal->getMood()) + " / 300", "fonts/Marker Felt.ttf", 20);
        labelHealth->setPosition(position);
        menuLayer->addChild(labelHealth);
    }
}

void Animal::addAnimalInfoA(Animal* animal, const Vec2& position) {
    if (animal) {
        auto labelHealth = Label::createWithTTF("Affection : " + std::to_string(animal->getAffection()) + "/1000", "fonts/Marker Felt.ttf", 20);
        labelHealth->setPosition(position);
        menuLayer->addChild(labelHealth);
    }
}

void Animal::closeAnimalMenu(cocos2d::Ref* sender) {
    // �رղ˵����߼�
    if (menuLayer) {
        menuLayer->removeFromParentAndCleanup(true); // �Ƴ��˵���
        menuLayer = nullptr; // �������
    }
}
