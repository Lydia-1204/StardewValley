#pragma once
#include "NpcTemplate.h"

class Elliott : public NpcTemplate {
public:
    Elliott() : NpcTemplate(NPCType::ELLIOTT, Vec2(70, 300)) {}

    void addTaskDialogues() override {
        addDialogue("task", "Could you plant a crop for me?\n I need it for a recipe.\n                   $350");
        addDialogue("task", "Thank you so much! I can't wait to see this crop blossom!\nHere is your reward: $350");
    }
    void taskFinishReward() override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+350);
        affection += 40;
    }

    static Elliott* getInstance() {
        if (instance == nullptr) {
            instance = new (std::nothrow) Elliott();
            if (instance && instance->init(NPCType::ELLIOTT, Vec2(70, 300))) {
                instance->autorelease();
            } else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;
    }

private:
    static Elliott* instance;
};
