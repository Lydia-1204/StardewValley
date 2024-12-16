/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Animal.cpp
 * File Function: 实现Animal类，完成动物养殖功能
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/
 ****************************************************************/

#include "Animal.h"
#include "AnimalManager.h"
USING_NS_CC;


Animal* Animal::create(int targetAnimal, const std::string& nickname) {
    Animal* animal = new (std::nothrow) Animal();
    if (animal && animal->init(targetAnimal, nickname)) {
        animal->autorelease();
        return animal;
    }
    delete animal;
    return nullptr;
}

bool Animal::init(int targetAnimal, const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

    std::string texturePath;
    if (targetAnimal == 1) {
        texturePath = "../Resources/Chicken.png";
        _picturename = "Chicken.png";
    }
    else if (targetAnimal == 2) {
        _picturename = "Cow.png";
        texturePath = "../Resources/Cow.png";
    }
    else if (targetAnimal == 3) {
        _picturename = "Cat.png";
        texturePath = "../Resources/Cat.png";
    }
    else if (targetAnimal == 4) {
        _picturename = "Dog.png";
        texturePath = "../Resources/Dog.png";
    }
    else if (targetAnimal == 5) {
        _picturename = "Pig.png";
        texturePath = "../Resources/Pig.png";
    }
    else if (targetAnimal == 6) {
        _picturename = "Sheep.png";
        texturePath = "../Resources/Sheep.png";
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

    Vec2 waterBowlPosition = Vec2(25, 70); // 相对对应动物的初始坐标的位置
    placeWaterBowl(waterBowlPosition);

    // 设置鼠标点击事件监听器
    auto listener = EventListenerMouse::create();
    listener->onMouseDown = std::bind(&Animal::onMouseDown, this, std::placeholders::_1, nickname);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    // 定时更新动物状态
    schedule(CC_SCHEDULE_SELECTOR(Animal::update), 1.0f / 60.0f);

    return true;
}

Vec2 Animal::getCurrentDirection() {
    // 检查 an_sprite 是否为空指针
    if (an_sprite == nullptr) {
        throw("Animal direction error!!");
    }
    // 获取当前位置
    Vec2 currentPosition = an_sprite->getPosition();

    // 如果 lastPosition 未被初始化，或者与当前位置相同，返回 Vec2(0, 0)
    if (lastPosition == currentPosition) {
        return Vec2(0, 0);
    }

    // 计算并返回当前方向
    Vec2 direction = currentPosition - lastPosition;
    lastPosition = currentPosition; // 更新 lastPosition 为当前位置
    return direction;
}

// 在 update 方法中调用 updateDirection
void Animal::update(float dt) {
    updateDirection(dt, _picturename);
    // 其他更新逻辑...
}

void Animal::updateDirection(float dt, const std::string& picturename) {
    Vec2 direction = getCurrentDirection();

    // 加载纹理图集（只加载一次）
    bool textureLoaded = false;
    Texture2D* texture;
    if (!textureLoaded) {
        // 加载大图纹理
        texture = Director::getInstance()->getTextureCache()->addImage(picturename.c_str());
        if (texture) {
            textureLoaded = true;  // 标记纹理已加载
        }
        else {
            CCLOG("Error: Failed to load sprite sheet texture.");
            return;
        }
    }

    // 确保当方向发生变化时才更新精灵帧
    if (direction.x > 0) { // 向右移动
        // 定义右方向的精灵区域
        Rect rectForRight;
        if (picturename == "Chicken.png") {
            rectForRight = Rect(16, 16, 16, 16);  // 假设右移的精灵区域是从大图的 (3*18, 3*19) 开始，大小为 18x19
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForRight = Rect(0, 32, 32, 32);  // 假设右移的精灵区域是从大图的 (3*18, 3*19) 开始，大小为 18x19
        }
        else {
            rectForRight = Rect(0, 32, 32, 32);  // 假设右移的精灵区域是从大图的 (3*18, 3*19) 开始，大小为 18x19
        }

        SpriteFrame* spriteFrameRight = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForRight);
        an_sprite->setSpriteFrame(spriteFrameRight);
        if (picturename == "Cow.png" || picturename == "Pig.png" || picturename == "Sheep.png") {
            an_sprite->setFlippedX(false); 
        }

    }
    else if (direction.x < 0) { // 向左移动
        // 定义左方向的精灵区域
        Rect rectForLeft;
        if (picturename == "Chicken.png") {
            rectForLeft = Rect(16, 0, 16, 16);  // 假设左移的精灵区域
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForLeft = Rect(0, 32 * 3, 32, 32);  // 假设左移的精灵区域
        }
        else {
            // 需要左右翻转
            rectForLeft = Rect(0, 32, 32, 32);

        }
        SpriteFrame* spriteFrameLeft = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForLeft);
        an_sprite->setSpriteFrame(spriteFrameLeft);
        if (picturename == "Cow.png" || picturename == "Pig.png" || picturename == "Sheep.png") {
            an_sprite->setFlippedX(true); // 向左时翻转
        }
    }
    else if (direction.y > 0) { // 向上移动
        // 定义上方向的精灵区域
        Rect rectForUp;
        if (picturename == "Chicken.png") {
            rectForUp = Rect(16, 16 * 2, 16, 16);  // 假设上移的精灵区域
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForUp = Rect(0, 32 * 2, 32, 32);  // 假设上移的精灵区域
        }
        else {
            rectForUp = Rect(0, 32 * 2, 32, 32);  // 假设右移的精灵区域是从大图的 (3*18, 3*19) 开始，大小为 18x19
        }
        SpriteFrame* spriteFrameUp = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForUp);
        an_sprite->setSpriteFrame(spriteFrameUp);
    }
    else if (direction.y < 0) { // 向下移动
        // 定义下方向的精灵区域
        Rect rectForDown;
        if (picturename == "Chicken.png") {
            rectForDown = Rect(16, 16 * 6, 16, 16);  // 假设下移的精灵区域
        }
        else if (picturename == "Cat.png" || picturename == "Dog.png") {
            rectForDown = Rect(0, 0, 32, 32);  // 假设下移的精灵区域
        }
        else {
            rectForDown = Rect(0, 0, 32, 32);  // 假设右移的精灵区域是从大图的 (3*18, 3*19) 开始，大小为 18x19
        }
        SpriteFrame* spriteFrameDown = SpriteFrame::createWithTexture(Director::getInstance()->getTextureCache()->getTextureForKey(picturename.c_str()), rectForDown);
        an_sprite->setSpriteFrame(spriteFrameDown);
    }

    CCLOG("Direction: x = %f, y = %f", direction.x, direction.y);
}

