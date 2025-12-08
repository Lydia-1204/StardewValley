#pragma once
#ifndef TOOLFACTORY_H
#define TOOLFACTORY_H

#include "Tool.h"
#include <functional>
#include <unordered_map>

/**
 * ToolFactory 类 - 工具工厂
 *
 * 使用工厂模式创建工具实例，通过注册创建函数消除 switch-case 语句
 * 支持单例模式，确保全局只有一个工厂实例
 */
class ToolFactory
{
public:
    /**
     * 获取工厂单例实例
     * @return ToolFactory* 工厂实例指针
     */
    static ToolFactory *getInstance();

    /**
     * 创建指定类型的工具
     * @param type 工具类型枚举
     * @return Tool* 创建的工具实例，失败返回 nullptr
     */
    Tool *createTool(Tool::ToolType type);

    /**
     * 清理工厂资源，释放内存
     */
    void cleanup();

private:
    /**
     * 私有构造函数，支持单例模式
     */
    ToolFactory();

    /**
     * 析构函数
     */
    ~ToolFactory();

    /**
     * 禁用拷贝构造函数
     */
    ToolFactory(const ToolFactory &) = delete;

    /**
     * 禁用赋值操作符
     */
    ToolFactory &operator=(const ToolFactory &) = delete;

    /**
     * 静态单例实例指针
     */
    static ToolFactory *_instance;

    /**
     * 注册所有工具类型的创建函数
     */
    void registerCreators();

    /**
     * 创建函数映射表，存储每种工具类型对应的创建函数
     */
    std::unordered_map<Tool::ToolType, std::function<Tool *()>> _creators;
};

#endif // TOOLFACTORY_H