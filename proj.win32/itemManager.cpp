/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     Item.cpp
 * File Function: ʵ��Item����Itemmanager��ʵ����Ʒ�Ĺ��� ����
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/15
 *********************************************************************************************************/


#include "Item.h"
#include "ItemManager.h"

USING_NS_CC;



ItemManager* ItemManager::instance = nullptr;


ItemManager* ItemManager::getInstance(int selectedCharacter, const std::string& nickname) {
    if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
        instance = new (std::nothrow) ItemManager();
        if (instance && instance->init(selectedCharacter, nickname)) {
            instance->autorelease();  // ��ӵ��ڴ����ϵͳ
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // ����Ψһʵ��
}





bool ItemManager::init(int _selectedCharacter, const std::string& _nickname) {
    if (!Node::init()) {
        return false;
    }
    selectedCharacter = _selectedCharacter;
    nickname = _nickname;
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ��Ʒ������
     /*
    auto ItemBarBg = Sprite::create("../ResourcesL/ooseSprites-73/DialogBoxGreen..png");
    ItemBarBg->setPosition(visibleSize.width / 2, visibleSize.height * 0.1);
    this->addChild(ItemBarBg);*/

    // ��ʼ����Ʒ��
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    float startX = (visibleSize.width - gridWidth * 10) / 2.0f; // ������Ʒ��
    float startY = visibleSize.height * 0.1f+32.0f;

    for (int i = 0; i < 10; i++) {
        auto grid = Sprite::create("../Resources/tools/tools_bg.png");
        grid->setPosition(startX + i * gridWidth, startY);
        this->addChild(grid);

        Items.push_back(nullptr); // ��ʼ����Ʒ��Ϊ��
    }

    // ѡ�п�
    selectionBox = Sprite::create("../Resources/tools/tools_selected.png");
    selectionBox->setVisible(false);
    this->addChild(selectionBox, 10);

    // ��ʼ��״̬
    selectedItemIndex = -1;

    // ����¼�������
   // ����¼�������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
        CCLOG("mouseDown");
        auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����
        auto locationInItemsBg = this->convertToNodeSpace(locationInWorld); // ת���� Items_bg ������ϵ
        CCLOG("Mouse position in Items_bg space: %f, %f", locationInItemsBg.x, locationInItemsBg.y);

        // ����Ƿ�����Ʒ

        CCLOG("Adjusted Mouse Position: %f, %f", locationInItemsBg.x, locationInItemsBg.y);

        // �ж�����Ƿ�����Ʒ��
        for (int i = 0; i < Items.size(); i++) {
            if (Items[i] && Items[i]->getBoundingBox().containsPoint(locationInItemsBg)) {
                if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT) {
                    CCLOG("Item[%d] selected and used", i);
                    selectItem(i);  // ѡ����Ʒ
                    useitem();     // ֱ��ʹ����Ʒ
                }
                else if (event->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT) {
                    discardItem();  // �Ҽ�������Ʒ
                }
                return;
            }
        }
        };

    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
    initKeyboardListener();
    return true;
}

void ItemManager::addItem(Item::ItemType type) {
   
    float gridWidth = 32.0f; // ��Ʒ�����
    float startX = (Director::getInstance()->getVisibleSize().width - gridWidth * 10) / 2.0f;
    float startY = Director::getInstance()->getVisibleSize().height * 0.1f + 32.0f;

    for (int i = 0; i < Items.size(); i++) {
        if (Items[i] != nullptr && Items[i]->getType() == type) {
            // �����Ʒ�����Ѿ���������͵���Ʒ����������
            Items[i]->increaseQuantity(1);
            return;
        }
        if (Items[i] == nullptr) {
            // ����п�λ����������Ʒ
            auto item = Item::create(type);
            item->setPosition(startX + i * gridWidth, startY);
            Items[i] = item;
            this->addChild(Items[i]);
            auto location = Items[i]->getPosition();
            CCLOG("Items [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }
  
    CCLOG("ItemBar is full, cannot add more Items.");
    auto fullLabel = Label::createWithTTF("ItemBar is full!", "fonts/Marker Felt.ttf", 24);
    fullLabel->setPosition(startX, startY+32.0f);
    this->addChild(fullLabel);
    fullLabel->runAction(Sequence::create(FadeOut::create(2.0f), RemoveSelf::create(), nullptr));
}

void ItemManager::discardItem() {
    if (selectedItemIndex < 0 || selectedItemIndex >= Items.size() || Items[selectedItemIndex] == nullptr) {
        CCLOG("No Item selected to discard");
        return;
    }

    auto Item = Items[selectedItemIndex];
    Item->decreaseQuantity(1);
    if (Item->getQuantity() <= 0) {

        this->removeChild(Item);
        Items[selectedItemIndex] = nullptr;
        selectedItemIndex = -1;
        selectionBox->setVisible(false);
    }
    
    CCLOG("Item discarded: %d", static_cast<int>(Item->getType()));

}
void ItemManager::selectItem(int index) {
    if (index < 0 || index >= Items.size() || Items[index] == nullptr) {
        CCLOG("Invalid Item selection");
        return;
    }

    selectedItemIndex = index;
    updateSelectionBox();
    CCLOG("Item selected: %d", index);
}

void ItemManager::useitem() {
    if (selectedItemIndex < 0 || selectedItemIndex >= Items.size() || Items[selectedItemIndex] == nullptr) {
        CCLOG("No Item selected to use");
        return;
    }

    auto Item = Items[selectedItemIndex];
    CCLOG("Using Item: %d", static_cast<int>(Item->getType()));
    // �����Ʒʹ���߼�
    Item->useitem();
}



void ItemManager::updateSelectionBox() {
    if (selectedItemIndex < 0 || selectedItemIndex >= Items.size() || Items[selectedItemIndex] == nullptr) {
        selectionBox->setVisible(false);
        return;
    }
    selectionBox->setVisible(true);
    selectionBox->setPosition(Items[selectedItemIndex]->getPosition());
}

void ItemManager::initKeyboardListener() {
    auto keyboardListener = EventListenerKeyboard::create();
    keyboardListener->onKeyPressed = [=](EventKeyboard::KeyCode keyCode, Event* event) {
        switch (keyCode) {
        case EventKeyboard::KeyCode::KEY_C:
            useitem(); // ʹ�õ�ǰ��Ʒ
            break;
        case EventKeyboard::KeyCode::KEY_V:
            discardItem(); // ������ǰ��Ʒ
            break;
        default:
            break;
        }
        };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(keyboardListener, this);
}