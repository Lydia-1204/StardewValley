/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    Chest.cpp
 * File Function: 实现Chest类，使用工厂模式和对象池管理箱子物品存储
 * Author:       王小萌 2351882
 * Update Date:  2024/12/21
 *********************************************************************************************************/
#include "Inventory/Chest.h"
#include "Inventory/ItemFactory.h"
#include "Inventory/ItemManager.h"
#include "Inventory/Tool.h"
#include "Characters/Player.h"
#include "Scenes/GameScene.h"

USING_NS_CC;

Chest *Chest::_instance = nullptr;

Chest::Chest() : isOpen(false), chestPosition(Vec2::ZERO), chestSprite(nullptr),
                 chestPanel(nullptr), itemFactory(nullptr), itemManager(nullptr)
{
    itemFactory = ItemFactory::getInstance();
    itemManager = ItemManager::getInstance();
    initializeChestSlots();
    initializeToolGrids();
    preloadChestPool();
}

Chest *Chest::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new Chest();
        _instance->autorelease();
    }
    return _instance;
}

Chest::~Chest()
{
    // 清理存储的物品
    clearChest();

    // 清理工具
    for (auto tool : chestTools)
    {
        CC_SAFE_RELEASE(tool);
    }
    chestTools.clear();

    // 清理UI元素
    if (chestSprite)
    {
        chestSprite->removeFromParent();
    }
    if (chestPanel)
    {
        chestPanel->removeFromParent();
    }
    for (auto slot : itemSlots)
    {
        if (slot)
        {
            slot->removeFromParent();
        }
    }
    for (auto grid : toolgrids)
    {
        if (grid)
        {
            grid->removeFromParent();
        }
    }
    itemSlots.clear();
    toolgrids.clear();

    // 清理单例
    if (_instance)
    {
        _instance = nullptr;
    }
}

void Chest::openChest()
{
    if (isOpen)
        return;

    isOpen = true;
    showChestContents();

    // 创建箱子精灵
    if (!chestSprite)
    {
        chestSprite = Sprite::create("../Resources/box_open.png");
        if (chestSprite)
        {
            chestSprite->setPosition(chestPosition);
            this->addChild(chestSprite, 20);
        }
    }

    CCLOG("Chest opened at position (%.1f, %.1f)", chestPosition.x, chestPosition.y);
}

void Chest::closeChest()
{
    if (!isOpen)
        return;

    isOpen = false;
    hideChestContents();

    // 更换为关闭的箱子精灵
    if (chestSprite)
    {
        chestSprite->setTexture("../Resources/boxxx.png");
    }

    CCLOG("Chest closed");
}

void Chest::chestSetPosition(const Vec2 &position)
{
    chestPosition = position;
    if (chestSprite)
    {
        chestSprite->setPosition(position);
    }
}

Vec2 Chest::getChestPosition() const
{
    return chestPosition;
}

bool Chest::storeItem(Item::ItemType type, int quantity)
{
    if (quantity <= 0)
        return false;

    // 从ItemManager获取物品
    Item *itemToStore = nullptr;
    for (int i = 0; i < quantity; i++)
    {
        itemToStore = itemManager->getItemFromPool(type);
        if (itemToStore)
        {
            itemToStore->reset();
            itemToStore->type = type;
            itemToStore->quantity = 1;
            storedItems[type].push_back(itemToStore);
        }
    }

    if (itemToStore)
    {
        updateChestUI();
        CCLOG("Stored %d items of type %d in chest", quantity, static_cast<int>(type));
        return true;
    }

    return false;
}

Item *Chest::retrieveItem(Item::ItemType type)
{
    auto it = storedItems.find(type);
    if (it != storedItems.end() && !it->second.empty())
    {
        Item *item = it->second.back();
        it->second.pop_back();

        // 如果该类型物品为空，删除映射
        if (it->second.empty())
        {
            storedItems.erase(it);
        }

        updateChestUI();
        CCLOG("Retrieved item of type %d from chest", static_cast<int>(type));
        return item;
    }

    return nullptr;
}

bool Chest::hasItem(Item::ItemType type, int quantity)
{
    auto it = storedItems.find(type);
    if (it != storedItems.end())
    {
        return it->second.size() >= static_cast<size_t>(quantity);
    }
    return false;
}

int Chest::getItemCount(Item::ItemType type)
{
    auto it = storedItems.find(type);
    if (it != storedItems.end())
    {
        return static_cast<int>(it->second.size());
    }
    return 0;
}

void Chest::addTool(Tool *tool)
{
    if (!tool)
        return;

    // 查找空位或相同类型的工具
    for (int i = 0; i < 28; i++)
    {
        if (i == chestTools.size())
        {
            auto newTool = Tool::create(tool->getType());
            chestTools.push_back(newTool);

            // 设置位置（修正后的代码）
            if (i < toolgrids.size())
            {
                int x = i / 7;
                int y = i % 7;
                newTool->setPosition(toolgrids[x][y].getPosition()); // 使用 . 而不是 ->
            }

            this->addChild(newTool, 2);
            CCLOG("Tool added to chest at index %d", i);
            return;
        }
    }
}

void Chest::removeTool(Tool *tool)
{
    if (!tool)
        return;

    for (auto it = chestTools.begin(); it != chestTools.end(); ++it)
    {
        if (*it == tool)
        {
            chestTools.erase(it);
            if (tool->getParent())
            {
                tool->removeFromParent();
            }
            CCLOG("Tool removed from chest");
            return;
        }
    }
}

