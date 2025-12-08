/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     toolManager.cpp
 * File Function: 实现toolManager类，使用对象池模式管理工具
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#include "Inventory/ToolManager.h"
#include "Inventory/ToolFactory.h"
#include "Inventory/Chest.h"
#include "Characters/Player.h"
#include "Scenes/GameScene.h"

USING_NS_CC;

ToolManager *ToolManager::instance = nullptr;

ToolManager::ToolManager() : selectedCharacter(1), nickname("guest"), chest(nullptr),
                             player(nullptr), selectionBox(nullptr), selectedToolIndex(-1)
{
    preloadToolPool();
    initKeyboardListener();
}

ToolManager *ToolManager::getInstance(int selectedCharacter, const std::string &nickname)
{
    if (instance == nullptr)
    {
        instance = new ToolManager();
        instance->selectedCharacter = selectedCharacter;
        instance->nickname = nickname;
    }
    return instance;
}

bool ToolManager::init(int selectedCharacter, const std::string &nickname)
{
    if (!Node::init())
    {
        return false;
    }

    this->selectedCharacter = selectedCharacter;
    this->nickname = nickname;
    this->chest = Chest::getInstance();
    this->player = Player::getInstance(selectedCharacter, nickname);

    return true;
}

ToolManager::~ToolManager()
{
    // 清理对象池
    for (auto tool : availablePool)
    {
        CC_SAFE_RELEASE(tool);
    }
    for (auto tool : activePool)
    {
        CC_SAFE_RELEASE(tool);
    }
    for (auto tool : tools)
    {
        CC_SAFE_RELEASE(tool);
    }
    availablePool.clear();
    activePool.clear();
    tools.clear();

    // 清理UI元素
    if (selectionBox)
    {
        selectionBox->removeFromParent();
    }
}

void ToolManager::preloadToolPool()
{
    // 预创建常用工具到对象池
    expandToolPool(Tool::ToolType::HOE, 3);
    expandToolPool(Tool::ToolType::AXE, 3);
    expandToolPool(Tool::ToolType::WATERING_CAN, 2);
    expandToolPool(Tool::ToolType::FISHING_ROD, 2);
    expandToolPool(Tool::ToolType::FERTILIZER, 2);
    expandToolPool(Tool::ToolType::HOEPLUS, 1);
    expandToolPool(Tool::ToolType::AXEPLUS, 1);
    expandToolPool(Tool::ToolType::ANIMALFOOD, 2);
}

void ToolManager::expandToolPool(Tool::ToolType type, int count)
{
    ToolFactory *factory = ToolFactory::getInstance();
    for (int i = 0; i < count; i++)
    {
        Tool *tool = factory->createTool(type);
        if (tool)
        {
            tool->retain();
            tool->setVisible(false);
            availablePool.push_back(tool);
        }
    }
}

Tool *ToolManager::getToolFromPool(Tool::ToolType type)
{
    // 尝试从可用池中找到相同类型的工具
    for (auto it = availablePool.begin(); it != availablePool.end(); ++it)
    {
        if ((*it)->getType() == type)
        {
            Tool *tool = *it;
            availablePool.erase(it);
            tool->reset();
            tool->type = type; // 恢复类型
            tool->setVisible(true);
            activePool.push_back(tool);
            return tool;
        }
    }

    // 如果没有可用的工具，扩展池
    expandToolPool(type, 3);
    return getToolFromPool(type);
}

void ToolManager::returnToolToPool(Tool *tool)
{
    // 从活动池中移除
    for (auto it = activePool.begin(); it != activePool.end(); ++it)
    {
        if (*it == tool)
        {
            activePool.erase(it);
            break;
        }
    }

    // 重置并返回到可用池
    tool->reset();
    availablePool.push_back(tool);
}

void ToolManager::addTool(Tool::ToolType type)
{
    Tool *tool = getToolFromPool(type);
    if (tool)
    {
        tools.push_back(tool);
        this->addChild(tool);

        // 设置位置
        int index = tools.size() - 1;
        float x = 50 + (index % 5) * 60;
        float y = 400 - (index / 5) * 60;
        tool->setPosition(Vec2(x, y));
    }
}

void ToolManager::recycleTool(Tool *tool)
{
    returnToolToPool(tool);

    // 从tools向量中移除
    for (auto it = tools.begin(); it != tools.end(); ++it)
    {
        if (*it == tool)
        {
            tools.erase(it);
            break;
        }
    }

    // 从场景移除
    if (tool->getParent())
    {
        tool->removeFromParent();
    }
}

void ToolManager::selectTool(int index)
{
    if (index >= 0 && index < tools.size())
    {
        selectedToolIndex = index;
        updateSelectionBox();
    }
}

void ToolManager::useTool()
{
    if (selectedToolIndex >= 0 && selectedToolIndex < tools.size())
    {
        Tool *tool = tools[selectedToolIndex];
        if (tool)
        {
            tool->usetool();

            // 如果需要，可以添加使用后的逻辑
            // 例如：耐久度减少等
        }
    }
}

void ToolManager::discardTool()
{
    if (selectedToolIndex >= 0 && selectedToolIndex < tools.size())
    {
        Tool *tool = tools[selectedToolIndex];
        recycleTool(tool);
        selectedToolIndex = -1;
        updateSelectionBox();
    }
}

void ToolManager::clearAllTools()
{
    // 回收所有工具
    for (auto tool : tools)
    {
        returnToolToPool(tool);
        if (tool->getParent())
        {
            tool->removeFromParent();
        }
    }
    tools.clear();
    selectedToolIndex = -1;
    updateSelectionBox();
}

void ToolManager::updateSelectionBox()
{
    if (!selectionBox)
    {
        selectionBox = Sprite::create("../Resources/ui/selection.png");
        if (selectionBox)
        {
            this->addChild(selectionBox, -1);
        }
    }

    if (selectedToolIndex >= 0 && selectedToolIndex < tools.size())
    {
        Tool *tool = tools[selectedToolIndex];
        if (tool && selectionBox)
        {
            selectionBox->setPosition(tool->getPosition());
            selectionBox->setVisible(true);
        }
    }
    else if (selectionBox)
    {
        selectionBox->setVisible(false);
    }
}

void ToolManager::initKeyboardListener()
{
    auto keyboardListener = EventListenerKeyboard::create();

    keyboardListener->onKeyPressed = [&](EventKeyboard::KeyCode keyCode, Event *event)
    {
        switch (keyCode)
        {
        case EventKeyboard::KeyCode::KEY_1:
            selectTool(0);
            break;
        case EventKeyboard::KeyCode::KEY_2:
            selectTool(1);
            break;
        case EventKeyboard::KeyCode::KEY_3:
            selectTool(2);
            break;
        case EventKeyboard::KeyCode::KEY_4:
            selectTool(3);
            break;
        case EventKeyboard::KeyCode::KEY_5:
            selectTool(4);
            break;
        case EventKeyboard::KeyCode::KEY_Q:
            useTool();
            break;
        case EventKeyboard::KeyCode::KEY_R:
            discardTool();
            break;
        default:
            break;
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}