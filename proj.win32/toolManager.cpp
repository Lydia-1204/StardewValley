/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: ʵ��tool����toolmanager��ʵ�ֹ��ߵĹ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/
#include "toolManager.h"
#include"Player.h"
#include"Chest.h"
#include "map.h"
#include"UIManager.h"
#include"itemManager.h"
USING_NS_CC;

ToolManager* ToolManager::instance = nullptr;


// ----------------------------- ToolManager -----------------------------

ToolManager* ToolManager::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
        instance = new (std::nothrow) ToolManager();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();  // ��ӵ��ڴ����ϵͳ
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // ����Ψһʵ��
}





bool ToolManager::init(int _selectedCharacter, const std::string& _nickname) {
    if (!Node::init()) {
        return false;
    }
    chest = Chest::getInstance();
    selectedCharacter = _selectedCharacter;
    nickname = _nickname;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ����������
     /*
    auto toolBarBg = Sprite::create("../ResourcesL/ooseSprites-73/DialogBoxGreen..png");
    toolBarBg->setPosition(visibleSize.width / 2, visibleSize.height * 0.1);
    this->addChild(toolBarBg);*/

    // ��ʼ��������
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    float startX = (visibleSize.width - gridWidth * 10) / 2.0f; // ���й�����
    float startY = visibleSize.height * 0.1f;

    for (int i = 0; i < 12; i++) {
        auto grid = Sprite::create("../Resources/tools/tools_bg.png");
        grid->setPosition(startX + i * gridWidth, startY);
        this->addChild(grid);

        tools.push_back(nullptr); // ��ʼ��������Ϊ��
    }

    // ѡ�п�
    selectionBox = Sprite::create("../Resources/tools/tools_selected.png");
    selectionBox->setVisible(false);
    this->addChild(selectionBox, 10);

    // ��ʼ��״̬
    selectedToolIndex = -1;


    // ����¼�������
   // ����¼�������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
    //    CCLOG("mouseDown");
        auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����
        auto locationInToolsBg = this->convertToNodeSpace(locationInWorld); // ת���� tools_bg ������ϵ
      //  CCLOG("Mouse position in tools_bg space: %f, %f", locationInToolsBg.x, locationInToolsBg.y);

        // ����Ƿ�������

        // �ж�����Ƿ���������-------------------------------------------
        for (int i = 0; i < tools.size(); i++) {
            if (tools[i] && tools[i]->getBoundingBox().containsPoint(locationInToolsBg)) {
                if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                    CCLOG("Tool[%d] selected and used", i);
                    selectTool(i);  // ѡ�й���

                    if (chest->isOpen == 1) {//�����ӽ���
                        chest->addTool(tools[i]);
                        discardTool();
                    }
                    else
                        useTool();  // ֱ��ʹ�ù���
                }
                else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                    discardTool();  // �Ҽ���������
                }
                return;
            }
        }
        //�򹤾�------------------------------------------------------------------
        int x=1;
        if (locationInWorld.distance(Vec2(270,425)) < 20 ) {
            if (UIManager::getInstance(x,"")->getMoney() >= 450) {
                UIManager::getInstance(x, "")->setMoney(-450);
                this->addTool(Tool::ToolType::HOEPLUS);
            }
            
              else {
                  auto fullLabel = Label::createWithTTF("money not enough!", "fonts/Marker Felt.ttf", 24);
                  fullLabel->setPosition(startX, startY + 64.0f);
                  this->addChild(fullLabel);
                  fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
              }
        }
        else if (locationInWorld.distance(Vec2(270,376)) < 20 ) {
            if (UIManager::getInstance(x, "")->getMoney() >= 600) {
                UIManager::getInstance(x, "")->setMoney(-600);
                this->addTool(Tool::ToolType::FISHING_RODPLUS);
            }
            
            else {
                auto fullLabel = Label::createWithTTF("money not enough!", "fonts/Marker Felt.ttf", 24);
                fullLabel->setPosition(startX, startY + 64.0f);
                this->addChild(fullLabel);
                fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
            }
        }
        else if (locationInWorld.distance(Vec2(270, 316)) < 20) {
            if (UIManager::getInstance(x, "")->getMoney() >= 500) {
                UIManager::getInstance(x, "")->setMoney(-500);
                this->addTool(Tool::ToolType::AXEPLUS);
            }
            
            else {
                auto fullLabel = Label::createWithTTF("money not enough!", "fonts/Marker Felt.ttf", 24);
                fullLabel->setPosition(startX, startY + 64.0f);
                this->addChild(fullLabel);
                fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
            }
        }
        else if (locationInWorld.distance(Vec2(270, 265)) < 20 ) {
            if (UIManager::getInstance(x, "")->getMoney() >= 300) {
                UIManager::getInstance(x, "")->setMoney(-300);
                this->addTool(Tool::ToolType::WATERING_CANPLUS);
            }
            
             else {
                 auto fullLabel = Label::createWithTTF("money not enough!", "fonts/Marker Felt.ttf", 24);
                 fullLabel->setPosition(startX, startY + 64.0f);
                 this->addChild(fullLabel);
                 fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
             }
        }
        else if (locationInWorld.distance(Vec2(270, 206)) < 20 ) {
            if (UIManager::getInstance(x, "")->getMoney() >= 300) {
                UIManager::getInstance(x, "")->setMoney(-300);
                ItemManager::getInstance(1, " ")->addItem(Item::ItemType::GIFT);
            }
            
              else {
                  auto fullLabel = Label::createWithTTF("money not enough!", "fonts/Marker Felt.ttf", 24);
                  fullLabel->setPosition(startX, startY + 64.0f);
                  this->addChild(fullLabel);
                  fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
              }
        }
    };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    initKeyboardListener();
    return true;

}

