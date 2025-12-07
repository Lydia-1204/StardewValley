/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    ItemManager.cpp
 * File Function: 实现ItemManager类，使用对象池模式管理物品，配合工厂模式创建物品
 * Author:       王小萌 2351882
 * Update Date:  2024/12/21
 *********************************************************************************************************/
#include "Inventory/ItemManager.h"
#include "Inventory/ItemFactory.h"
#include "Inventory/Chest.h"
#include "Characters/Player.h"
#include "Scenes/GameScene.h"

USING_NS_CC;

ItemManager *ItemManager::_instance = nullptr;

ItemManager::ItemManager() : selectedCharacter(1), nickname("guest"), selectedItemIndex(-1), chest(nullptr), player(nullptr), selectionBox(nullptr)
{
    preloadPool();
    initKeyboardListener();
}

ItemManager *ItemManager::getInstance(int selectedCharacter, const std::string &nickname)
{
    if (_instance == nullptr)
    {
        _instance = new ItemManager();
        _instance->selectedCharacter = selectedCharacter;
        _instance->nickname = nickname;
    }
    return _instance;
}

ItemManager::~ItemManager()
{
    // 清理对象池
    for (auto item : availablePool)
    {
        CC_SAFE_RELEASE(item);
    }
    for (auto item : activePool)
    {
        CC_SAFE_RELEASE(item);
    }
    for (auto item : items)
    {
        CC_SAFE_RELEASE(item);
    }
    availablePool.clear();
    activePool.clear();
    items.clear();

    // 清理UI元素
    if (selectionBox)
    {
        selectionBox->removeFromParent();
    }
}

void ItemManager::preloadPool()
{
    // 预创建常用物品到对象池
    expandPool(Item::ItemType::SEED, 10);
    expandPool(Item::ItemType::FISH, 5);
    expandPool(Item::ItemType::EGG, 3);
    expandPool(Item::ItemType::MILK, 3);
    expandPool(Item::ItemType::WOOL, 2);
    expandPool(Item::ItemType::BONE, 2);
    expandPool(Item::ItemType::WOODEN, 2);
    expandPool(Item::ItemType::FRUIT, 2);
    expandPool(Item::ItemType::MINERAL, 5);
    expandPool(Item::ItemType::GIFT, 3);
    expandPool(Item::ItemType::FAT, 2);
}

void ItemManager::expandPool(Item::ItemType type, int count)
{
    ItemFactory *factory = ItemFactory::getInstance();
    for (int i = 0; i < count; i++)
    {
        Item *item = factory->createItem(type);
        if (item)
        {
            item->retain();
            item->setVisible(false);
            availablePool.push_back(item);
        }
    }
}

Item *ItemManager::getItemFromPool(Item::ItemType type)
{
    // 尝试从可用池中找到相同类型的物品
    for (auto it = availablePool.begin(); it != availablePool.end(); ++it)
    {
        if ((*it)->getType() == type)
        {
            Item *item = *it;
            availablePool.erase(it);
            item->reset();
            item->type = type; // 恢复类型
            item->quantity = 1;
            item->setVisible(true);
            activePool.push_back(item);
            return item;
        }
    }

    // 如果没有可用的物品，扩展池
    expandPool(type, 5);
    return getItemFromPool(type);
}

void ItemManager::returnItemToPool(Item *item)
{
    // 从活动池中移除
    for (auto it = activePool.begin(); it != activePool.end(); ++it)
    {
        if (*it == item)
        {
            activePool.erase(it);
            break;
        }
    }

    // 重置并返回到可用池
    item->reset();
    availablePool.push_back(item);
}

void ItemManager::addItem(Item::ItemType type)
{
    Item *item = getItemFromPool(type);
    if (item)
    {
        items.push_back(item);
        this->addChild(item);

        // 设置位置
        int index = items.size() - 1;
        float x = 50 + (index % 5) * 60;
        float y = 500 - (index / 5) * 60;
        item->setPosition(Vec2(x, y));
    }
}

