#pragma once
#include "NpcTemplate.h"

class Abigail : public NpcTemplate {
public:
    Abigail() : NpcTemplate(NPCType::ABIGAIL, Vec2(300, 300)) {}

    void addTaskDialogues() override {
        addDialogue("task", "Your tools are looking a bit worn. \nMaybe it's time to buy an upgrade?\n                              $100");
        addDialogue("task", "Congratulations! I believe you can now work better with these upgraded tools!\nHere is your reward: $100");
    }
    void taskFinishReward() override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+100);
        affection += 40;
    }

    static Abigail* getInstance() {
        if (instance == nullptr) {
            instance = new (std::nothrow) Abigail();
            if (instance && instance->init(NPCType::ABIGAIL, Vec2(300, 300))) {
                instance->autorelease();
            } else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;
    }

private:
    static Abigail* instance;
};
