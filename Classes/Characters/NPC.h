/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     NPC.h
 * File Function: 定义NPC类和NPC类
 * Author:        邓语乐 2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#pragma once
#ifndef NPC_H
#define NPC_H

#include "cocos2d.h"

class NPC : public cocos2d::Sprite {
public:
    friend class Player;
    NPC() :_affection(0), _relationship("Empty") {}                        // 私有化构造函数
    virtual ~NPC();

    enum class NPCType {
        ELLIOTT = 0,
        SAM,
        ABIGAIL,
        SHANE,
        // 添加其他NPC类型
    };
    // 定义四种NPC类型
    std::vector<NPC::NPCType> npcTypes = {
    NPC::NPCType::ELLIOTT,//
    NPC::NPCType::SAM,//
    NPC::NPCType::ABIGAIL,//
    NPC::NPCType::SHANE,//
    };

    static NPC* getInstance();
    static NPC* create();
    virtual bool init();

    static NPC* create(NPCType type);
    bool init(NPCType type);
    NPCType getType() const;
    void initialmove();  // 初始化NPC的自动移动
    void moveAlongPath(const std::vector<cocos2d::Vec2>& path);
    void update(float dt) override; // 重载update函数
    void updateDirection(float dt);
    cocos2d::Vec2 getNpcsPosition()   const;
    void removeDialogue(const std::string& category, const std::string& dialogue);
    const std::vector<std::string>& getDialogues(const std::string& category) const;
    const std::vector<NPC*>& NPC::getAllNPCs() const {
        return npcs; // npcs 是NPC对象的vector
    }
    std::string name;
    int _affection;               // 好感度
    std::string _relationship;     // 当前关系
    cocos2d::Label* _affectionLabel; // 用于显示好感度的标签*************************************************************************
    cocos2d::Label* _relationshipLabel; // 用于显示关系的标签**************************************************************************60 friends 90 romantic 

    // 添加对话管理的方法
    void addDialogue(const std::string& category, const std::string& dialogue); // 添加分类对话
    std::string getRandomDialogue(const std::string& category) const;          // 随机获取分类对话
    void displayDialogueBox(const std::string& category);

    // 设置和获取好感度
    void setAffection(int affection);
    int getAffection() const;

    // 设置和获取当前关系
    void setRelationship(const std::string& relationship);
    std::string getRelationship() const;

    // 显示好感度框
    void showAffectionBox();

    bool isGift = 0;//之后完善   看player手上是不是gift

private:
    NPCType type; // NPC类型
    static NPC* instance;
    std::vector<NPC*> npcs; // 存储四个NPC实例
    void createAndPositionNPCs(); // 创建并设置NPC的位置
    cocos2d::Vec2 lastPosition;

    // 新增记录texture单元大小的变量，方便在updateDirection中使用
    float widthSegment = 17;
    float heightSegment = 32;
    // 使用unordered_map存储分类对话
    std::unordered_map<std::string, std::vector<std::string>> categorizedDialogues;

};

#endif // NPC_H
