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
        // �������NPC����
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

    // �����������ĸ���ͬ����ʵ��
    //NpcTemplate* getNpcInstance(NPCType npcType, Vec2 startPoint);
    //static NpcTemplate* getInstance();
	
    //static NpcTemplate* create(NPCType npcType, Vec2 startPoint);

    void update(float dt) override; // ����update����
    void updateDirection(float dt);
    void initialmove();  // ��ʼ��NPC���Զ��ƶ�

    void moveAlongPath(const std::vector<cocos2d::Vec2>& path);

    // ���úͻ�ȡ�øж�
    void setAffection(int affection);
    int getAffection() const;


    // ���úͻ�ȡ��ǰ��ϵ
    void setRelationship(const std::string& relationship);
    std::string getRelationship() const;

    // ��ʾ�øжȿ�
    void showAffectionBox();


    void removeDialogues(const std::string& category);
    std::string getDialogue(const std::string& category, int index);    //���� category �� index �ṩָ���ĶԻ�
    //��npc������Ӿ��帺�������Ի�
    //virtual void addTaskDialogues();    
     std::vector<NpcTemplate*> getAllNPCs() { return npcInstances; } // npcs ��NPC�����vector  



    std::string name;
    int affection;               // �øж�
    std::string relationship;     // ��ǰ��ϵ
    cocos2d::Label* affectionLabel; // ������ʾ�øжȵı�ǩ*************************************************************************
    cocos2d::Label* relationshipLabel; // ������ʾ��ϵ�ı�ǩ**************************************************************************60 friends 90 romantic 

    void increaseAffection(int aff) {
        affection += aff;
    }

    // ��ӶԻ�����ķ���
    void addDialogue(const std::string& category, const std::string& dialogue); // ��ӷ���Ի�
    std::string getRandomDialogue() ;          // �����ȡgeneral/relationship�Ի�
    void displayDialogueBox(int index);
    void closeDialogueBox();

    virtual void addTaskDialogues() = 0;

    bool haveChatBefore = false; //�Ƿ��ѷ������Ի�
    void haveChat() {   //�����Ƿ��ѶԻ�״̬
        haveChatBefore = true;
    }
    bool haveFinishTask = false; //�Ƿ�����ɸ�npc������
    void taskFinish();  //�����������״̬
    virtual void taskFinishReward();    //��Ŷ�Ӧnpc��������Ľ���


    bool isOpenDialogueBox = false;

    bool isGift = 0;//֮������   ��player�����ǲ���gift

    NPCType type; // NPC����
    static NpcTemplate* instance;
    Vec2 movePathStartPoint;


    //static std::unordered_map<NPCType, NpcTemplate*> npcInstances; // �洢�ĸ�NPCʵ��

    std::vector<NpcTemplate*> npcInstances;

    void addGeneralDialogue();
private:

    void createAndPositionNPCs(); // ����������NPC��λ��
    cocos2d::Vec2 lastPosition;

    // ������¼texture��Ԫ��С�ı�����������updateDirection��ʹ��
    float widthSegment = 17;
    float heightSegment = 32;
    // ʹ��unordered_map�洢����Ի�
    std::unordered_map<std::string, std::vector<std::string>> categorizedDialogues;

};


class Shane : public NpcTemplate {
public:
    Shane() : NpcTemplate(NPCType::SHANE, Vec2(400, 100)) {} // ��ΪShane��NPC

    void addTaskDialogues() override {
        // ���Shane����ĶԻ�
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
        // ʹ�û���� create �������г�ʼ��
        Shane* npc = dynamic_cast<Shane*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // �������н��ж���ĳ�ʼ���������Ҫ�Ļ�
            // ���磺npc->setPosition(startPoint); // �����������Ҫ�ٴ�����λ��
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    }   
    
    */

    static Shane* getInstance() {
        if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
            instance = new (std::nothrow) Shane();
            if (instance && instance->init(NPCType::SHANE, Vec2(400, 100))) {
                instance->autorelease();  // ��ӵ��ڴ����ϵͳ
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // ����Ψһʵ��
    }

private:
    static Shane* instance;  // ��̬ʵ��

};


class Elliott : public NpcTemplate {
public:
    Elliott() :NpcTemplate(NPCType::ELLIOTT, Vec2(70, 300)) {}		//��ΪElliott��npc

    void addTaskDialogues()override {
        // ���Elliott����ĶԻ�
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
        // ʹ�û���� create �������г�ʼ��
        Elliott* npc = dynamic_cast<Elliott*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // �������н��ж���ĳ�ʼ���������Ҫ�Ļ�
            // ���磺npc->setPosition(startPoint); // �����������Ҫ�ٴ�����λ��
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    }    
    */


    static Elliott* getInstance() {
        if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
            instance = new (std::nothrow) Elliott();
            if (instance && instance->init(NPCType::ELLIOTT, Vec2(70, 300))) {
                instance->autorelease();  // ��ӵ��ڴ����ϵͳ
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // ����Ψһʵ��
    }

private:
    static Elliott* instance;  // ��̬ʵ��
};


class Sam : public NpcTemplate {
public:
    Sam() : NpcTemplate(NPCType::SAM, Vec2(200, 100)) {} // ��ΪSam��NPC

    void addTaskDialogues()override {
        // ���Sam����ĶԻ�
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
        // ʹ�û���� create �������г�ʼ��
        Sam* npc = dynamic_cast<Sam*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // �������н��ж���ĳ�ʼ���������Ҫ�Ļ�
            // ���磺npc->setPosition(startPoint); // �����������Ҫ�ٴ�����λ��
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    } 
    */


    static Sam* getInstance() {
        if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
            instance = new (std::nothrow) Sam();
            if (instance && instance->init(NPCType::SAM, Vec2(200, 100))) {
                instance->autorelease();  // ��ӵ��ڴ����ϵͳ
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // ����Ψһʵ��
    }

private:
    static Sam* instance;  // ��̬ʵ��

};


class Abigail : public NpcTemplate {
public:
    Abigail() : NpcTemplate(NPCType::ABIGAIL, Vec2(300, 300)) {} // ��ΪAbigail��NPC

    void addTaskDialogues()override {
        // ���Abigail����ĶԻ�
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
        // ʹ�û���� create �������г�ʼ��
        Abigail* npc = dynamic_cast<Abigail*>(NpcTemplate::create(npcType, startPoint));

        if (npc) {
            // �������н��ж���ĳ�ʼ���������Ҫ�Ļ�
            // ���磺npc->setPosition(startPoint); // �����������Ҫ�ٴ�����λ��
            //npc->setPosition(Vec2(100, 100));
        }

        return npc;
    }    
    */



    static Abigail* getInstance() {
        if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
            instance = new (std::nothrow) Abigail();
            if (instance && instance->init(NPCType::ABIGAIL, Vec2(300, 300))) {
                instance->autorelease();  // ��ӵ��ڴ����ϵͳ
            }
            else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;  // ����Ψһʵ��
    }

private:
    static Abigail* instance;  // ��̬ʵ��

};


#endif