void Chest::storeAllItems()
{
    // 将ItemManager中所有物品存入箱子
    int itemCount = itemManager->getItemCount();
    for (int i = 0; i < itemCount; i++)
    {
        Item *item = itemManager->getItem(i);
        if (item)
        {
            storeItem(item->getType(), item->getQuantity());
        }
    }

    // 清空ItemManager
    itemManager->clearAllItems();

    CCLOG("Stored all items from ItemManager to chest");
}

void Chest::retrieveAllItems()
{
    // 将箱子中所有物品取出到ItemManager
    for (auto &pair : storedItems)
    {
        for (auto item : pair.second)
        {
            itemManager->addItem(item->getType());
        }
    }

    // 清空箱子
    clearChest();

    CCLOG("Retrieved all items from chest to ItemManager");
}

void Chest::toggleChest()
{
    if (isOpen)
    {
        closeChest();
    }
    else
    {
        openChest();
    }
}

void Chest::updateChestUI()
{
    if (!isOpen)
        return;

    // 更新物品槽位显示
    int slotIndex = 0;
    for (auto &pair : storedItems)
    {
        for (auto item : pair.second)
        {
            if (slotIndex < itemSlots.size())
            {
                updateSlotDisplay(slotIndex, item);
                slotIndex++;
            }
        }
    }

    // 隐藏未使用的槽位
    for (int i = slotIndex; i < itemSlots.size(); i++)
    {
        if (itemSlots[i])
        {
            itemSlots[i]->setVisible(false);
        }
    }
}

void Chest::showChestContents()
{
    if (!chestPanel)
    {
        chestPanel = LayerColor::create(Color4B(50, 50, 50, 200));
        chestPanel->setPosition(Vec2(100, 100));
        chestPanel->setContentSize(Size(600, 400));
        this->addChild(chestPanel, 25);
    }

    chestPanel->setVisible(true);
    updateChestUI();
}

void Chest::hideChestContents()
{
    if (chestPanel)
    {
        chestPanel->setVisible(false);
    }
}

void Chest::preloadChestPool()
{
    // 预加载常用物品到箱子池
    std::vector<Item::ItemType> commonTypes = {
        Item::ItemType::SEED,
        Item::ItemType::FISH,
        Item::ItemType::EGG,
        Item::ItemType::MILK,
        Item::ItemType::WOOL,
        Item::ItemType::BONE,
        Item::ItemType::WOODEN,
        Item::ItemType::FRUIT,
        Item::ItemType::MINERAL,
        Item::ItemType::GIFT,
        Item::ItemType::FAT};

    for (auto type : commonTypes)
    {
        for (int i = 0; i < 5; i++)
        {
            Item *item = itemFactory->createItem(type);
            if (item)
            {
                item->retain();
                item->setVisible(false);
                activeChestItems.push_back(item);
            }
        }
    }
}

void Chest::clearChest()
{
    // 回收所有存储的物品
    for (auto &pair : storedItems)
    {
        for (auto item : pair.second)
        {
            item->reset();
            activeChestItems.push_back(item);
        }
        pair.second.clear();
    }
    storedItems.clear();

    updateChestUI();
    CCLOG("Chest cleared");
}

size_t Chest::getChestSize() const
{
    size_t total = 0;
    for (const auto &pair : storedItems)
    {
        total += pair.second.size();
    }
    return total;
}

Item *Chest::getItemFromChestPool(Item::ItemType type)
{
    // 尝试从activeChestItems中找到相同类型的物品
    for (auto it = activeChestItems.begin(); it != activeChestItems.end(); ++it)
    {
        if ((*it)->getType() == type)
        {
            Item *item = *it;
            activeChestItems.erase(it);
            item->reset();
            item->type = type;
            item->quantity = 1;
            item->setVisible(true);
            return item;
        }
    }

    // 如果没有找到，创建新的
    return itemFactory->createItem(type);
}

void Chest::returnItemToChestPool(Item *item)
{
    if (item)
    {
        item->reset();
        activeChestItems.push_back(item);
    }
}

void Chest::initializeChestSlots()
{
    // 创建物品槽位
    for (int i = 0; i < 20; i++)
    { // 5x4 网格
        Sprite *slot = Sprite::create("../Resources/ui/item_slot.png");
        if (slot)
        {
            float x = 150 + (i % 5) * 80;
            float y = 350 - (i / 5) * 80;
            slot->setPosition(Vec2(x, y));
            slot->setVisible(false);
            itemSlots.push_back(slot);
        }
    }
}

void Chest::initializeToolGrids()
{
    // 创建工具槽位网格
    for (int i = 0; i < 28; i++)
    { // 7x4 网格
        Sprite *grid = Sprite::create("../Resources/ui/tool_grid.png");
        if (grid)
        {
            float x = 150 + (i % 7) * 60;
            float y = 350 - (i / 7) * 60;
            grid->setPosition(Vec2(x, y));
            grid->setVisible(false);
            toolgrids.push_back(grid);
        }
    }
}

void Chest::updateSlotDisplay(int slotIndex, Item *item)
{
    if (slotIndex >= 0 && slotIndex < itemSlots.size() && item)
    {
        Sprite *slot = itemSlots[slotIndex];
        if (slot)
        {
            // 显示物品图标
            slot->setTexture(item->getTexture());
            slot->setVisible(true);

            // 更新数量标签
            auto label = static_cast<Label *>(slot->getChildByTag(100));
            if (!label)
            {
                label = Label::createWithTTF("1", "../Resources/fonts/arial.ttf", 16);
                label->setColor(Color3B::WHITE);
                label->setPosition(Vec2(slot->getContentSize().width - 10, slot->getContentSize().height - 10));
                slot->addChild(label, 1, 100);
            }
            label->setString(StringUtils::format("%d", item->getQuantity()));
        }
    }
}