void ItemManager::removeItem(Item *item)
{
    if (!item)
        return;

    // 从items向量中移除
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (*it == item)
        {
            items.erase(it);
            break;
        }
    }

    // 返回到对象池
    returnItemToPool(item);

    // 从场景移除
    if (item->getParent())
    {
        item->removeFromParent();
    }

    updateItemPositions();
}

void ItemManager::recycleItem(Item *item)
{
    returnItemToPool(item);

    // 从items向量中移除
    for (auto it = items.begin(); it != items.end(); ++it)
    {
        if (*it == item)
        {
            items.erase(it);
            break;
        }
    }

    // 从场景移除
    if (item->getParent())
    {
        item->removeFromParent();
    }
}

void ItemManager::selectItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        selectedItemIndex = index;
        updateSelectionBox();
    }
}

void ItemManager::useitem()
{
    if (selectedItemIndex >= 0 && selectedItemIndex < items.size())
    {
        Item *item = items[selectedItemIndex];
        if (item && item->getQuantity() > 0)
        {
            item->useitem();
            item->decreaseQuantity(1);

            // 如果数量为0，回收物品
            if (item->getQuantity() <= 0)
            {
                recycleItem(item);
                if (selectedItemIndex >= items.size())
                {
                    selectedItemIndex = items.size() - 1;
                }
                updateSelectionBox();
            }
        }
    }
}

void ItemManager::discardItem()
{
    if (selectedItemIndex >= 0 && selectedItemIndex < items.size())
    {
        Item *item = items[selectedItemIndex];
        recycleItem(item);
        selectedItemIndex = -1;
        updateSelectionBox();
    }
}

Item *ItemManager::getItem(int index)
{
    if (index >= 0 && index < items.size())
    {
        return items[index];
    }
    return nullptr;
}

int ItemManager::getItemCount() const
{
    return items.size();
}

int ItemManager::getItemQuantity(Item::ItemType type)
{
    int total = 0;
    for (auto item : items)
    {
        if (item->getType() == type)
        {
            total += item->getQuantity();
        }
    }
    return total;
}

void ItemManager::clearAllItems()
{
    // 回收所有物品
    for (auto item : items)
    {
        returnItemToPool(item);
        if (item->getParent())
        {
            item->removeFromParent();
        }
    }
    items.clear();
    selectedItemIndex = -1;
    updateSelectionBox();
}

size_t ItemManager::getPoolSize(Item::ItemType type) const
{
    size_t count = 0;
    for (auto item : availablePool)
    {
        if (item->getType() == type)
        {
            count++;
        }
    }
    return count;
}

void ItemManager::updateItemPositions()
{
    for (size_t i = 0; i < items.size(); i++)
    {
        float x = 50 + (i % 5) * 60;
        float y = 500 - (i / 5) * 60;
        items[i]->setPosition(Vec2(x, y));
    }
}

void ItemManager::refreshUI()
{
    updateItemPositions();
    for (auto item : items)
    {
        item->updateItemUI();
    }
}

void ItemManager::updateSelectionBox()
{
    if (!selectionBox)
    {
        selectionBox = Sprite::create("../Resources/ui/selection.png");
        if (selectionBox)
        {
            this->addChild(selectionBox, -1);
        }
    }

    if (selectedItemIndex >= 0 && selectedItemIndex < items.size())
    {
        Item *item = items[selectedItemIndex];
        if (item && selectionBox)
        {
            selectionBox->setPosition(item->getPosition());
            selectionBox->setVisible(true);
        }
    }
    else if (selectionBox)
    {
        selectionBox->setVisible(false);
    }
}

void ItemManager::initKeyboardListener()
{
    auto keyboardListener = EventListenerKeyboard::create();

    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event)
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_1:
            selectItem(0);
            break;
        case EventKeyboard::KeyCode::KEY_2:
            selectItem(1);
            break;
        case EventKeyboard::KeyCode::KEY_3:
            selectItem(2);
            break;
        case EventKeyboard::KeyCode::KEY_4:
            selectItem(3);
            break;
        case EventKeyboard::KeyCode::KEY_5:
            selectItem(4);
            break;
        case EventKeyboard::KeyCode::KEY_ENTER:
            useitem();
            break;
        case EventKeyboard::KeyCode::KEY_BACKSPACE:
            discardItem();
            break;
        default:
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}