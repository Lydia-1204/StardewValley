#include "CropState.h"
#include "Farming/Crop.h"
#include "Inventory/ToolManager.h"
#include "Inventory/ItemManager.h"
#include "Scenes/GameScene.h"

// 辅助宏：判断工具类型
bool isWateringCan() {
    ToolManager* tm = ToolManager::getInstance(1, "guest");
    if (!tm || tm->selectedToolIndex < 0 || tm->selectedToolIndex >= tm->tools.size()) return false;
    auto type = tm->tools[tm->selectedToolIndex]->getType();
    return (type == Tool::ToolType::WATERING_CAN || type == Tool::ToolType::WATERING_CANPLUS);
}

bool isFertilizer() {
    ToolManager* tm = ToolManager::getInstance(1, "guest");
    if (!tm || tm->selectedToolIndex < 0 || tm->selectedToolIndex >= tm->tools.size()) return false;
    return tm->tools[tm->selectedToolIndex]->getType() == Tool::ToolType::FERTILIZER;
}

// ================= SeedState (种子) =================

void SeedState::enter(Crop* crop) {
    CCLOG("Enter SeedState");
    crop->changeTexture(crop->stageTextures[0]); // seed.png
}

void SeedState::execute(Crop* crop, float dt) {
    // 检查是否枯死
    if (crop->lastWateredTime >= 150) {
        crop->changeState(new DeadState());
    }
}

void SeedState::onClick(Crop* crop, EventMouse* event) {
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        if (isWateringCan()) {
            crop->water();
        }
    }
    else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        if (isFertilizer()) {
            crop->fertilize();
        }
    }
}

void SeedState::exit(Crop* crop) {}

void SeedState::grow(Crop* crop) {
    if (crop->watered) {
        crop->newAge += crop->age;
        crop->watered = false; // 消耗水分
        // 种子成长后进入生长期
        crop->changeState(new GrowingState());
    }
}

// ================= GrowingState (生长中) =================

void GrowingState::enter(Crop* crop) {
    CCLOG("Enter GrowingState");
    // 根据 newAge 计算当前贴图
    int stage = (crop->newAge / 10) % crop->stageTextures.size();
    // 保护防止越界
    if (stage > 0 && stage < crop->stageTextures.size()) {
        crop->changeTexture(crop->stageTextures[stage]);
    }
}

void GrowingState::execute(Crop* crop, float dt) {
    // 检查是否枯死
    if (crop->lastWateredTime >= 150) {
        crop->changeState(new DeadState());
    }
}

void GrowingState::onClick(Crop* crop, EventMouse* event) {
    // 与种子阶段相同的交互逻辑
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        if (isWateringCan()) {
            crop->water();
        }
    }
    else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
        if (isFertilizer()) {
            crop->fertilize();
        }
    }
}

void GrowingState::exit(Crop* crop) {}

void GrowingState::grow(Crop* crop) {
    if (crop->watered) {
        crop->newAge += crop->age;
        crop->watered = false;

        int stage = (crop->newAge / 10) % crop->stageTextures.size();
        CCLOG("Growing Stage: %d", stage);

        // 更新贴图
        if (stage < crop->stageTextures.size()) {
            crop->changeTexture(crop->stageTextures[stage]);
        }

        // 判断是否成熟 (假设 index 6 是 mature)
        if (stage >= 6) {
            crop->changeState(new MatureState());
        }
    }
}

// ================= MatureState (成熟) =================

void MatureState::enter(Crop* crop) {
    CCLOG("Enter MatureState");
    // 确保显示成熟图片
    crop->changeTexture("../Resources/mature.png");

    // 显示成熟提示图标 (复用原有逻辑)
    Sprite* matureSprite = Sprite::create("../Resources/mature_.png");
    if (matureSprite) {
        matureSprite->setPosition(crop->getContentSize().width / 2, crop->getContentSize().height + 20);
        crop->addChild(matureSprite, 1, "matureSprite");
        crop->scheduleOnce(CC_SCHEDULE_SELECTOR(Crop::removeSprite), 2.0f);
    }
}

void MatureState::execute(Crop* crop, float dt) {
    // 成熟后通常不再枯死，或者逻辑不同
}

void MatureState::onClick(Crop* crop, EventMouse* event) {
    if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
        // 收获逻辑
        if (crop->getSceneRef()) {
            // 【修正】这里必须把 crop 自己传进去，否则 GameScene 不知道要删除谁
            crop->getSceneRef()->removeCrop(crop);

            ItemManager::getInstance(1, "")->addItem(Item::ItemType::FRUIT);
            CCLOG("Harvested!");
        }
    }
}

void MatureState::exit(Crop* crop) {
    // 清理成熟图标
    crop->removeChildByName("matureSprite");
}

void MatureState::grow(Crop* crop) {
    // 成熟后不再生长
}

// ================= DeadState (枯死) =================

void DeadState::enter(Crop* crop) {
    CCLOG("Enter DeadState");
    crop->changeTexture("../Resources/dead.png");

    // 原逻辑中的 deadSprite
    crop->deadSprite = Sprite::create("../Resources/dead.png");
    if (crop->deadSprite) {
        crop->deadSprite->setPosition(Vec2(10, 10));
        crop->addChild(crop->deadSprite);
    }

    // 重置移除计数器
    crop->deadTimes = 0;
}

void DeadState::execute(Crop* crop, float dt) {
    // 每一帧累加计数，模拟原来的 timer++ 逻辑
    // 为了更平滑，可以使用时间累加而不是帧数
    crop->deadTimes++;

    if (crop->deadTimes > 500) { // 保持原来的阈值
        CCLOG("Removing dead crop");
        if (crop->deadSprite) {
            crop->deadSprite->removeFromParentAndCleanup(true);
            crop->deadSprite = nullptr;
        }
        if (crop->getSceneRef()) {
            crop->getSceneRef()->removeCrop(crop);
        }
    }
}

void DeadState::onClick(Crop* crop, EventMouse* event) {
    // 枯死状态下可以添加铲除逻辑 (如果需要)
    // 目前根据原代码，此处无交互
}

void DeadState::exit(Crop* crop) {}

void DeadState::grow(Crop* crop) {
    // 死了就不能长了
}