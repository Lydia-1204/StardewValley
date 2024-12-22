/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:    item.h
 * File Function: ʵ��Chest��ʵ�֣�ʵ�ֹ�����ĵĴ���ʹ��
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#include "Chest.h"
#include"toolManager.h"
#include"itemManager.h"

USING_NS_CC;
Chest* Chest::instance = nullptr;


Chest* Chest::getInstance( ) {
    if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
        instance = new (std::nothrow) Chest();
        if (instance && instance->init( )) {
            instance->autorelease();  // ��ӵ��ڴ����ϵͳ
            instance->setupItemsAndTools();  // ��ʼ��������Ʒ�͹���
            instance->initMouseListener();//�����¼�
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // ����Ψһʵ��
}



void Chest::setupItemsAndTools() {
    // ��ȡ��Ļ�Ŀɼ��ߴ�
    auto visibleSize = Director::getInstance()->getVisibleSize();

    // ���㱳������Ĵ�С��Ϊ��Ļ�İ˷�֮һ
    auto bgWidth = visibleSize.width / 5.6f;
    auto bgHeight = visibleSize.height / 5.6f;

    // ����ÿ�����ӵĿ��
    float gridWidth = 32.0f;
    float gridHeight = 32.0f;
    // ������Ʒ����������
    itemBarBg = Sprite::create("../Resources/tools/Chest_bg..png");
    itemBarBg->setScaleX(bgWidth / itemBarBg->getContentSize().width);
    itemBarBg->setScaleY(bgHeight / itemBarBg->getContentSize().height);

    Size mapSize = MapManager::getInstance()->getCurrentMapSize(1);
    itemBarBg->setPosition(mapSize.width / 2, 0);
    //itemBarBg->setPosition(Vec2(visibleSize.width / 2 - bgWidth / 2, visibleSize.height / 2));
    this->addChild(itemBarBg, 1);

    // ������������������
    toolBarBg = Sprite::create("../Resources/tools/Chest_bg..png");
    toolBarBg->setScaleX(bgWidth / toolBarBg->getContentSize().width);
    toolBarBg->setScaleY(bgHeight / toolBarBg->getContentSize().height);
    toolBarBg->setPosition(mapSize.width / 2+ gridWidth*7, 0);
    this->addChild(toolBarBg, 1);
    //���߸�
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 7; j++) {
            itemgrids[i][j] = Sprite::create("../Resources/tools/tools_bg.png");
            itemgrids[i][j]->setPosition(itemBarBg->getPositionX() - bgWidth / 2 + 16 + j * gridWidth,
                itemBarBg->getPositionY() + bgHeight / 2 - gridWidth - i * gridWidth + 16);
            this->addChild(itemgrids[i][j], 2);
        }
    // ��ʼ����Ʒ�͹��ߣ�����������Ӧ�ı���������
    for (int i = 0; i < 7; i++) {
        Item* item;
        switch (i) {
        case 0: item = Item::create(Item::ItemType::SEED); break;
        case 1: item = Item::create(Item::ItemType::FISH); break;
        case 2: item = Item::create(Item::ItemType::FRUIT); break;
        case 3: item = Item::create(Item::ItemType::BONE); break;
        case 4: item = Item::create(Item::ItemType::WOOL); break;
        case 5: item = Item::create(Item::ItemType::WOODEN); break;
        case 6: item = Item::create(Item::ItemType::EGG); break;
        }
        chestItems.push_back(item);
        // ���ݱ��������λ�ú͸��ӵĴ�С������Ʒ��λ��
        auto itemPosition = Vec2(itemBarBg->getPositionX() - bgWidth / 2 + 16 + i * gridWidth,
            itemBarBg->getPositionY() + bgHeight / 2 - gridWidth + 16);
        item->setPosition(itemPosition);
        this->addChild(item, 3);
        addItem(item);
    }


    //���߸�
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 7; j++) {
            toolgrids[i][j] = Sprite::create("../Resources/tools/tools_bg.png");

            toolgrids[i][j]->setPosition(toolBarBg->getPositionX() - bgWidth / 2 + 16 + j * gridWidth,
                toolBarBg->getPositionY() + bgHeight / 2 - gridWidth - i * gridWidth + 16);
            this->addChild(toolgrids[i][j], 2);
        }
    // ��ʼ������
    for (int i = 0; i < 4; i++) {
        Tool* tool;
        switch (i) {
        case 0: tool = Tool::create(Tool::ToolType::HOE); break;
        case 1: tool = Tool::create(Tool::ToolType::AXE); break;
        case 2: tool = Tool::create(Tool::ToolType::WATERING_CAN); break;
        case 3: tool = Tool::create(Tool::ToolType::FISHING_ROD); break;
        }
        chestTools.push_back(tool);
        auto toolPosition = Vec2(toolBarBg->getPositionX() - bgWidth / 2 + 16 + i * gridWidth,
            toolBarBg->getPositionY() + bgHeight / 2 - gridWidth + 16);
        tool->setPosition(toolPosition);
        this->addChild(tool, 2);
    }
    //��ǩ
    itemLabel = Label::createWithSystemFont("Items", "../Resources/fonts/Marker Felt.ttf", 20);
    itemLabel->setTextColor(Color4B::WHITE);
    itemLabel->setPosition(Vec2(itemBarBg->getPositionX(), itemBarBg->getPositionY()-32.0*2));  // ����λ��
    this->addChild(itemLabel, 4);

    // ��������ǩ
   toolLabel = Label::createWithSystemFont("Tools", "../Resources/fonts/Marker Felt.ttf", 20);
    toolLabel->setTextColor(Color4B::WHITE);
    toolLabel->setPosition(Vec2(toolBarBg->getPositionX() , toolBarBg->getPositionY() - 32.0 * 2));  // ����λ��
    this->addChild(toolLabel, 4);

    this->setVisible(false);
}


