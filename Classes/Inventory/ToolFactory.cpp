#include "Inventory/ToolFactory.h"
#include "Inventory/Tool.h"

USING_NS_CC;

// 静态成员初始化
ToolFactory *ToolFactory::_instance = nullptr;

/**
 * 获取工厂单例实例
 * 使用懒汉式单例模式，确保全局只有一个工厂实例
 * @return ToolFactory* 工厂实例指针
 */
ToolFactory *ToolFactory::getInstance()
{
    if (_instance == nullptr)
    {
        _instance = new ToolFactory();
    }
    return _instance;
}

/**
 * 构造函数
 * 私有构造函数支持单例模式，初始化时注册所有工具创建器
 */
ToolFactory::ToolFactory()
{
    registerCreators();
}

/**
 * 析构函数
 * 清理工厂资源
 */
ToolFactory::~ToolFactory()
{
    cleanup();
}

/**
 * 注册所有工具类型的创建函数
 * 使用 lambda 表达式为每种工具类型创建对应的创建函数
 * 每个创建函数负责创建、初始化和内存管理
 */
void ToolFactory::registerCreators()
{
    // 注册锄头创建器
    _creators[Tool::ToolType::HOE] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::HOE))
        {
            tool->autorelease(); // 交由 cocos2d-x 内存管理
            return tool;
        }
        CC_SAFE_DELETE(tool); // 创建失败时安全删除
        return nullptr;
    };

    // 注册斧头创建器
    _creators[Tool::ToolType::AXE] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::AXE))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册水壶创建器
    _creators[Tool::ToolType::WATERING_CAN] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::WATERING_CAN))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册鱼竿创建器
    _creators[Tool::ToolType::FISHING_ROD] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::FISHING_ROD))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册化肥创建器
    _creators[Tool::ToolType::FERTILIZER] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::FERTILIZER))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册升级锄头创建器
    _creators[Tool::ToolType::HOEPLUS] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::HOEPLUS))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册升级斧头创建器
    _creators[Tool::ToolType::AXEPLUS] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::AXEPLUS))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册动物饲料创建器
    _creators[Tool::ToolType::ANIMALFOOD] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::ANIMALFOOD))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };

    // 注册空类型创建器（用于初始化）
    _creators[Tool::ToolType::NONE] = []() -> Tool *
    {
        Tool *tool = new Tool();
        if (tool && tool->init(Tool::ToolType::NONE))
        {
            tool->autorelease();
            return tool;
        }
        CC_SAFE_DELETE(tool);
        return nullptr;
    };
}

/**
 * 创建指定类型的工具
 * 工厂模式的核心方法，通过查找注册的创建函数来创建工具
 * @param type 工具类型枚举
 * @return Tool* 创建的工具实例，失败返回 nullptr
 */
Tool *ToolFactory::createTool(Tool::ToolType type)
{
    // 在创建函数映射表中查找对应的创建函数
    auto it = _creators.find(type);
    if (it != _creators.end())
    {
        return it->second(); // 执行创建函数
    }
    // 未找到对应的工具类型，输出错误日志
    CCLOG("ToolFactory: Unknown tool type %d", static_cast<int>(type));
    return nullptr;
}

/**
 * 清理工厂资源
 * 清空创建函数映射表并删除单例实例
 */
void ToolFactory::cleanup()
{
    _creators.clear(); // 清空创建函数映射表
    if (_instance)
    {
        delete _instance;
        _instance = nullptr; // 重置单例指针
    }
}