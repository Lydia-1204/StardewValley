#pragma once
#ifndef TOOL_H
#define TOOL_H

#include "cocos2d.h"

class Tool : public cocos2d::Sprite {
public:
    enum class ToolType {
        NONE,
        HOE, //��ͷ
        AXE, //��ͷ
        WATERING_CAN, //ˮ��
        FISHING_ROD, //���
        // ���������������
    };

    static Tool* create(ToolType type);
    bool init(ToolType type);
    ToolType getType() const;

private:
    ToolType type; // ��������
};

class ToolManager : public cocos2d::Node {
public:
    static ToolManager* create();
    virtual bool init();

    void addTool(Tool::ToolType type); // ��ӹ��ߵ�������
    void selectTool(int index);        // ѡ�й���
    void useTool();                    // ʹ��ѡ�еĹ���
    void discardTool();                // ����ѡ�еĹ���

private:
    std::vector<Tool*> tools;         // �������еĹ���
    int selectedToolIndex;            // ��ǰѡ�еĹ�������
    cocos2d::Sprite* selectionBox;    // ѡ�й��ߵĸ�����
    void updateSelectionBox();        // ����ѡ�п�λ��
};

#endif // TOOL_H