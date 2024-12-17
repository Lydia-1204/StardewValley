/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     item.cpp
 * File Function: 实现item类，实现物品的创造使用
 * Author:        王小萌 2351882
 * Update Date:   2024/12/17
 *********************************************************************************************************/
#include "Item.h"

USING_NS_CC;

Item* Item::create(ItemType type) {
    Item* item = new (std::nothrow)Item();
   
    if (item && item->init(type)) {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}


bool Item::init(ItemType type) {
    if (!Sprite::init()) {
        return false;
    }

    this->type = type;
    quantity = 1;
    // 根据工具类型加载不同的资源
    switch (type) {
    case ItemType::SEED:
        this->initWithFile("../Resources/item/fish.png");
        break;
    case ItemType::FISH:
        this->initWithFile("../Resources/item/fish.png");
        break;
    case ItemType::EGG:
        this->initWithFile("../Resources/item/egg.png");
        break;
    case ItemType::MILK:
        this->initWithFile("../Resources/item/milk.png");
        break;
    case ItemType::BONE:
        this->initWithFile("../Resources/item/bone.png");
        break;
    case ItemType::WOOL:
        this->initWithFile("../Resources/item/wool.png");
        break;
    }
    quantityLabel = Label::createWithTTF("0", "../Resources/fonts/arial.ttf", 13);
    quantityLabel->setColor(Color3B::BLACK);
    quantityLabel->setPosition(this->getContentSize().width , this->getContentSize().height);
    this->addChild(quantityLabel,10);
    updateItemUI(); // 初始更新UI
    return true;
}

Item::ItemType Item::getType() const {
    return type;
}


void Item::useitem() {
    auto playerPos = Player::getInstance(selectedCharacter, nickname)->getPosition();
    auto direction = Player::getInstance(selectedCharacter, nickname)->_currentDirection;
    Vec2 dstPos;
    switch (direction) {

    case 0://下
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 1://右
        dstPos = Vec2(playerPos.x + 16, playerPos.y);
        break;
    case 2://上
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 3://左
        dstPos = Vec2(playerPos.x - 16, playerPos.y);
        break;
    }
    switch (type) {
    case ItemType::FISH: // 
        CCLOG("Using FISH: ...");

        break;

    case ItemType::SEED: //种树
        CCLOG("Using SEED: Chopping a tree...");

        break;

    case ItemType::EGG: // 
        CCLOG("Using EGG: ...");
     
        break;

    case ItemType::MILK: // 
        CCLOG("Using MILK: ...");
      
        break;
    case ItemType::BONE: // 
        CCLOG("Using MILK: ...");

        break;
    case ItemType::WOOL: // 
        CCLOG("Using MILK: ...");
    default:
        CCLOG("Invalid Item type or Item not equipped.");
        break;
    }

}
void Item::increaseQuantity(int amout) {

    quantity+=amout;
    updateItemUI();
}
void Item::decreaseQuantity(int amout) {
    if (quantity > 0)
        quantity-=amout;
    updateItemUI();
}

int Item::getQuantity()const {

    return quantity;
}

void Item::updateItemUI() {
    quantityLabel->setString(StringUtils::format("%d", quantity));
    // 如果数量为0，可以选择隐藏物品或数量标签
    if (quantity == 0) {
        quantityLabel->setVisible(false);
        // 或者 this->setVisible(false);
    }
    else {
        quantityLabel->setVisible(true);
    };
}