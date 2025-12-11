#include "Inventory/ToolFactory.h"

ToolFactory *ToolFactory::instance = nullptr;

ToolFactory *ToolFactory::getInstance()
{
    if (instance == nullptr)
    {
        instance = new ToolFactory();
    }
    return instance;
}

ToolFactory::ToolFactory()
{
    initRegistry();
}

void ToolFactory::initRegistry()
{
    // 在这里统一管理所有工具的图片路径和价格

    // 基础工具 (假设基础工具价格为0)
    toolRegistry[Tool::ToolType::HOE] = {"../Resources/tools/HOE.png", 0};
    toolRegistry[Tool::ToolType::AXE] = {"../Resources/tools/AXE.png", 0};
    toolRegistry[Tool::ToolType::WATERING_CAN] = {"../Resources/tools/WATERING_CAN.png", 0};
    toolRegistry[Tool::ToolType::FISHING_ROD] = {"../Resources/tools/FISHING_ROD.png", 0};

    // 消耗品/其他
    toolRegistry[Tool::ToolType::FERTILIZER] = {"../Resources/tools/fertilizeruse.png", 50}; // 假设价格
    toolRegistry[Tool::ToolType::ANIMALFOOD] = {"../Resources/tools/Soup.png", 50};

    // 升级版工具
    toolRegistry[Tool::ToolType::HOEPLUS] = {"../Resources/tools/HOE++.png", 450};
    toolRegistry[Tool::ToolType::AXEPLUS] = {"../Resources/tools/AXE++.png", 500};
    toolRegistry[Tool::ToolType::WATERING_CANPLUS] = {"../Resources/tools/WATERING_CAN++.png", 300};
    toolRegistry[Tool::ToolType::FISHING_RODPLUS] = {"../Resources/tools/FISHING_ROD++.png", 600};
}

Tool *ToolFactory::createTool(Tool::ToolType type)
{
    Tool *tool = new (std::nothrow) Tool();
    if (tool && tool->init(type))
    {
        tool->autorelease();
        return tool;
    }
    CC_SAFE_DELETE(tool);
    return nullptr;
}

void ToolFactory::configureTool(Tool *tool, Tool::ToolType type)
{
    auto it = toolRegistry.find(type);
    if (it != toolRegistry.end())
    {
        const ToolData &data = it->second;
        tool->setTexture(data.texturePath);
        // 如果 Tool 类有 setPrice 方法，可以在这里调用，目前假设直接赋值或 Tool 没有存储 price 需求
        // tool->price = data.price;
    }
    else
    {
        CCLOG("ToolFactory: Warning, type not found in registry.");
    }
}