/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     item.cpp
 * File Function: 实现item类，实现物品的创造使用
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 * Refactor:      Items System Refactoring (Factory & Object Pool)
 *********************************************************************************************************/
#include "Inventory/Item.h"
#include "Inventory/ItemFactory.h" // 引入工厂类
#include "Farming/Crop.h"
#include "World/Map.h"
#include "Scenes/GameScene.h"
#include "Characters/NpcTemplate.h"

#include "Characters/Elliott.h"
#include "Characters/Sam.h"
#include "Characters/Shane.h"
#include "Characters/Abigail.h"
// ---------------------------------------------------------

USING_NS_CC;

Item *Item::create(ItemType type)
{
    /* --------------------------------------------------------------------------
     * 【使用工厂模式重构】对象创建
     * 说明：不再直接 new 对象，而是委托给工厂创建，隐藏创建细节。
     * -------------------------------------------------------------------------- */
    return ItemFactory::getInstance()->createItem(type);
}

bool Item::init(ItemType type)
{
    if (!Sprite::init())
    {
        return false;
    }

    this->type = type;
    this->quantity = 1;

    /* --------------------------------------------------------------------------
     * 【使用工厂模式重构】对象初始化
     * 说明：委托工厂配置数据（图片、价格），消除了此处原本冗长的 switch-case。
     * 工厂根据传入的 type，查找注册表并注入纹理和价格数据。
     * 此方法同样被对象池的 reset() 调用，确保新对象和复用对象的数据配置逻辑唯一。
     * -------------------------------------------------------------------------- */
    ItemFactory::getInstance()->configureItem(this, type);

    quantityLabel = Label::createWithTTF("0", "../Resources/fonts/arial.ttf", 18);
    quantityLabel->setColor(Color3B::BLACK);
    quantityLabel->setPosition(this->getContentSize().width, this->getContentSize().height);
    this->addChild(quantityLabel, 10);
    updateItemUI();
    return true;
}

void Item::reset(ItemType newType)
{
    /* --------------------------------------------------------------------------
     * 【使用对象池模式重构】对象重置
     * 说明：复用的对象可能保留着上一次使用的状态（如透明度、缩放、旋转）。
     *       当旧对象被复用时，将其状态恢复为初始状态，并加载新数据。
     *       这里再次利用了工厂模式的 configureItem 能力。
     * -------------------------------------------------------------------------- */
    this->type = newType;
    this->quantity = 1;
    this->setVisible(true);
    this->setOpacity(255);
    this->setScale(1.0f);
    this->setRotation(0.0f);

    ItemFactory::getInstance()->configureItem(this, newType);

    if (quantityLabel)
    {
        quantityLabel->setPosition(this->getContentSize().width, this->getContentSize().height);
    }
    updateItemUI();
}

Item::ItemType Item::getType() const
{
    return type;
}

void Item::useitem()
{
    auto playerPos = Player::getInstance(selectedCharacter, nickname)->getPosition();
    auto direction = Player::getInstance(selectedCharacter, nickname)->_currentDirection;
    Vec2 dstPos;

    // 获取所有 NPC
    std::vector<NpcTemplate *> npcs;
    // 确保这些 NPC 类有 getInstance 方法。
    npcs.push_back(Elliott::getInstance());
    npcs.push_back(Sam::getInstance());
    npcs.push_back(Shane::getInstance());
    npcs.push_back(Abigail::getInstance());

    switch (direction)
    {
    case 0: // 下
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 1: // 右
        dstPos = Vec2(playerPos.x + 16, playerPos.y);
        break;
    case 2: // 上
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 3: // 左
        dstPos = Vec2(playerPos.x - 16, playerPos.y);
        break;
    }

    switch (type)
    {
    case ItemType::FISH:
        CCLOG("Using FISH: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fish.png");
        break;

    case ItemType::SEED:
        CCLOG("Using SEED: Chopping a tree...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/seed.png");
        break;

    case ItemType::EGG:
        CCLOG("Using EGG: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/egg.png");
        break;

    case ItemType::MILK:
        CCLOG("Using MILK: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/milk.png");
        break;
    case ItemType::BONE:
        CCLOG("Using BONE: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/bone.png");
        break;
    case ItemType::WOOL:
        CCLOG("Using WOOL: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/wool.png");
        break;
    case ItemType::WOODEN:
        CCLOG("Using WOODEN: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/wooden.png");
        break;
    case ItemType::FRUIT:
        CCLOG("Using FRUIT: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fruit.png");
        break;
    case ItemType::MINERAL:
        CCLOG("Using MINERAL: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/mineral.png");
        break;
    case ItemType::FAT:
        CCLOG("Using FAT: ...");
        displayItemAbovePlayer(Player::getInstance(1, " "), "../Resources/item/fat.png");
        break;
    case ItemType::GIFT:
        for (auto npc : npcs)
        {
            if (npc && playerPos.distance(npc->getNpcsPosition()) < 50)
            {
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

void Item::increaseQuantity(int amout)
{
    quantity += amout;
    updateItemUI();
}
void Item::decreaseQuantity(int amout)
{
    if (quantity > 0)
        quantity -= amout;

    updateItemUI();
}

int Item::getQuantity() const
{
    return quantity;
}

void Item::updateItemUI()
{
    quantityLabel->setString(StringUtils::format("%d", quantity));
    if (quantity == 0)
    {
        this->setVisible(false);
    }
    else
    {
        this->setVisible(true);
    };
}

int Item::getPrice()
{
    return price;
};

void Item::displayItemAbovePlayer(Player *player, const std::string &itemImagePath)
{
    itemSprite = Sprite::create(itemImagePath);
    if (!itemSprite)
    {
        CCLOG("Failed to load item image: %s", itemImagePath.c_str());
        return;
    }

    auto playerPos = player->getPosition();
    itemSprite->setPosition(playerPos.x, playerPos.y + 20);
    player->getParent()->addChild(itemSprite, 10);

    auto moveBy = MoveBy::create(5.0f, Vec2(1, 0));
    auto fadeOut = FadeOut::create(1.0f);
    auto removeSelf = RemoveSelf::create();
    auto sequence = Sequence::create(Spawn::create(moveBy, fadeOut, nullptr), removeSelf, nullptr);

    itemSprite->runAction(sequence);
}