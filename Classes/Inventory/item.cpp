/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     item.cpp
 * File Function: 实现item类，实现物品的创造使用
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#include "Inventory/Item.h"
#include "Farming/Crop.h"
#include "World/Map.h"
#include "Scenes/GameScene.h"
#include "Characters/NpcTemplate.h"
#include "Characters/Elliott.h"
#include "Characters/Sam.h"
#include "Characters/Shane.h"
#include "Characters/Abigail.h"
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
    // 获取所有 NPC
    std::vector<NpcTemplate*> npcs;
    npcs.push_back(Elliott::getInstance());
    npcs.push_back(Sam::getInstance());
    npcs.push_back(Shane::getInstance());
    npcs.push_back(Abigail::getInstance());
   
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

        //  decreaseQuantity(1);
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fish.png");
        break;

    case ItemType::SEED: //种树

        //商店 卖出
        CCLOG("Using SEED: Chopping a tree...");
        //add plant

        /*
        if (MapManager::getInstance()->currentMapLabel == 1) {
            int x;
            Crop* newCrop = Crop::create(GameScene::getInstance(x, ""), " ");
            newCrop->setPosition(playerPos);  // 设置作物的位置
            GameScene::getInstance(x, "")->myCrops.push_back(newCrop); // 传递gameScene指针给Crop
            GameScene::getInstance(x, "")->addChild(newCrop);  // 将作物添加到场景中
            // 设置回调函数

            newCrop->setTextureChangedCallback([this](const std::string& texturePath) {
                int x;
                GameScene::getInstance(x, "")->onCropTextureChanged(texturePath);
                //  isCropAlive = true;  // 初始化标志为 true
                });
            decreaseQuantity(1);
        }
        */
        /*
        if (MapManager::getInstance()->currentMapLabel == 1) {

            newCrop->setPosition(playerPos);  // 设置作物的位置
            GameScene::getInstance(x, "")->addChild(newCrop);  // 将作物添加到场景中 
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
       
        for (auto npc : npcs) {
            if (playerPos.distance(npc->getNpcsPosition())<50){
                npc->setAffection(npc->getAffection() + 10);
                decreaseQuantity(1);
            }
        }
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
    // 如果数量为0，可以选择隐藏物品或数量标签
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
    // 创建物品图标
     itemSprite = Sprite::create(itemImagePath);
    if (!itemSprite) {
        CCLOG("Failed to load item image: %s", itemImagePath.c_str());
        return;
    }

    // 设置物品图标位置（人物正上方）
    auto playerPos = player->getPosition();
    itemSprite->setPosition(playerPos.x, playerPos.y + 20); // 正上方 32 像素
    player->getParent()->addChild(itemSprite, 10); // 添加到 Player 的父节点

    // 创建动画：向上移动 + 渐隐消失
    auto moveBy = MoveBy::create(5.0f, Vec2(1, 0)); // 向上移动 16 像素
    auto fadeOut = FadeOut::create(1.0f); // 渐隐
    auto removeSelf = RemoveSelf::create(); // 动画完成后移除
    auto sequence = Sequence::create(Spawn::create(moveBy, fadeOut, nullptr), removeSelf, nullptr);

    // 运行动画
    itemSprite->runAction(sequence);
}
