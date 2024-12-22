/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.cpp
 * File Function: 实现Item类与Itemmanager，实现物品的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/


#include "item.h"
#include "itemManager.h"
#include "map.h"
#include "Chest.h"
#include"UIManager.h"
USING_NS_CC;



ItemManager* ItemManager::instance = nullptr;


ItemManager* ItemManager::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {  // 如果实例不存在，则创建
        instance = new (std::nothrow) ItemManager();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();  // 添加到内存管理系统
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // 返回唯一实例
}





bool ItemManager::init(int _selectedCharacter, const std::string& _nickname) {
    if (!Node::init()) {
        return false;
    }
    selectedCharacter = _selectedCharacter;
    nickname = _nickname;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // 物品栏背景
     /*
    auto ItemBarBg = Sprite::create("../ResourcesL/ooseSprites-73/DialogBoxGreen..png");
    ItemBarBg->setPosition(visibleSize.width / 2, visibleSize.height * 0.1);
    this->addChild(ItemBarBg);*/

    // 初始化物品栏
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    float startX = (visibleSize.width - gridWidth * 10) / 2.0f; // 居中物品栏
    float startY = visibleSize.height * 0.1f+32.0f;

    for (int i = 0; i < 12; i++) {
        auto grid = Sprite::create("../Resources/tools/tools_bg.png");
        grid->setPosition(startX + i * gridWidth, startY);
        this->addChild(grid);

        Items.push_back(nullptr); // 初始化物品栏为空
    }

    // 选中框
    selectionBox = Sprite::create("../Resources/tools/tools_selected.png");
    selectionBox->setVisible(false);
    this->addChild(selectionBox, 10);

    // 初始化状态
    selectedItemIndex = -1;
    //箱子
    chest = Chest::getInstance();

    // 添加事件监听器
   // 添加事件监听器
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
        CCLOG("mouseDown");
        auto locationInWorld = event->getLocationInView();  // 获取屏幕视图中的坐标
        auto locationInItemsBg = this->convertToNodeSpace(locationInWorld); // 转换到 Items_bg 的坐标系
        CCLOG("Mouse position in Items_bg space: %f, %f", locationInItemsBg.x, locationInItemsBg.y);

        // 检测是否点击物品

        CCLOG("Adjusted Mouse Position: %f, %f", locationInItemsBg.x, locationInItemsBg.y);

        // 判断鼠标是否点击物品栏

        int x = 0;
        bool isitemManaager = 0;
        for (int i = 0; i < Items.size(); i++) {
            if (Items[i] && Items[i]->getBoundingBox().containsPoint(locationInItemsBg)) {
                if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                    CCLOG("Item[%d] selected and used", i);
                    isitemManaager = 1;
                    selectItem(i);  // 选中物品
                    if (chest->isOpen == 1) {//与箱子交互
                        chest->addItem(Items[i]);
                        discardItem();
                    }
                    else if (UIManager::getInstance(x, "")->isPriceBoardOpen == 1) {//买卖东西

                        if (Items[i]->getType() != Item::ItemType::GIFT) {
                            Items[i]->decreaseQuantity(1);
                            UIManager::getInstance(x, "")->setMoney(Items[i]->price);
                        }
                    }
                   
                }
                else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                    discardItem();  // 右键丢弃物品
                }
                return;
            }
        }
        if (!isitemManaager) {//点击其他
            if (this && selectedItemIndex >= 0 &&
                selectedItemIndex < Items.size()) {
                if (Items[selectedItemIndex]->quantity > 0)
                    useitem();

            }

        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    initKeyboardListener();
    return true;
}

void ItemManager::addItem(Item::ItemType type) {
   
  

    float gridWidth = 32.0f; // 物品栏宽度
    float startX = (Director::getInstance()->getVisibleSize().width - gridWidth * 10) / 2.0f;
    float startY = Director::getInstance()->getVisibleSize().height * 0.1f + 32.0f;

    for (int i = 0; i < Items.size(); i++) {
        if (Items[i] != nullptr && Items[i]->getType() == type) {
            // 如果物品栏中已经有这个类型的物品，增加数量
            Items[i]->increaseQuantity(1);
            if (type == Item::ItemType::MINERAL && Items[i]->getQuantity() >= 30) {
                Items[i]->price = 60;
            }
            else if (type == Item::ItemType::MINERAL && Items[i]->getQuantity() < 30) {
                Items[i]->price = 80;
            }

            return;
        }
        if (Items[i] == nullptr) {
            // 如果有空位，创建新物品
            auto item = Item::create(type);
            item->setPosition(startX + i * gridWidth, startY);
            Items[i] = item;
            this->addChild(Items[i]);
            auto location = Items[i]->getPosition();
            CCLOG("Items [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }

   
    CCLOG("ItemBar is full, cannot add more Items.");
    auto fullLabel = Label::createWithTTF("ItemBar is full!", "fonts/Marker Felt.ttf", 24);
    fullLabel->setPosition(startX, startY+32.0f);
    this->addChild(fullLabel);
    fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
}

void ItemManager::discardItem() {
    if (selectedItemIndex < 0 || selectedItemIndex >= Items.size() || Items[selectedItemIndex] == nullptr) {
        CCLOG("No Item selected to discard");
        return;
    }

    auto Item = Items[selectedItemIndex];
    Item->decreaseQuantity(1);
    if (Item->getQuantity() <= 0) {
        this->removeChild(Item);
        Items[selectedItemIndex] = nullptr;
        selectedItemIndex = -1;
        selectionBox->setVisible(false);
    }
    
    CCLOG("Item discarded: %d", static_cast<int>(Item->getType()));

}
void ItemManager::selectItem(int index) {
    if (index < 0 || index >= Items.size() || Items[index] == nullptr) {
        CCLOG("Invalid Item selection");
        return;
    }

    selectedItemIndex = index;
    updateSelectionBox();
    CCLOG("Item selected: %d", index);
}

void ItemManager::useitem() {
    if (selectedItemIndex < 0 || selectedItemIndex >= Items.size() || Items[selectedItemIndex] == nullptr) {
        CCLOG("No Item selected to use");
        return;
    }

    auto Item = Items[selectedItemIndex];
    CCLOG("Using Item: %d", static_cast<int>(Item->getType()));
    // 添加物品使用逻辑
    Item->useitem();
}



void ItemManager::updateSelectionBox() {
    if (selectedItemIndex < 0 || selectedItemIndex >= Items.size() || Items[selectedItemIndex] == nullptr) {
        selectionBox->setVisible(false);
        return;
    }
    selectionBox->setVisible(true);
    selectionBox->setPosition(Items[selectedItemIndex]->getPosition());
}

void ItemManager::initKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_C:
            useitem(); // 使用当前物品
            break;
        case EventKeyboard::KeyCode::KEY_V:
            discardItem(); // 丢弃当前物品
            break;
        default:
            break;
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}