void Animal::moveAlongPath(const std::vector<Vec2>& path, const std::string& picturename) {
    // 使用 ActionInterval 作为 action 变量的类型
    ActionInterval* action = nullptr;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        // MoveTo::create 返回的是 FiniteTimeAction 类型，需要转换为 ActionInterval 类型
        auto moveAction = static_cast<ActionInterval*>(MoveTo::create(1.0f, path[i + 1]));

        // 使用 Sequence::createWithTwoActions 创建 Sequence，需要两个 FiniteTimeAction 参数
        action = (i == 0) ? moveAction : Sequence::createWithTwoActions(static_cast<FiniteTimeAction*>(action), static_cast<FiniteTimeAction*>(moveAction));
    }

    // RepeatForever::create 需要一个 ActionInterval 参数
    auto repeatAction = RepeatForever::create(action);
    an_sprite->runAction(repeatAction);

    // 需要确保运动过程中不停地更新方向（即精灵帧）
    schedule([this, picturename](float dt) {
        // 在运动时更新方向，并传递图片名称
        updateDirection(dt, picturename);
        }, 1.0f / 60.0f, "update_direction_key");
}

void Animal::initialmove(const std::string& picturename) {
    Vec2 startPosition = an_sprite->getPosition(); // 记录初始位置
    lastPosition = startPosition; // 设置lastPosition为初始位置

    // 定义路径点
    std::vector<Vec2> path = {
        Vec2(startPosition.x, startPosition.y),
        Vec2(startPosition.x + 50, startPosition.y),
        Vec2(startPosition.x + 50, startPosition.y + 50),
        Vec2(startPosition.x, startPosition.y + 50),
        Vec2(startPosition.x, startPosition.y)
    };

    // 沿着路径移动
    moveAlongPath(path, picturename);

}

// 实现获取动物类型的函数
Animal::AnimalType Animal::getAnimalType() const {
    if (name == "chicken") return AnimalType::CHICKEN;
    else if (name == "cow") return AnimalType::COW;
    else if (name == "cat") return AnimalType::CAT;
    else if (name == "dog") return AnimalType::DOG;
    else if (name == "pig") return AnimalType::PIG;
    else if (name == "sheep") return AnimalType::SHEEP;
    return AnimalType::CHICKEN; // 默认类型
}

