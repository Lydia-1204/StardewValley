/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     item.cpp
 * File Function: ʵ��item�࣬ʵ����Ʒ�Ĵ���ʹ��
 * Author:        ��С�� 2351882
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
    // ���ݹ������ͼ��ز�ͬ����Դ
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
    updateItemUI(); // ��ʼ����UI
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

    case 0://��
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 1://��
        dstPos = Vec2(playerPos.x + 16, playerPos.y);
        break;
    case 2://��
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 3://��
        dstPos = Vec2(playerPos.x - 16, playerPos.y);
        break;
    }
    switch (type) {
    case ItemType::FISH: // 
        CCLOG("Using FISH: ...");

        break;

    case ItemType::SEED: //����
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
    // �������Ϊ0������ѡ��������Ʒ��������ǩ
    if (quantity == 0) {
        quantityLabel->setVisible(false);
        // ���� this->setVisible(false);
    }
    else {
        quantityLabel->setVisible(true);
    };
}