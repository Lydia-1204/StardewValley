#pragma once
#include "NpcTemplate.h"

class Shane : public NpcTemplate {
public:
    Shane() : NpcTemplate(NPCType::SHANE, Vec2(400, 100)) {}

    void addTaskDialogues() override {
        addDialogue("task", "Could you send a gift to [Abigail]? \nShe is around your home, I believe she will be happy!\n              $450");
        addDialogue("task", "Thank you! What a good act of kindness!\nHere is your reward: $450");
    }
    void taskFinishReward() override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+450);
        affection += 40;
    }

    static Shane* getInstance() {
        if (instance == nullptr) {
            instance = new (std::nothrow) Shane();
            if (instance && instance->init(NPCType::SHANE, Vec2(400, 100))) {
                instance->autorelease();
            } else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;
    }

private:
    static Shane* instance;
};
