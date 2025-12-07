/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    ItemFactory.cpp
 * File Function: 实现ItemFactory类，使用工厂模式创建物品，消除switch-case语句
 * Author:       王小萌 2351882
 * Update Date:  2024/12/21
 *********************************************************************************************************/
#include "Inventory/ItemFactory.h"
#include "Inventory/Item.h"

USING_NS_CC;

ItemFactory *ItemFactory::_instance = nullptr;

ItemFactory *ItemFactory::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ItemFactory();
    }
    return _instance;
}

ItemFactory::ItemFactory()
{
    registerCreators();
}

ItemFactory::~ItemFactory()
{
    cleanup();
}

void ItemFactory::registerCreators()
{
    // 注册种子创建器
    _creators[Item::ItemType::SEED] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::SEED))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册鱼肉创建器
    _creators[Item::ItemType::FISH] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::FISH))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册鸡蛋创建器
    _creators[Item::ItemType::EGG] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::EGG))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册牛奶创建器
    _creators[Item::ItemType::MILK] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::MILK))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册骨头创建器
    _creators[Item::ItemType::BONE] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::BONE))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册羊毛创建器
    _creators[Item::ItemType::WOOL] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::WOOL))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册木头创建器
    _creators[Item::ItemType::WOODEN] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::WOODEN))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册南瓜创建器
    _creators[Item::ItemType::FRUIT] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::FRUIT))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册矿石创建器
    _creators[Item::ItemType::MINERAL] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::MINERAL))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册礼物创建器
    _creators[Item::ItemType::GIFT] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::GIFT))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册油脂创建器
    _creators[Item::ItemType::FAT] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::FAT))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };

    // 注册空类型创建器
    _creators[Item::ItemType::NONE] = []() -> Item *
    {
        Item *item = new Item();
        if (item && item->init(Item::ItemType::NONE))
        {
            item->autorelease();
            return item;
        }
        CC_SAFE_DELETE(item);
        return nullptr;
    };
}

Item *ItemFactory::createItem(Item::ItemType type)
{
    auto it = _creators.find(type);
    if (it != _creators.end())
    {
        return it->second();
    }
    CCLOG("ItemFactory: Unknown item type %d", static_cast<int>(type));
    return nullptr;
}

void ItemFactory::preloadItems(Item::ItemType type, int count)
{
    auto it = _itemPools.find(type);
    if (it == _itemPools.end())
    {
        _itemPools[type] = std::vector<Item *>();
    }

    for (int i = 0; i < count; i++)
    {
        Item *item = createItem(type);
        if (item)
        {
            item->retain();
            item->setVisible(false);
            _itemPools[type].push_back(item);
        }
    }
}

void ItemFactory::cleanup()
{
    // 清理对象池
    for (auto &pair : _itemPools)
    {
        for (auto item : pair.second)
        {
            CC_SAFE_RELEASE(item);
        }
        pair.second.clear();
    }
    _itemPools.clear();

    // 清理创建器映射
    _creators.clear();

    // 清理单例实例
    if (_instance)
    {
        delete _instance;
        _instance = nullptr;
    }
}