void Chest::showItemsAndTools() {
    // ��ʾ�����е���Ʒ
    for (auto& item : chestItems) {
        item->setVisible(true);  // ��ʾ��Ʒ
        itemBarBg->setVisible(true);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 7; j++)
                itemgrids[i][j]->setVisible(true);
    }
    for (auto& tool : chestTools) {
        tool->setVisible(true);  // ��ʾ����
        toolBarBg->setVisible(true);
        for (int i = 0; i < 4; i++)
            for (int j = 0; j < 7; j++)
               toolgrids[i][j]->setVisible(true);
    }
}

// ��ʼ�������������
void Chest::initMouseListener() {
    // �����������
    auto mouseListener = EventListenerMouse::create();
    mouseListener->onMouseDown = [=](EventMouse* event) {
        auto locationInWorld = event->getLocationInView();  // ��ȡ��Ļ��ͼ�е�����
        auto locationInChest = this->convertToNodeSpace(locationInWorld);  // ת�������ӽڵ������ϵ

        // ����Ƿ�������Ʒ����
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 7; j++) {
                if (itemgrids[i][j]->getBoundingBox().containsPoint(locationInChest)) {
                    if (i * 7 + j < chestItems.size()) {  // ����ø�������Ʒ
                        onItemClicked(chestItems[i * 7 + j]);  // ������Ʒ����¼�
                    }
                    return;
                }
            }
        }

        // ����Ƿ����˹��߸���
        for (int i = 0; i < 4; i++) {
            for (int j = 0; j < 7; j++) {
                if (toolgrids[i][j]->getBoundingBox().containsPoint(locationInChest)) {
                    if (i * 7 + j < chestTools.size()) {  // ����ø����й���
                        onToolClicked(chestTools[i * 7 + j]);  // �������ߵ���¼�
                    }
                    return;
                }
            }
        }
        };
    //*******�رհ�ť**********//
    auto closeItem = MenuItemImage::create(
        "../Resources/closeChest.png", // ����״̬
        "../Resources/closeChest.png", // �����״̬
        CC_CALLBACK_1(Chest::closeChest, this)); // ��ť����¼�


    closeItem->setPosition(Vec2(toolgrids[0][6]->getPositionX() + 10.0f, toolgrids[0][6]->getPositionY() + 10.0f));  // ���������Ͻ�
    auto menu = Menu::create(closeItem, nullptr);
    menu->setPosition(Vec2::ZERO);  // ���ò˵�λ��
    this->addChild(menu, 5);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(mouseListener, this);
}

