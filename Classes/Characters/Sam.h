#pragma once
#include "NpcTemplate.h"

class Sam : public NpcTemplate {
public:
    Sam() : NpcTemplate(NPCType::SAM, Vec2(200, 100)) {}

    void addTaskDialogues() override {
        addDialogue("task", "The mines hold many secrets. \nCan you explore them and bring back something interesting?\n                      $1000");
        addDialogue("task", "Wow! Thanks! Your adventure of exploring the mines must be fun!\nHere is your reward: $1000");
    }
    void taskFinishReward() override {
        int x = 1;
        UIManager::getInstance(x, "")->setMoney(+1000);
        affection += 40;
    }

    static Sam* getInstance() {
        if (instance == nullptr) {
            instance = new (std::nothrow) Sam();
            if (instance && instance->init(NPCType::SAM, Vec2(200, 100))) {
                instance->autorelease();
            } else {
                CC_SAFE_DELETE(instance);
            }
        }
        return instance;
    }

private:
    static Sam* instance;
};

