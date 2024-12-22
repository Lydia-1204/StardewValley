/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.cpp
 * File Function: ʵ��Crop�࣬���ũ������ֲ����
 * Author:        ���庬 2351591
 * Update Date:   2024/12/22
 ****************************************************************/

#include "Crop.h"
USING_NS_CC;
#include "GameScene.h" // ���� GameScene ����������
#include "map.h"
#include"ToolManager.h"
#include"ItemManager.h"

Crop* Crop::create(GameScene* scene, const std::string& nickname) {
    Crop* crop = new (std::nothrow) Crop();
    crop->getGameScene(scene);  // ����GameScene������
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

    // ��ȡ���ӽ׶ε�ͼƬ·��
    std::string seedTexturePath = stageTextures[static_cast<int>(CropState::Seed)];

    // ʹ�����ӽ׶ε�ͼƬ·����ʼ������
    myCrop = Sprite::create(seedTexturePath);
    if (!myCrop) {
        throw("Failed to create seed sprite!");
    }
    else {
        // �����Ӿ�����ӵ������У���������Խ����洢�������Ա�֮��ʹ��
        this->addChild(myCrop);
    }


    // ��ʼ���¼�������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = CC_CALLBACK_1(Crop::onMouseDown, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);

    // ��ʱ�������״̬
    schedule(CC_SCHEDULE_SELECTOR(Crop::update), 1.0f / 60.0f);

    return true;
}

void Crop::onMouseDown(EventMouse* event) {
   
    // ��ȡ���������Ļ����
    EventMouse* mouseEvent = static_cast<EventMouse*>(event);
    Vec2 mouseLocation = mouseEvent->getLocation();

    // ��ӵ��������+���������Աߵ��ж��߼�
    auto playerPos = Player::getInstance(1, "guest")->getPosition();
    auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����

    CCLOG("mouseDown");
    CCLOG("locationInWorld Mouse Position: %f, %f", locationInWorld.x, locationInWorld.y);
    CCLOG("players locationInWorld: %f, %f", playerPos.x, playerPos.y);
    CCLOG("cropss locationInWorld: %f, %f", this->getPosition().x, this->getPosition().y);
    // ��ҿ������ﲢ���ʱ������
    if (locationInWorld.distance(this->getPosition()) < 30 && playerPos.distance(this->getPosition()) < 30) {
        
        // ���������
        if (mouseEvent->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
            // ��������ѳ��죬�������ʱ�Ƴ���
            if (state == CropState::Mature) {
                if (gameScene) {
                    gameScene->removeCrop();  // ���� GameScene �� removeCrop ����
                    ItemManager::getInstance(1,"")->addItem(Item::ItemType::FRUIT);
                }

                // ������뱳���Ĵ���
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
            // ����Ҽ����

           ToolManager* toolManager = ToolManager::getInstance(1, "guest");
            if (toolManager && toolManager->selectedToolIndex >= 0 &&
                toolManager->selectedToolIndex < toolManager->tools.size() && toolManager->tools[toolManager->selectedToolIndex]->getType() == Tool::ToolType::FERTILIZER) {
                fertilize();
            }
        }
    }
}


Crop::Crop() : state(CropState::Seed), age(0), watered(false), lastWateredTime(0), timeElapsed(0) {
    // ��ʼ����ͬ�����׶ε�ͼƬ·��
    stageTextures = {
        "../Resources/seed.png",     // ���ӽ׶�
        "../Resources/stage1.png",   // ��һ�׶�
        "../Resources/stage2.png",   // �ڶ��׶�
        "../Resources/stage3.png",   // �����׶�
        "../Resources/stage4.png",   // ���Ľ׶�
        "../Resources/stage5.png",   // ����׶�
        "../Resources/mature.png"    // ����׶�
    };
}

int times = 0;
void Crop::update(float dt) {
    // ÿ֡���£�����ʱ���������״̬�ͽ�ˮ״̬
    timeElapsed += dt;
    if (MapManager::getInstance()->currentMapLabel == 1 && this) {

        this->setVisible(true);
    }
    else {
        this->setVisible(false);
    }
    if (timeElapsed >= 1.0f) {
        age += timeElapsed;  // ���Ӿ�����ʱ�䣨����Ϊ��λ��
        lastWateredTime += timeElapsed;  // �������ϴν�ˮ������ʱ��
        CCLOG("Crop time update!");
        CCLOG("Age: %d lastWateredTime: %d", age, lastWateredTime);
        timeElapsed = 0.0f;
        if (lastWateredTime >= 150) {
            times++;
        }
    }

    // ����Ƿ�ﵽ�������ڣ�50�룩
    if (age >= 50) {  // 50�������������
        CCLOG("Crop time !!!!");
        grow();  // ������������
        age = 0;  // ���������ʱ��
    }

    // ����Ƿ��Ѿ�����30��δ��ˮ
    //CCLOG("%d", times);
    if (lastWateredTime >= 150) {  // 150��δ��ˮ
        CCLOG("Crop will die!");
        state = CropState::Dead;
        changeTexture("../Resources/dead.png");  // ��ʾ�����׶ε�ͼƬ
        CCLOG("Crop is dead!");

        // ����һ���µĽڵ�����ʾ"dead"ͼƬ
        deadSprite = Sprite::create("../Resources/dead.png");
        if (!deadSprite) {
            throw("Failed to create dead sprite!");
        }
        deadSprite->setPosition(Vec2(10, 10));  // ����"dead"ͼƬ��λ����������ͬ
        this->addChild(deadSprite);  // ��"dead"ͼƬ���ΪCrop���ӽڵ�

        // �Ƴ�������߼�
        if (times > 500) {
            // �����ӳ������Ƴ�����
            CCLOG("Will remove crop!");
            if (deadSprite) {
                CCLOG("Will remove label!");
                deadSprite->removeFromParentAndCleanup(true);
                deadSprite = nullptr;  // ��ֹ���������ٴη�����ɾ���Ķ���
            }
            if (gameScene) {
                gameScene->removeCrop();  // ���� GameScene �� removeCrop ����
            }
        }
    }

    
}

void Crop::grow() {
    if (state != CropState::Mature && state != CropState::Dead) {  // ���������δ���������
        CCLOG("Into grow!");
        if (watered) {
            newAge += age;
            // ���ý�ˮ״̬
            watered = false;
            // ���������׶Σ�ÿ10������һ���׶�
            int stage = (newAge / 10) % stageTextures.size();
            CCLOG("Stage: %d", stage);
            // ��������ͼƬ
            changeTexture(stageTextures[stage]);
            // �������ˮ��ʱ��
            lastWateredTime = 0; // ���ý�ˮ��ʱ��
            if (stage == 6) {
                state = CropState::Mature;
            }
            else {
                state = CropState::Growing;
            }
        }
    }

    // ���������죬��������
    if (state == CropState::Mature) {
        // ������������ӳ�������Ϊ��֪ͨ���
        std::string imagePath = "../Resources/mature_.png";
        Sprite* matureSprite = Sprite::create(imagePath);

        // ���ͼƬ�Ƿ���سɹ�
        if (matureSprite) {
            // ����ͼƬ��λ�ã������Զ��ﾫ�������Ϊλ�òο�
            matureSprite->setPosition(myCrop->getPosition());

            // ��ͼƬ��ӵ��������ڵĽڵ�
            this->addChild(matureSprite, 1, "matureSprite"); // ���ñ�ǩ"happySprite"

            // ����һ����ʱ���Ƴ�ͼƬ����ֹ��һֱ��ʾ
            this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 2.0f); // 2����Ƴ�ͼƬ
        }
    }
}

