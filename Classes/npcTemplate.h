#pragma once
#ifndef NPCTEMPLATE_H
#define NPCTEMPLATE_H

#include "cocos2d.h"
#include "Constant.h"
#include"UIManager.h"

class Player;

enum class NPCType {
        ELLIOTT = 0,
        SAM,
        ABIGAIL,
        SHANE,
        // 添加其他NPC类型
    };


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
    cocos2d::Label* affectionLabel; // 用于显示好感度的标签*************************************************************************
    cocos2d::Label* relationshipLabel; // 用于显示关系的标签**************************************************************************60 friends 90 romantic 

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
    virtual void taskFinishReward();    //存放对应npc完成任务后的奖励


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

};


class Shane : public NpcTemplate {
public:
    Shane() : NpcTemplate(NPCType::SHANE, Vec2(400, 100)) {} // 名为Shane的NPC

    void addTaskDialogues() override {
        // 添加Shane任务的对话
        addDialogue("task", "Could you send a gift to [Abigail]? \nShe is around your home, I believe she will be happy!\n              $450");
        addDialogue("task", "Thank you! What a good act of kindness!\nHere is your reward: $450");
    }

    void taskFinishReward()override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+450);
        affection += 40;
    }
    /*
     static Shane* create(NPCType npcType = NPCType::SHANE, Vec2 startPoint = Vec2(400, 100)) {
        // 使用基类的 create 函数进行初始化
        Shane* npc = dynamic_cast<Shane*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // 在子类中进行额外的初始化，如果需要的话
            // 例如：npc->setPosition(startPoint); // 如果在这里需要再次设置位置
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    }   
    
    */

    static Shane* getInstance() {
        if (instance == nullptr) {  // 如果实例不存在，则创建
            instance = new (std::nothrow) Shane();
            if (instance && instance->init(NPCType::SHANE, Vec2(400, 100))) {
                instance->autorelease();  // 添加到内存管理系统
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // 返回唯一实例
    }

private:
    static Shane* instance;  // 静态实例

};


class Elliott : public NpcTemplate {
public:
    Elliott() :NpcTemplate(NPCType::ELLIOTT, Vec2(70, 300)) {}		//名为Elliott的npc

    void addTaskDialogues()override {
        // 添加Elliott任务的对话
        addDialogue("task", "Could you plant a crop for me?\n I need it for a recipe.\n                   $350");
        addDialogue("task", "Thank you so much! I can't wait to see this crop blossom!\nHere is your reward: $350");
    }
    void taskFinishReward()override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+350);
        affection += 40;
    }

    /*
    static Elliott* create(NPCType npcType = NPCType::ELLIOTT, Vec2 startPoint = Vec2(100, 100)) {
        // 使用基类的 create 函数进行初始化
        Elliott* npc = dynamic_cast<Elliott*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // 在子类中进行额外的初始化，如果需要的话
            // 例如：npc->setPosition(startPoint); // 如果在这里需要再次设置位置
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    }    
    */


    static Elliott* getInstance() {
        if (instance == nullptr) {  // 如果实例不存在，则创建
            instance = new (std::nothrow) Elliott();
            if (instance && instance->init(NPCType::ELLIOTT, Vec2(70, 300))) {
                instance->autorelease();  // 添加到内存管理系统
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // 返回唯一实例
    }

private:
    static Elliott* instance;  // 静态实例
};


class Sam : public NpcTemplate {
public:
    Sam() : NpcTemplate(NPCType::SAM, Vec2(200, 100)) {} // 名为Sam的NPC

    void addTaskDialogues()override {
        // 添加Sam任务的对话
        addDialogue("task", "The mines hold many secrets. \nCan you explore them and bring back something interesting?\n                      $1000");
        addDialogue("task", "Wow! Thanks! Your adventure of exploring the mines must be fun!\nHere is your reward: $1000");
    }
    void taskFinishReward()override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+1000);
        affection += 40;
    }

    /*
       static Sam* create(NPCType npcType = NPCType::SAM, Vec2 startPoint = Vec2(200, 100)) {
        // 使用基类的 create 函数进行初始化
        Sam* npc = dynamic_cast<Sam*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // 在子类中进行额外的初始化，如果需要的话
            // 例如：npc->setPosition(startPoint); // 如果在这里需要再次设置位置
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    } 
    */


    static Sam* getInstance() {
        if (instance == nullptr) {  // 如果实例不存在，则创建
            instance = new (std::nothrow) Sam();
            if (instance && instance->init(NPCType::SAM, Vec2(200, 100))) {
                instance->autorelease();  // 添加到内存管理系统
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // 返回唯一实例
    }

private:
    static Sam* instance;  // 静态实例

};


class Abigail : public NpcTemplate {
public:
    Abigail() : NpcTemplate(NPCType::ABIGAIL, Vec2(300, 300)) {} // 名为Abigail的NPC

    void addTaskDialogues()override {
        // 添加Abigail任务的对话
        addDialogue("task", "Your tools are looking a bit worn. \nMaybe it's time to buy an upgrade?\n                              $100");
        addDialogue("task", "Congratulations! I believe you can now work better with these upgraded tools!\nHere is your reward: $100");;
    }
    void taskFinishReward()override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+100);
        affection += 40;
    }

    /*
    static Abigail* create(NPCType npcType = NPCType::ABIGAIL, Vec2 startPoint = Vec2(300, 100)) {
        // 使用基类的 create 函数进行初始化
        Abigail* npc = dynamic_cast<Abigail*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // 在子类中进行额外的初始化，如果需要的话
            // 例如：npc->setPosition(startPoint); // 如果在这里需要再次设置位置
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    }    
    */



    static Abigail* getInstance() {
        if (instance == nullptr) {  // 如果实例不存在，则创建
            instance = new (std::nothrow) Abigail();
            if (instance && instance->init(NPCType::ABIGAIL, Vec2(300, 300))) {
                instance->autorelease();  // 添加到内存管理系统
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // 返回唯一实例
    }

private:
    static Abigail* instance;  // 静态实例

};


#endif