void ToolManager::addTool(Tool::ToolType type) {
    float gridWidth = 32.0f;//���������
    float startX = (Director::getInstance()->getVisibleSize().width - gridWidth * 10) / 2.0f;
    float startY = Director::getInstance()->getVisibleSize().height * 0.1f;
    for (int i = 0; i < tools.size(); i++) {
        if (tools[i] == nullptr) {
            auto tool = Tool::create(type);


            tool->setPosition(startX + i * gridWidth, startY);
            tools[i] = tool;
            this->addChild(tools[i]);
            auto location = tools[i]->getPosition();
            CCLOG("tools [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }
    CCLOG("ToolBar is full, cannot add more tools.");
    auto fullLabel = Label::createWithTTF("ToolBar is full!", "fonts/Marker Felt.ttf", 24);
    fullLabel->setPosition(startX, startY + 64.0f);
    this->addChild(fullLabel);
    fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
}


void ToolManager::selectTool(int index) {
    if (index < 0 || index >= tools.size() || tools[index] == nullptr) {
        CCLOG("Invalid tool selection");
        return;
    }

    selectedToolIndex = index;
    updateSelectionBox();
    CCLOG("Tool selected: %d", index);
}

void ToolManager::useTool() {
    if (selectedToolIndex < 0 || selectedToolIndex >= tools.size() || tools[selectedToolIndex] == nullptr) {
        CCLOG("No tool selected to use");
        return;
    }

    auto tool = tools[selectedToolIndex];
    CCLOG("Using tool: %d", static_cast<int>(tool->getType()));
    // ��ӹ���ʹ���߼�
    tool->usetool();
}

void ToolManager::discardTool() {
    if (selectedToolIndex < 0 || selectedToolIndex >= tools.size() || tools[selectedToolIndex] == nullptr) {
        CCLOG("No tool selected to discard");
        return;
    }

    auto tool = tools[selectedToolIndex];
    this->removeChild(tool);
    tools[selectedToolIndex] = nullptr;
    selectedToolIndex = -1;
    selectionBox->setVisible(false);
    CCLOG("Tool discarded: %d", static_cast<int>(tool->getType()));

}

void ToolManager::updateSelectionBox() {
    if (selectedToolIndex < 0 || selectedToolIndex >= tools.size() || tools[selectedToolIndex] == nullptr) {
        selectionBox->setVisible(false);
        return;
    }
    selectionBox->setVisible(true);
    selectionBox->setPosition(tools[selectedToolIndex]->getPosition());
}

void ToolManager::initKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_C:
            useTool(); // ʹ�õ�ǰ����
            break;
        case EventKeyboard::KeyCode::KEY_V:
            discardTool(); // ������ǰ����
            break;
        default:
            break;
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}