void Crop::removeSprite(float dt) {
    // ���ݱ�ǩ��ȡͼƬ���Ƴ���
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
    watered = true;  // ���Ϊ�ѽ�ˮ
    std::string imagePath = "../Resources/watered.png";
    Sprite* happySprite = Sprite::create(imagePath);
    lastWateredTime = 0;
    // ���ͼƬ�Ƿ���سɹ�
    if (happySprite) {
        // ����ͼƬ��λ�ã������Զ��ﾫ�������Ϊλ�òο�
        happySprite->setPosition(myCrop->getPosition());

        // ��ͼƬ��ӵ��������ڵĽڵ�
        this->addChild(happySprite, 1, "watered"); // ���ñ�ǩ"happySprite"

        // ����һ����ʱ���Ƴ�ͼƬ����ֹ��һֱ��ʾ
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 1.0f); // 2����Ƴ�ͼƬ
    }
}

void Crop::fertilize() {
    std::string imagePath = "../Resources/fertilize.png";
    Sprite* happySprite = Sprite::create(imagePath);

    // ���ͼƬ�Ƿ���سɹ�
    if (happySprite) {
        // ����ͼƬ��λ�ã������Զ��ﾫ�������Ϊλ�òο�
        happySprite->setPosition(myCrop->getPosition());

        // ��ͼƬ��ӵ��������ڵĽڵ�
        this->addChild(happySprite, 1, "fertilize"); // ���ñ�ǩ"happySprite"

        // ����һ����ʱ���Ƴ�ͼƬ����ֹ��һֱ��ʾ
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 1.0f); // 2����Ƴ�ͼƬ
    }
}

void Crop::changeTexture(const std::string& texturePath) {
    CCLOG("State: %d", state);
    if (state != CropState::Dead) {
        CCLOG("Loading texture from path: %s", texturePath.c_str());
        // ���ز�������ʾ��ͼƬ
        auto texture = cocos2d::Director::getInstance()->getTextureCache()->addImage(texturePath);
        if (texture) {
            myCrop->setTexture(texture);
            CCLOG("Texture loaded successfully: %s", texturePath.c_str());
        }
        else {
            CCLOG("Failed to load texture: %s", texturePath.c_str());
        }

        // ���ûص�����֪ͨ GameScene
        if (textureChangedCallback) {
            textureChangedCallback(texturePath);
        }
    }
}

void Crop::setTextureChangedCallback(const TextureChangedCallback& callback) {
    textureChangedCallback = callback;
}