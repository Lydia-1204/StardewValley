/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.cpp
 * File Function: 实现Crop类，完成农作物种植功能
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/22
 ****************************************************************/

#include "Crop.h"
USING_NS_CC;
#include "GameScene.h" // 包含 GameScene 的完整定义
#include "map.h"
#include"ToolManager.h"
#include"ItemManager.h"

Crop* Crop::create(GameScene* scene, const std::string& nickname) {
    Crop* crop = new (std::nothrow) Crop();
    crop->getGameScene(scene);  // 设置GameScene的引用
    if (crop && crop->init(nickname)) {
        crop->autorelease();
        return crop;
    }
    delete crop;
    return nullptr;
}

void Crop::getGameScene(GameScene* scene) {
    gameScene = scene;
}

bool Crop::init(const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

    // 获取种子阶段的图片路径
    std::string seedTexturePath = stageTextures[static_cast<int>(CropState::Seed)];

    // 使用种子阶段的图片路径初始化精灵
    myCrop = Sprite::create(seedTexturePath);
    if (!myCrop) {
        throw("Failed to create seed sprite!");
    }
    else {
        // 将种子精灵添加到场景中，或者你可以将它存储在类中以便之后使用
        this->addChild(myCrop);
    }


    // 初始化事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Crop::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // 定时更新玩家状态
    schedule(CC_SCHEDULE_SELECTOR(Crop::update), 1.0f / 60.0f);

    return true;
}

void Crop::onMouseDown(EventMouse* event) {
   
    // 获取鼠标点击的屏幕坐标
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    Vec2 mouseLocation = mouseEvent->getLocation();

    // 添加点击到作物+人在作物旁边的判断逻辑
    auto playerPos = Player::getInstance(1, "guest")->getPosition();
    auto locationInWorld = event->getLocationInView();  // 获取屏幕视图中的坐标

    CCLOG("mouseDown");
    CCLOG("locationInWorld Mouse Position: %f, %f", locationInWorld.x, locationInWorld.y);
    CCLOG("players locationInWorld: %f, %f", playerPos.x, playerPos.y);
    CCLOG("cropss locationInWorld: %f, %f", this->getPosition().x, this->getPosition().y);
    // 玩家靠近动物并点击时，交互
    if (locationInWorld.distance(this->getPosition()) < 30 && playerPos.distance(this->getPosition()) < 30) {
        
        // 检查点击类型
        if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            // 如果作物已成熟，点击作物时移除它
            if (state == CropState::Mature) {
                if (gameScene) {
                    gameScene->removeCrop();  // 调用 GameScene 的 removeCrop 方法
                    ItemManager::getInstance(1,"")->addItem(Item::ItemType::FRUIT);
                }

                // 添加收入背包的代码
            }

            else {
                ToolManager* toolManager = ToolManager::getInstance(1, "guest");
                if (toolManager && toolManager->selectedToolIndex >= 0 &&
                    toolManager->selectedToolIndex < toolManager->tools.size() 
                    && (toolManager->tools[toolManager->selectedToolIndex]->getType() == Tool::ToolType::WATERING_CAN|| toolManager->tools[toolManager->selectedToolIndex]->getType() == Tool::ToolType::WATERING_CANPLUS)) {
                    water();
                }
            }
        }
        else if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
            // 鼠标右键点击

           ToolManager* toolManager = ToolManager::getInstance(1, "guest");
            if (toolManager && toolManager->selectedToolIndex >= 0 &&
                toolManager->selectedToolIndex < toolManager->tools.size() && toolManager->tools[toolManager->selectedToolIndex]->getType() == Tool::ToolType::FERTILIZER) {
                fertilize();
            }
        }
    }
}


Crop::Crop() : state(CropState::Seed), age(0), watered(false), lastWateredTime(0), timeElapsed(0) {
    // 初始化不同生长阶段的图片路径
    stageTextures = {
        "../Resources/seed.png",     // 种子阶段
        "../Resources/stage1.png",   // 第一阶段
        "../Resources/stage2.png",   // 第二阶段
        "../Resources/stage3.png",   // 第三阶段
        "../Resources/stage4.png",   // 第四阶段
        "../Resources/stage5.png",   // 第五阶段
        "../Resources/mature.png"    // 成熟阶段
    };
}