void Animal::onMouseDown(Event* event, std::string animalName) {
    // 获取鼠标点击的屏幕坐标
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    Vec2 mouseLocation = mouseEvent->getLocation();

    // 添加点击到动物的判断逻辑
    

    // 检查点击类型
    if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        // 获取当前时间点
        auto now = std::chrono::steady_clock::now();

        // 添加判断是否携带食物的代码

        // 检查是否是双击
        if (wasDoubleClick) {
            // 重置双击标记
            wasDoubleClick = false;
            // 执行喂食操作
            feed();  // 假设feed函数不需要参数，或者传递适当的参数
            CCLOG("Double left click on Animal, animal fed.");
        }
        else {
            // 标记为双击可能
            wasDoubleClick = true;
            // 检查两次点击的时间间隔，确定是否为双击
            if (std::chrono::duration_cast<std::chrono::milliseconds>(now - lastClickTime).count() < 500) {
                // 时间间隔小于500毫秒，视为双击
                wasDoubleClick = true;
            }
            else {
                // 时间间隔大于500毫秒，不是双击，重置标记
                wasDoubleClick = false;
                // 检查健康、心情和好感度是否都达到了满值
                if (health == 100 && mood == 300 && affection == 1000) {
                    // 根据动物类型产生相应的产物
                    std::string product = produceProduct();
                    CCLOG("Produced product: %s", product.c_str());
                }
                else {
                    // 如果没有达到满值，更新好感度和心情
                    updateFavorability();
                    updateMood();
                    CCLOG("Left click on Animal, favorability and mood updated.");
                }
            }
            // 更新上次点击时间
            lastClickTime = now;
        }
    }
    else if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        // 鼠标右键点击
        manager->openAnimalMenu();
        CCLOG("Right click on Animal, menu opened.");
    }
}

void Animal::updateFavorability() {
    // 增加或更新动物的好感度逻辑
    affection += 15; // 假设每次点击增加15点好感度
    if (affection > 1000) affection = 1000;  // 最大好感度为1000
}

void Animal::updateMood() {
    // 更新动物的心情参数逻辑
    mood += 35; // 假设点击后动物心情变为高兴
    if (mood > 300 || mood == 300)
    {
        mood = 300;
        // 当心情值达到最大时显示一张图片
        std::string imagePath = "../Resources/happy_animal.png";
        Sprite* happySprite = Sprite::create(imagePath);

        // 检查图片是否加载成功
        if (happySprite) {
            // 设置图片的位置，这里以动物精灵的中心为位置参考
            happySprite->setPosition(an_sprite->getPosition());

            // 将图片添加到动物所在的节点
            this->addChild(happySprite, 1, "happySprite"); // 设置标签"happySprite"

            // 设置一个延时来移除图片，防止它一直显示
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(Animal::removeHappySprite), 2.0f); // 2秒后移除图片
        }
    }
}

void Animal::removeHappySprite(float dt) {
    // 根据标签获取图片并移除它
    Node* happySprite = this->getChildByName("happySprite");
    if (happySprite) {
        this->removeChild(happySprite);
    }
}

void Animal::feed() {
    health += 15;
    if (health > 100) health = 100;  
    CCLOG("Feeding the animal.");
    // 添加喂食逻辑
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
    // 水碗图片的路径
    std::string waterBowlImagePath = "../Resources/water_bowl.png";

    // 创建水碗图片
    Sprite* waterBowlSprite = Sprite::create(waterBowlImagePath);
    if (!waterBowlSprite) {
        CCLOG("水碗图片加载失败");
        return;
    }

    // 设置水碗的初始位置
    waterBowlSprite->setPosition(position);
    waterBowlSprite->setName("waterBowl"); // 设置一个唯一标识名称

    // 将水碗添加到场景或动物的父节点
    this->addChild(waterBowlSprite);

    // 更新状态为水碗有水
    setHasWaterInBowl(true);
}

bool Animal::checkWaterBowlImage(Node* node) {
    // 水碗图片的路径或标识符
    std::string waterBowlImagePath = "../Resources/water_bowl.png";

    // 检查节点是否存在以及是否是 Sprite
    Sprite* spriteNode = dynamic_cast<Sprite*>(node);
    if (!spriteNode) {
        setHasWaterInBowl(false);
        return false;
    }

    // 获取 Sprite 的纹理文件路径（假设纹理路径可用）
    Texture2D* texture = spriteNode->getTexture();
    if (!texture) {
        setHasWaterInBowl(false);
        return false;
    }

    // 比较图片路径是否为水碗图片
    std::string currentImagePath = texture->getPath(); // 假设引擎支持获取路径
    if (currentImagePath == waterBowlImagePath) {
        setHasWaterInBowl(true);
        return true;
    }

    setHasWaterInBowl(false);
    return false;
}


std::string Animal::produceProduct() {
    switch (getAnimalType()) {
        case AnimalType::CHICKEN:
            return "Egg";
        case AnimalType::COW:
            return "Milk";
        case AnimalType::CAT:
            return "Fish";
        case AnimalType::DOG:
            return "Bone";
        case AnimalType::PIG:
            return "Fat";
        case AnimalType::SHEEP:
            return "Wool";
        default:
            return "Unknown";
    }
}
