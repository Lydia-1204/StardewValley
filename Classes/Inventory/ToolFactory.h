#pragma once
#ifndef TOOLFACTORY_H
#define TOOLFACTORY_H

#include "cocos2d.h"
#include "Inventory/Tool.h"
#include <map>
#include <string>

// 定义工具的基本数据结构
struct ToolData
{
    std::string texturePath;
    int price;
};

class ToolFactory
{
public:
    static ToolFactory *getInstance();

    // 创建一个新的 Tool 对象
    Tool *createTool(Tool::ToolType type);

    // 配置现有的 Tool 对象
    void configureTool(Tool *tool, Tool::ToolType type);

private:
    ToolFactory();
    static ToolFactory *instance;
    std::map<Tool::ToolType, ToolData> toolRegistry;

    void initRegistry(); // 初始化注册表
};

#endif // TOOLFACTORY_H