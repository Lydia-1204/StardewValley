// --- START OF FILE Inventory/ItemFactory.cpp ---
#include "Inventory/ItemFactory.h"

ItemFactory *ItemFactory::instance = nullptr;

ItemFactory *ItemFactory::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ItemFactory();
    }
    return instance;
}

ItemFactory::ItemFactory()
{
    initRegistry();
}

void ItemFactory::initRegistry()
{
    /* --------------------------------------------------------------------------
     * 【使用工厂模式重构】
     * 说明：使用 Map 注册表替代了原 Item.cpp 中的 switch-case 硬编码，新增物品只需在此注册表中添加一行配置。
     *       将物品的属性配置（资源路径、价格等）与 Item 类的业务逻辑完全分离，符合开闭原则（OCP）。
     * -------------------------------------------------------------------------- */
    itemRegistry[Item::ItemType::SEED] = {"../Resources/item/seed.png", 100};
    itemRegistry[Item::ItemType::FISH] = {"../Resources/item/fish.png", 300};
    itemRegistry[Item::ItemType::EGG] = {"../Resources/item/egg.png", 100};
    itemRegistry[Item::ItemType::MILK] = {"../Resources/item/milk.png", 200};
    itemRegistry[Item::ItemType::BONE] = {"../Resources/item/bone.png", 150};
    itemRegistry[Item::ItemType::WOOL] = {"../Resources/item/wool.png", 250};
    itemRegistry[Item::ItemType::WOODEN] = {"../Resources/item/wooden.png", 200};
    itemRegistry[Item::ItemType::FRUIT] = {"../Resources/item/fruit.png", 300};
    itemRegistry[Item::ItemType::MINERAL] = {"../Resources/item/mineral.png", 80};
    itemRegistry[Item::ItemType::FAT] = {"../Resources/item/fat.png", 200};
    itemRegistry[Item::ItemType::GIFT] = {"../Resources/item/gift.png", 300};
}

Item *ItemFactory::createItem(Item::ItemType type)
{
    Item *item = new (std::nothrow) Item();
    if (item && item->init(type))
    {
        item->autorelease();
        return item;
    }
    CC_SAFE_DELETE(item);
    return nullptr;
}

void ItemFactory::configureItem(Item *item, Item::ItemType type)
{
    // 查找配置
    auto it = itemRegistry.find(type);
    if (it != itemRegistry.end())
    {
        const ItemData &data = it->second;
        // 设置纹理和价格
        item->setTexture(it->second.texturePath);
        item->price = data.price;
    }
    else
    {
        // 默认处理或错误处理
        CCLOG("ItemFactory: Warning, type not found in registry.");
    }
}