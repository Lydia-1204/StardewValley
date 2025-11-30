/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     tool.cpp
 * File Function: 实现tool类，实现工具的管理 升级
 * Author:        王小萌 2351882
 * Update Date:   2024/12/21
 *********************************************************************************************************/
#include "Inventory/Tool.h"

#include "World/Map.h"
#include "Characters/Player.h"
#include "Inventory/ItemManager.h"

USING_NS_CC;

 
Tool* Tool::create(ToolType type) {
    Tool* tool = new (std::nothrow) Tool();
    if (tool && tool->init(type)) {
        tool->autorelease();
        return tool;
    }
    CC_SAFE_DELETE(tool);
    return nullptr;
}

bool Tool::init(ToolType type) {
    if (!Sprite::init()) {
        return false;
    }

    this->type = type;

    // 根据工具类型加载不同的资源
    switch (type) {
    case ToolType::HOE:
        this->initWithFile("../Resources/tools/HOE.png");
        break;
    case ToolType::AXE:
        this->initWithFile("../Resources/tools/AXE.png");
        break;
    case ToolType::WATERING_CAN:
        this->initWithFile("../Resources/tools/WATERING_CAN.png");
        break;
    case ToolType::FISHING_ROD:
        this->initWithFile("../Resources/tools/FISHING_ROD.png");
        break;
    case ToolType::FERTILIZER:
        this->initWithFile("../Resources/tools/fertilizeruse.png");
        break;
    case ToolType::ANIMALFOOD:
        this->initWithFile("../Resources/tools/Soup.png");
        break;
        //*********升级版***********//
    case ToolType::HOEPLUS:
        this->initWithFile("../Resources/tools/HOE++.png");
        break;
    case ToolType::AXEPLUS:
        this->initWithFile("../Resources/tools/AXE++.png");
        break;
    case ToolType::WATERING_CANPLUS:
        this->initWithFile("../Resources/tools/WATERING_CAN++.png");
        break;
    case ToolType::FISHING_RODPLUS:
        this->initWithFile("../Resources/tools/FISHING_ROD++.png");
        break;
    
   
    }
    return true;
}

Tool::ToolType Tool::getType() const {
    return type;
}




void Tool::usetool() {
    auto playerPos = Player::getInstance(selectedCharacter, nickname)->getPosition();
    auto direction = Player::getInstance(selectedCharacter, nickname)->_currentDirection;
    Vec2 dstPos;
    switch (direction) {

    case 0://下
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 1://右
        dstPos = Vec2(playerPos.x + 16, playerPos.y);
        break;
    case 2://上
        dstPos = Vec2(playerPos.x, playerPos.y - 16);
        break;
    case 3://左
        dstPos = Vec2(playerPos.x - 16, playerPos.y);
        break;
    }
   
    switch (type) {

   case ToolType::HOEPLUS:
        CCLOG("Using HOE++: Chopping a tree...");
    case ToolType::HOE: // 锄头功能：挖坑
        CCLOG("Using HOE: Digging a hole...");

        if (MapManager::getInstance()->getCurrentBlockLabel() == 7) { //挖矿
            if (playerPos.x < 600 && playerPos.x>460 && playerPos.y < 400 && playerPos.y>300) {
                ItemManager::getInstance(selectedCharacter, nickname)->addItem(Item::ItemType::MINERAL);
                Player::getInstance(selectedCharacter, nickname)->changeMining();
            }
        }
        break;

    case ToolType::AXEPLUS:
        CCLOG("Using AXE++: Chopping a tree...");
    case ToolType::AXE: // 斧头功能：砍树
        CCLOG("Using AXE: Chopping a tree...");
        if (MapManager::getInstance()->getCurrentBlockLabel() == 2) { // 砍树
            if (playerPos.distance(Vec2(810, 465)) < 50 || playerPos.distance(Vec2(875, 630)) < 50
                || playerPos.distance(Vec2(900, 385)) < 50 || playerPos.distance(Vec2(950, 540)) < 50
                || playerPos.distance(Vec2(1050, 440)) < 50 || playerPos.distance(Vec2(1100, 610)) < 50
                || playerPos.distance(Vec2(1170, 365)) < 50) {
                ItemManager::getInstance(selectedCharacter, nickname)->addItem(Item::ItemType::WOODEN);
            }
        }
        break;
    
  case ToolType::WATERING_CANPLUS:
        CCLOG("Using WATERING_CAN++: Chopping a tree...");
    case ToolType::WATERING_CAN: // 水壶功能：浇水
        Player::getInstance(selectedCharacter, nickname)->changePlanting();
        CCLOG("Using WATERING_CAN: Watering the crops...");     
        break;

  case ToolType::FISHING_RODPLUS: // 鱼竿功能：钓鱼
      CCLOG("Using FISHING_ROD++: Fishing...");
    case ToolType::FISHING_ROD: // 鱼竿功能：钓鱼
        CCLOG("Using FISHING_ROD: Fishing...");   
        if (MapManager::getInstance()->getCurrentBlockLabel() == 4) { // 钓鱼
            if (playerPos.distance(Vec2(1100, 110)) < 100) {
                ItemManager::getInstance(selectedCharacter, nickname)->addItem(Item::ItemType::FISH);
                Player::getInstance(selectedCharacter, nickname)->changeFishing();
            }
        }
        break;
    

    case ToolType::FERTILIZER: //
        CCLOG("Using FERTILIZER: Chopping a tree...");
        break;
    case ToolType::ANIMALFOOD: //
        Player::getInstance(selectedCharacter, nickname)->changeBreeding();
        CCLOG("Using ANIMALFOOD: Chopping a tree...");
        break;
    default:
        CCLOG("Invalid tool type or tool not equipped.");
        break;
    }

}
void Tool::buytool() {

    ;
}
