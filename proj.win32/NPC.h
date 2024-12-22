/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     NPC.h
 * File Function: ����NPC���NPC��
 * Author:        ������ 2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#pragma once
#ifndef NPC_H
#define NPC_H

#include "cocos2d.h"

class NPC : public cocos2d::Sprite {
public:
    friend class Player;
    NPC() :_affection(0), _relationship("Empty") {}                        // ˽�л����캯��
    virtual ~NPC();

    enum class NPCType {
        ELLIOTT = 0,
        SAM,
        ABIGAIL,
        SHANE,
        // �������NPC����
    };
    // ��������NPC����
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
    void initialmove();  // ��ʼ��NPC���Զ��ƶ�
    void moveAlongPath(const std::vector<cocos2d::Vec2>& path);
    void update(float dt) override; // ����update����
    void updateDirection(float dt);
    cocos2d::Vec2 getNpcsPosition()   const;
    void removeDialogue(const std::string& category, const std::string& dialogue);
    const std::vector<std::string>& getDialogues(const std::string& category) const;
    const std::vector<NPC*>& NPC::getAllNPCs() const {
        return npcs; // npcs ��NPC�����vector
    }
    std::string name;
    int _affection;               // �øж�
    std::string _relationship;     // ��ǰ��ϵ
    cocos2d::Label* _affectionLabel; // ������ʾ�øжȵı�ǩ*************************************************************************
    cocos2d::Label* _relationshipLabel; // ������ʾ��ϵ�ı�ǩ**************************************************************************60 friends 90 romantic 

    // ��ӶԻ�����ķ���
    void addDialogue(const std::string& category, const std::string& dialogue); // ��ӷ���Ի�
    std::string getRandomDialogue(const std::string& category) const;          // �����ȡ����Ի�
    void displayDialogueBox(const std::string& category);

    // ���úͻ�ȡ�øж�
    void setAffection(int affection);
    int getAffection() const;

    // ���úͻ�ȡ��ǰ��ϵ
    void setRelationship(const std::string& relationship);
    std::string getRelationship() const;

    // ��ʾ�øжȿ�
    void showAffectionBox();

    bool isGift = 0;//֮������   ��player�����ǲ���gift

private:
    NPCType type; // NPC����
    static NPC* instance;
    std::vector<NPC*> npcs; // �洢�ĸ�NPCʵ��
    void createAndPositionNPCs(); // ����������NPC��λ��
    cocos2d::Vec2 lastPosition;

    // ������¼texture��Ԫ��С�ı�����������updateDirection��ʹ��
    float widthSegment = 17;
    float heightSegment = 32;
    // ʹ��unordered_map�洢����Ի�
    std::unordered_map<std::string, std::vector<std::string>> categorizedDialogues;

};

#endif // NPC_H