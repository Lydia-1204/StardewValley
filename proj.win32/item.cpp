/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     item.cpp
 * File Function: ʵ��item�࣬ʵ����Ʒ�Ĵ���ʹ��
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#include "item.h"
#include"Crop.h"
#include "map.h"
#include"GameScene.h"

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
        this->initWithFile("../Resources/item/seed.png");
        price=100;
        break;
    case ItemType::FISH:
        this->initWithFile("../Resources/item/fish.png");
        price = 300;
        break;
    case ItemType::EGG:
        this->initWithFile("../Resources/item/egg.png");
        price = 100;
        break;
    case ItemType::MILK:
        this->initWithFile("../Resources/item/milk.png");
        price = 200;
        break;
    case ItemType::BONE:
        this->initWithFile("../Resources/item/bone.png");
        price = 150;
        break;
    case ItemType::WOOL:
        this->initWithFile("../Resources/item/wool.png");
        price = 250;
        break;
    case ItemType::WOODEN:
        this->initWithFile("../Resources/item/wooden.png");
        price = 200;
        break;
    case ItemType::FRUIT:
        this->initWithFile("../Resources/item/fruit.png");
        price = 300;
        break;
    case ItemType::MINERAL:
        this->initWithFile("../Resources/item/mineral.png");
        price = 80;
        break;
    case ItemType::FAT:
        this->initWithFile("../Resources/item/fat.png");
        price = 200;
        break;
    case ItemType::GIFT:
        this->initWithFile("../Resources/item/gift.png");
        price = 300;
        break;
    default:
        break;
    }
    quantityLabel = Label::createWithTTF("0", "../Resources/fonts/arial.ttf", 18);
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

        //  decreaseQuantity(1);
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fish.png");
        break;

    case ItemType::SEED: //����

        //�̵� ����
        CCLOG("Using SEED: Chopping a tree...");
        //add plant

        /*
        if (MapManager::getInstance()->currentMapLabel == 1) {
            int x;
            Crop* newCrop = Crop::create(GameScene::getInstance(x, ""), " ");
            newCrop->setPosition(playerPos);  // ���������λ��
            GameScene::getInstance(x, "")->myCrops.push_back(newCrop); // ����gameSceneָ���Crop
            GameScene::getInstance(x, "")->addChild(newCrop);  // ��������ӵ�������
            // ���ûص�����

            newCrop->setTextureChangedCallback([this](const std::string& texturePath) {
                int x;
                GameScene::getInstance(x, "")->onCropTextureChanged(texturePath);
                //  isCropAlive = true;  // ��ʼ����־Ϊ true
                });
            decreaseQuantity(1);
        }
        */
        /*
        if (MapManager::getInstance()->currentMapLabel == 1) {

            newCrop->setPosition(playerPos);  // ���������λ��
            GameScene::getInstance(x, "")->addChild(newCrop);  // ��������ӵ������� 
            decreaseQuantity(1);
        }
        */
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/seed.png");
        break;

    case ItemType::EGG: // 
        CCLOG("Using EGG: ...");
     

      //  decreaseQuantity(1);
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/egg.png");
        break;

    case ItemType::MILK: // 
        CCLOG("Using MILK: ...");
      

       // decreaseQuantity(1);
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/milk.png");
        break;
    case ItemType::BONE: // 
        CCLOG("Using MILK: ...");


       // decreaseQuantity(1);
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/bone.png");
        break;
    case ItemType::WOOL: // 

        CCLOG("Using MILK: ...");
       // decreaseQuantity(1);       
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/wool.png");
        break;
    case ItemType::WOODEN: // 

        CCLOG("Using WOODEN: ...");
     //   decreaseQuantity(1);

        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/wooden.png");
        break;
    case ItemType::FRUIT: // 

     //   decreaseQuantity(1);
        CCLOG("Using FRUIT: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fruit.png");
        break;
    case ItemType::MINERAL: // 
      //  decreaseQuantity(1);
        CCLOG("Using MINERAL: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/mineral.png");
        break;
    case ItemType::FAT: // 
      //  decreaseQuantity(1);
        CCLOG("Using FAT: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fat.png");
        break;
    case ItemType::GIFT: // 
     //   decreaseQuantity(1);
        CCLOG("Using GIFT: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/gift.png");
        break;
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
       this->setVisible(false);
    }
    else {
        this->setVisible(true);
    };
}
int Item::getPrice() {
    return price;
};

void Item::displayItemAbovePlayer(Player* player, const std::string& itemImagePath) {
    // ������Ʒͼ��
     itemSprite = Sprite::create(itemImagePath);
    if (!itemSprite) {
        CCLOG("Failed to load item image: %s", itemImagePath.c_str());
        return;
    }

    // ������Ʒͼ��λ�ã��������Ϸ���
    auto playerPos = player->getPosition();
    itemSprite->setPosition(playerPos.x, playerPos.y + 20); // ���Ϸ� 32 ����
    player->getParent()->addChild(itemSprite, 10); // ��ӵ� Player �ĸ��ڵ�

    // ���������������ƶ� + ������ʧ
    auto moveBy = MoveBy::create(5.0f, Vec2(1, 0)); // �����ƶ� 16 ����
    auto fadeOut = FadeOut::create(1.0f); // ����
    auto removeSelf = RemoveSelf::create(); // ������ɺ��Ƴ�
    auto sequence = Sequence::create(Spawn::create(moveBy, fadeOut, nullptr), removeSelf, nullptr);

    // ���ж���
    itemSprite->runAction(sequence);
}