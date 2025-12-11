#pragma once
#ifndef NPCTEMPLATE_H
#define NPCTEMPLATE_H

#include "cocos2d.h"
#include "App/Constant.h"
#include "Systems/UIManager.h"
#include <memory>

class Player;

enum class NPCType {
        ELLIOTT = 0,
        SAM,
        ABIGAIL,
        SHANE,
        // 添加其他NPC类型
    };


class NpcMoveStrategy;

class NpcTemplate : public cocos2d::Sprite {
public:
	friend class Player;

    NpcTemplate(NPCType npcType, Vec2 startPoint) :
        affection(0), relationship("Empty"), type(npcType), movePathStartPoint(startPoint) {}


	virtual ~NpcTemplate();
    cocos2d::Vec2 getNpcsPosition()   const;

	//virtual bool init();
    //static NpcTemplate* create();

    bool init(NPCType type, Vec2 startPoint);
    //virtual NpcTemplate* create();

    NPCType getType() const;

    // 创建并返回四个不同子类实例
    //NpcTemplate* getNpcInstance(NPCType npcType, Vec2 startPoint);
    //static NpcTemplate* getInstance();
	
    //static NpcTemplate* create(NPCType npcType, Vec2 startPoint);

    void update(float dt) override; // 重载update函数
    void updateDirection(float dt);
    void initialmove();  // 初始化NPC的自动移动

    void moveAlongPath(const std::vector<cocos2d::Vec2>& path);

    void setMoveStrategy(std::shared_ptr<NpcMoveStrategy> strategy);
    void usePatrolStrategy(float segmentSeconds = 3.5f);
    void useStandStillStrategy();

    // 设置和获取好感度
    void setAffection(int affection);
    int getAffection() const;


    // 设置和获取当前关系
    void setRelationship(const std::string& relationship);
    std::string getRelationship() const;

    // 显示好感度框
    void showAffectionBox();


    void removeDialogues(const std::string& category);
    std::string getDialogue(const std::string& category, int index);    //根据 category 和 index 提供指定的对话
    //各npc自行添加具体负责的任务对话
    //virtual void addTaskDialogues();    
     std::vector<NpcTemplate*> getAllNPCs() { return npcInstances; } // npcs 是NPC对象的vector  



    std::string name;
    int affection;               // 好感度
    std::string relationship;     // 当前关系
    cocos2d::Label* affectionLabel; // 用于显示好感度的标签
    cocos2d::Label* relationshipLabel; // 用于显示关系的标签 60 friends 90 romantic

    void increaseAffection(int aff) {
        affection += aff;
    }

    // 添加对话管理的方法
    void addDialogue(const std::string& category, const std::string& dialogue); // 添加分类对话
    std::string getRandomDialogue() ;          // 随机获取general/relationship对话
    void displayDialogueBox(int index);
    void closeDialogueBox();

    virtual void addTaskDialogues() = 0;

    bool haveChatBefore = false; //是否已发生过对话
    void haveChat() {   //更改是否已对话状态
        haveChatBefore = true;
    }
    bool haveFinishTask = false; //是否已完成该npc的任务
    void taskFinish();  //更改任务完成状态
    virtual void taskFinishReward() = 0;    //存放对应npc完成任务后的奖励


    bool isOpenDialogueBox = false;

    bool isGift = 0;//之后完善   看player手上是不是gift

    NPCType type; // NPC类型
    static NpcTemplate* instance;
    Vec2 movePathStartPoint;


    //static std::unordered_map<NPCType, NpcTemplate*> npcInstances; // 存储四个NPC实例

    std::vector<NpcTemplate*> npcInstances;

    void addGeneralDialogue();
private:
    void createAndPositionNPCs(); // 创建并设置NPC的位置
    cocos2d::Vec2 lastPosition;

    // 新增记录texture单元大小的变量，方便在updateDirection中使用
    float widthSegment = 17;
    float heightSegment = 32;
    // 使用unordered_map存储分类对话
    std::unordered_map<std::string, std::vector<std::string>> categorizedDialogues;
    bool friendshipInjected = false;
    bool romanticInjected = false;
    std::shared_ptr<NpcMoveStrategy> moveStrategy;

};

/* --------------------------------------------------------------------------
 * 【使用策略模式重构】
 * 说明：将 NPC 的移动行为抽离为策略接口 NpcMoveStrategy，通过不同策略（巡逻、静止等）实现可插拔移动逻辑；
 *       NpcTemplate 持有并分发策略，避免在基类硬编码路径与行为，降低耦合、便于扩展。
 * -------------------------------------------------------------------------- */
class NpcMoveStrategy {
public:
    virtual ~NpcMoveStrategy() = default;
    virtual void apply(NpcTemplate* npc) = 0;
};




#endif