int times = 0;
void Crop::update(float dt) {
    // 每帧更新，根据时间更新生长状态和浇水状态
    timeElapsed += dt;
    if (MapManager::getInstance()->currentMapLabel == 1 && this) {

        this->setVisible(true);
    }
    else {
        this->setVisible(false);
    }
    if (timeElapsed >= 1.0f) {
        age += timeElapsed;  // 增加经过的时间（以秒为单位）
        lastWateredTime += timeElapsed;  // 增加自上次浇水以来的时间
        CCLOG("Crop time update!");
        CCLOG("Age: %d lastWateredTime: %d", age, lastWateredTime);
        timeElapsed = 0.0f;
        if (lastWateredTime >= 150) {
            times++;
        }
    }

    // 检查是否达到生长周期（50秒）
    if (age >= 50) {  // 50秒等于生长周期
        CCLOG("Crop time !!!!");
        grow();  // 调用生长函数
        age = 0;  // 重置年龄计时器
    }

    // 检查是否已经超过30秒未浇水
    //CCLOG("%d", times);
    if (lastWateredTime >= 150) {  // 150秒未浇水
        CCLOG("Crop will die!");
        state = CropState::Dead;
        changeTexture("../Resources/dead.png");  // 显示死亡阶段的图片
        CCLOG("Crop is dead!");

        // 创建一个新的节点来显示"dead"图片
        deadSprite = Sprite::create("../Resources/dead.png");
        if (!deadSprite) {
            throw("Failed to create dead sprite!");
        }
        deadSprite->setPosition(Vec2(10, 10));  // 设置"dead"图片的位置与作物相同
        this->addChild(deadSprite);  // 将"dead"图片添加为Crop的子节点

        // 移除作物的逻辑
        if (times > 500) {
            // 立即从场景中移除作物
            CCLOG("Will remove crop!");
            if (deadSprite) {
                CCLOG("Will remove label!");
                deadSprite->removeFromParentAndCleanup(true);
                deadSprite = nullptr;  // 防止后续代码再次访问已删除的对象
            }
            if (gameScene) {
                gameScene->removeCrop();  // 调用 GameScene 的 removeCrop 方法
            }
        }
    }

    
}

void Crop::grow() {
    if (state != CropState::Mature && state != CropState::Dead) {  // 如果作物尚未成熟或死亡
        CCLOG("Into grow!");
        if (watered) {
            newAge += age;
            // 重置浇水状态
            watered = false;
            // 计算生长阶段，每10秒增加一个阶段
            int stage = (newAge / 10) % stageTextures.size();
            CCLOG("Stage: %d", stage);
            // 更新作物图片
            changeTexture(stageTextures[stage]);
            // 更新最后浇水的时间
            lastWateredTime = 0; // 重置浇水计时器
            if (stage == 6) {
                state = CropState::Mature;
            }
            else {
                state = CropState::Growing;
            }
        }
    }

    // 如果作物成熟，不再生长
    if (state == CropState::Mature) {
        // 可以在这里添加成熟后的行为，通知玩家
        std::string imagePath = "../Resources/mature_.png";
        Sprite* matureSprite = Sprite::create(imagePath);

        // 检查图片是否加载成功
        if (matureSprite) {
            // 设置图片的位置，这里以动物精灵的中心为位置参考
            matureSprite->setPosition(myCrop->getPosition());

            // 将图片添加到动物所在的节点
            this->addChild(matureSprite, 1, "matureSprite"); // 设置标签"happySprite"

            // 设置一个延时来移除图片，防止它一直显示
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 2.0f); // 2秒后移除图片
        }
    }
}

void Crop::removeSprite(float dt) {
    // 根据标签获取图片并移除它
    Node* matureSprite = this->getChildByName("matureSprite");
    if (matureSprite) {
        this->removeChild(matureSprite, 1);
    }
    Node* matureSprite1 = this->getChildByName("fertilize");
    if (matureSprite1) {
        this->removeChild(matureSprite1, 1);
    }
    Node* matureSprite2 = this->getChildByName("watered");
    if (matureSprite2) {
        this->removeChild(matureSprite2, 1);
    }

}

void Crop::water() {
    watered = true;  // 标记为已浇水
    std::string imagePath = "../Resources/watered.png";
    Sprite* happySprite = Sprite::create(imagePath);
    lastWateredTime = 0;
    // 检查图片是否加载成功
    if (happySprite) {
        // 设置图片的位置，这里以动物精灵的中心为位置参考
        happySprite->setPosition(myCrop->getPosition());

        // 将图片添加到动物所在的节点
        this->addChild(happySprite, 1, "watered"); // 设置标签"happySprite"

        // 设置一个延时来移除图片，防止它一直显示
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 1.0f); // 2秒后移除图片
    }
}

void Crop::fertilize() {
    std::string imagePath = "../Resources/fertilize.png";
    Sprite* happySprite = Sprite::create(imagePath);

    // 检查图片是否加载成功
    if (happySprite) {
        // 设置图片的位置，这里以动物精灵的中心为位置参考
        happySprite->setPosition(myCrop->getPosition());

        // 将图片添加到动物所在的节点
        this->addChild(happySprite, 1, "fertilize"); // 设置标签"happySprite"

        // 设置一个延时来移除图片，防止它一直显示
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 1.0f); // 2秒后移除图片
    }
}

void Crop::changeTexture(const std::string& texturePath) {
    CCLOG("State: %d", state);
    if (state != CropState::Dead) {
        CCLOG("Loading texture from path: %s", texturePath.c_str());
        // 加载并更换显示的图片
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(texturePath);
        if (texture) {
            myCrop->setTexture(texture);
            CCLOG("Texture loaded successfully: %s", texturePath.c_str());
        }
        else {
            CCLOG("Failed to load texture: %s", texturePath.c_str());
        }

        // 调用回调函数通知 GameScene
        if (textureChangedCallback) {
            textureChangedCallback(texturePath);
        }
    }
}

void Crop::setTextureChangedCallback(const TextureChangedCallback& callback) {
    textureChangedCallback = callback;
}