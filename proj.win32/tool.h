#pragma once
#ifndef TOOL_H
#define TOOL_H

#include "cocos2d.h"

class Tool : public cocos2d::Sprite {
public:
    enum class ToolType {
        NONE,
        HOE, //锄头
        AXE, //斧头
        WATERING_CAN, //水壶
        FISHING_ROD, //鱼竿
        // 添加其他工具类型
    };

    static Tool* create(ToolType type);
    bool init(ToolType type);
    ToolType getType() const;

private:
    ToolType type; // 工具类型
};

class ToolManager : public cocos2d::Node {
public:
    static ToolManager* create();
    virtual bool init();

    void addTool(Tool::ToolType type); // 添加工具到工具栏
    void selectTool(int index);        // 选中工具
    void useTool();                    // 使用选中的工具
    void discardTool();                // 丢弃选中的工具

private:
    std::vector<Tool*> tools;         // 工具栏中的工具
    int selectedToolIndex;            // 当前选中的工具索引
    cocos2d::Sprite* selectionBox;    // 选中工具的高亮框
    void updateSelectionBox();        // 更新选中框位置
};

#endif // TOOL_H