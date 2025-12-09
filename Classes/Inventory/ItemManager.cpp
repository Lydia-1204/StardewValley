/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     ItemManager.cpp
 * File Function: 实现Item类与Itemmanager (对象池模式，修复警告)
 * Author:        王小萌 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/

#include "Inventory/Item.h"
#include "Inventory/ItemManager.h"
#include "Inventory/ItemFactory.h"
#include "World/Map.h"
#include "Inventory/Chest.h"
#include "Systems/UIManager.h"

USING_NS_CC;

ItemManager *ItemManager::instance = nullptr;

ItemManager *ItemManager::getInstance(int selectedCharacter, const std::string &nickname)
{
    if (instance == nullptr)
    {
        instance = new (std::nothrow) ItemManager();
        if (instance && instance->init(selectedCharacter, nickname))
        {
            instance->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;
}

bool ItemManager::init(int _selectedCharacter, const std::string &_nickname)
{
    if (!Node::init())
    {
        return false;
    }
    selectedCharacter = _selectedCharacter;
    nickname = _nickname;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    float startX = (visibleSize.width - gridWidth * 10) / 2.0f;
    float startY = visibleSize.height * 0.1f + 32.0f;

    for (int i = 0; i < 12; i++)
    {
        auto grid = Sprite::create("../Resources/tools/tools_bg.png");
        grid->setPosition(startX + i * gridWidth, startY);
        this->addChild(grid);

        Items.push_back(nullptr);
    }

    selectionBox = Sprite::create("../Resources/tools/tools_selected.png");
    selectionBox->setVisible(false);
    this->addChild(selectionBox, 10);

    selectedItemIndex = -1;
    chest = Chest::getInstance();

    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse *event)
    {
        auto locationInWorld = event->getLocationInView();
        auto locationInItemsBg = this->convertToNodeSpace(locationInWorld);

        int x = 0;
        for (int i = 0; i < (int)Items.size(); i++)
        {
            if (Items[i] && Items[i]->getBoundingBox().containsPoint(locationInItemsBg))
            {
                if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
                {
                    CCLOG("Item[%d] selected and used", i);
                    selectItem(i);
                    if (chest->isOpen == 1)
                    {
                        chest->addItem(Items[i]);
                        discardItem();
                    }
                    else if (UIManager::getInstance(x, "")->isPriceBoardOpen == 1)
                    {
                        if (Items[i]->getType() != Item::ItemType::GIFT)
                        {
                            Items[i]->decreaseQuantity(1);
                            UIManager::getInstance(x, "")->setMoney(Items[i]->price);
                            if (Items[i]->getQuantity() <= 0)
                            {
                                discardItem();
                            }
                        }
                    }
                    else
                        useitem();
                }
                else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
                {
                    discardItem();
                }
                return;
            }
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    initKeyboardListener();
    return true;
}

// ---------------------- 对象池逻辑实现 ----------------------

Item *ItemManager::getPooledItem(Item::ItemType type)
{
    if (!itemPool.empty())
    {
        Item *item = itemPool.back();
        itemPool.pop_back();

        item->reset(type);
        item->autorelease();

        CCLOG("ItemManager: Retrieved item from pool. Pool size: %d", (int)itemPool.size());
        return item;
    }
    else
    {
        CCLOG("ItemManager: Creating new item (pool empty).");
        return ItemFactory::getInstance()->createItem(type);
    }
}

void ItemManager::returnItemToPool(Item *item)
{
    if (!item)
        return;

    item->retain();
    item->removeFromParent();
    itemPool.push_back(item);

    CCLOG("ItemManager: Item returned to pool. Pool size: %d", (int)itemPool.size());
}

// -----------------------------------------------------------

int ItemManager::getItemQuantity(Item::ItemType type)
{
    for (auto item : Items)
    {
        if (item && item->getType() == type)
        {
            return item->getQuantity();
        }
    }
    return 0;
}

void ItemManager::addItem(Item::ItemType type)
{

    float gridWidth = 32.0f;
    float startX = (Director::getInstance()->getVisibleSize().width - gridWidth * 10) / 2.0f;
    float startY = Director::getInstance()->getVisibleSize().height * 0.1f + 32.0f;

    for (int i = 0; i < (int)Items.size(); i++)
    {
        if (Items[i] != nullptr && Items[i]->getType() == type)
        {
            Items[i]->increaseQuantity(1);
            if (type == Item::ItemType::MINERAL && Items[i]->getQuantity() >= 30)
            {
                Items[i]->price = 60;
            }
            else if (type == Item::ItemType::MINERAL && Items[i]->getQuantity() < 30)
            {
                Items[i]->price = 80;
            }

            return;
        }
        if (Items[i] == nullptr)
        {
            auto item = getPooledItem(type);

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
    fullLabel->setPosition(startX, startY + 32.0f);
    this->addChild(fullLabel);
    fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
}

void ItemManager::discardItem()
{
    if (selectedItemIndex < 0 || selectedItemIndex >= (int)Items.size() || Items[selectedItemIndex] == nullptr)
    {
        CCLOG("No Item selected to discard");
        return;
    }

    auto Item = Items[selectedItemIndex];
    Item->decreaseQuantity(1);
    if (Item->getQuantity() <= 0)
    {
        returnItemToPool(Item);

        Items[selectedItemIndex] = nullptr;
        selectedItemIndex = -1;
        selectionBox->setVisible(false);
    }

    CCLOG("Item discarded");
}

void ItemManager::selectItem(int index)
{
    if (index < 0 || index >= (int)Items.size() || Items[index] == nullptr)
    {
        CCLOG("Invalid Item selection");
        return;
    }

    selectedItemIndex = index;
    updateSelectionBox();
    CCLOG("Item selected: %d", index);
}

void ItemManager::useitem()
{
    if (selectedItemIndex < 0 || selectedItemIndex >= (int)Items.size() || Items[selectedItemIndex] == nullptr)
    {
        CCLOG("No Item selected to use");
        return;
    }

    auto Item = Items[selectedItemIndex];
    CCLOG("Using Item: %d", static_cast<int>(Item->getType()));
    Item->useitem();
}

void ItemManager::updateSelectionBox()
{
    if (selectedItemIndex < 0 || selectedItemIndex >= (int)Items.size() || Items[selectedItemIndex] == nullptr)
    {
        selectionBox->setVisible(false);
        return;
    }
    selectionBox->setVisible(true);
    selectionBox->setPosition(Items[selectedItemIndex]->getPosition());
}

void ItemManager::initKeyboardListener()
{
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event *event)
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_C:
            useitem();
            break;
        case EventKeyboard::KeyCode::KEY_V:
            discardItem();
            break;
        default:
            break;
        }
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}