void Chest::addItem(Item* item) {
    for (int i = 0; i < 28; i++) {
        if (i< chestItems.size()&&chestItems[i] && chestItems[i]->getType() == item-> getType()) {
            // �����Ʒ�����Ѿ���������͵���Ʒ����������
            chestItems[i]->increaseQuantity(1);
            return;
        }
        if (i == chestItems.size()) {
            // ����п�λ����������Ʒ
            auto itemnew = Item::create(item->getType());
            int x = i / 7;
            int y = i % 7;
            itemnew->setPosition(itemgrids[x][y]->getPosition());
            chestItems.push_back( itemnew);
            this->addChild(chestItems[i],2);
            auto location = chestItems[i]->getPosition();
            CCLOG("Items [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }
}
void Chest::addTool(Tool* tool) {
  
    for (int i = 0; i < 28; i++) {
        if (i==chestTools.size()) {
            auto newtool = Tool::create(tool->getType());
            int x = i / 7;
            int y = i % 7;
            chestTools.push_back(newtool);
            chestTools[i]->setPosition(toolgrids[x][y]->getPosition());
            this->addChild(chestTools[i],2);
            //chestTools[i]->setVisible(true);
            auto location = chestTools[i]->getPosition();
            CCLOG("tools [%d]: %f,%f", i, location.x, location.y);
            return;
        }
    }
   
}


// ��Ʒ����¼�
void Chest::onItemClicked(Item* item) {
   
    
    if (item->getQuantity() > 0)
            ItemManager::getInstance(1, "nickname")->addItem(item->getType());
    // ��������ȡ����Ʒ�����������Ʒ��
        item->decreaseQuantity(1);  // ������Ʒ����
        if (item->getQuantity() == 0) {
            item->setVisible(false);
        }
    // ����Ʒ��ӵ������Ʒ��
     
    CCLOG("Item added to player inventory");
}

// ���ߵ���¼�
void Chest::onToolClicked(Tool* tool) {

    tool->setVisible(false);  // ���ٹ�������
    // ��������ӵ���ҵĹ�����
        ToolManager::getInstance(1, "nickname")->addTool(tool->getType());
    CCLOG("Tool added to player toolbar");
}

void Chest::chestSetPosition(const Vec2& position) {
    this->setPosition(position);
}
/*
void Chest::closeChest() {
    if (isOpen) {
        isOpen = false;
        CCLOG("Chest closed");
        // ���������е���Ʒ�͹���
        for (auto& item : chestItems) {
            item->setVisible(false);
            itemBarBg->setVisible(false);
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 4; j++)
                    itemgrids[i][j]->setVisible(false);
        }
        for (auto& tool : chestTools) {
            tool->setVisible(false);
            toolBarBg->setVisible(false);
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 4; j++)
                    toolgrids[i][j]->setVisible(false);
        }
    }
}
*/

void Chest::openChest() {
    if (!isOpen) {
        isOpen = true;
        CCLOG("Chest opened");
        this->setVisible(true);
        //showItemsAndTools();  // ��ʾ��Ʒ�͹���
    }
}

void Chest::closeChest(Ref* sender) {
    if (isOpen) {
        isOpen = false;
        CCLOG("Chest closed");
        this->setVisible(false);
        /*
        // ���������е���Ʒ�͹���
        for (auto& item : chestItems) {
            item->setVisible(false);
            itemBarBg->setVisible(false);
            for (int i = 0; i < 4; i++)
                for (int j = 0; j < 7; j++)
                    itemgrids[i][j]->setVisible(false);
        }
        for (auto& tool : chestTools) {
            tool->setVisible(false);
            toolBarBg->setVisible(false);
            for (int i = 0; i < 4; i++)
                for (int j = 0; j <7; j++)
                    toolgrids[i][j]->setVisible(false);
        }
        */
       // itemLabel->setVisible(false);
       // toolLabel->setVisible(false);
    }
}