#include "map.h"
USING_NS_CC;

MapManager* MapManager::instance = nullptr;

MapManager::MapManager() : currentMap(nullptr), playerStartPos(Vec2::ZERO), selectedCharacter(0) {}

MapManager* MapManager::getInstance() {
    if (!instance) {
        instance = MapManager::create();
    }
    return instance;
}

bool MapManager::init() {
    if (!Node::init()) {
        return false;
    }
    return true;
}

void MapManager::loadMapBlocks(const std::string& mapFolder) {
    // 加载四个地图块
  
    mapPool["upperLeft"] = TMXTiledMap::create("../Resources/map_block1.tmx");
    mapPool["upperRight"] = TMXTiledMap::create("../Resources/map_block2.tmx");
    mapPool["lowerLeft"] = TMXTiledMap::create("../Resources/map_block3.tmx");
    mapPool["lowerRight"] = TMXTiledMap::create("../Resources/map_block4.tmx");
    
    // 默认加载左上地图块
    currentMap = mapPool["upperRight"]; 

    if (!currentMap) {
        CCLOG("Error:Faled to load map.tmx");
        throw std::runtime_error("upperLeftMap created failed!!");
    }
    else  
        this->addChild(currentMap);
     
    //背景尺寸
    const Size spriteSize = currentMap->getContentSize();

    // 计算宽度和高度的缩放比例
    auto screenSize = Director::getInstance()->getVisibleSize();
    float scaleX = screenSize.width / spriteSize.width;
    float scaleY = screenSize.height / spriteSize.height;

    // 选择更大的缩放比例，确保图片覆盖整个屏幕
    float scale = std::max(scaleX, scaleY);
    currentMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 ));
    currentMap->setScale(scale); // 根据需要调整比例
    currentMap->setAnchorPoint(Vec2(0.5,0.5)); // 设置锚点
   
    CCLOG("currentMap : %p ", currentMap);
    playerStartPos = Vec2(16 * 5, 16 * 5); // 设置默认玩家初始位置
}

TMXTiledMap* MapManager::getCurrentBlock() {
    if (!currentMap)
        throw std::runtime_error("failed to load currentMap!!");
    CCLOG("currentMap : %p ", currentMap);
    return currentMap;
}

Vec2 MapManager::getPlayerStartPos() {
    return playerStartPos;
}

void MapManager::switchToBlock(const Vec2& direction) {
    CCLOG("currentMap : %p ", currentMap);
    // 根据方向切换地图块

    if (!currentMap) {
        CCLOG("Error: currentMap is nullptr");
        return;
    }

    // 移除当前地图块，保留内存
    this->removeChild(currentMap, false);
    currentMap = nullptr;


    CCLOG("currentMap : %p ", currentMap);
    // 根据方向切换地图块
    if (direction == Vec2(-1, 0)) { // 左边界
        currentMap = mapPool["upperLeft"];
    }
    else if (direction == Vec2(1, 0)) { // 右边界
        currentMap = mapPool["upperRight"];
    }
    else if (direction == Vec2(0, -1)) { // 下边界
        currentMap = mapPool["lowerLeft"];
    }
    else if (direction == Vec2(0, 1)) { // 上边界
        currentMap = mapPool["lowerRight"];
    }

    // 检查切换后的地图块是否有效
    if (!currentMap) {
        CCLOG("Error: Failed to switch map block");
        return;
    }


    CCLOG("currentMap : %p ", currentMap);
    /* 确保没有其他父节点
    if (currentMap&&currentMap->getParent()) {
        currentMap->removeFromParent();
    }*/
//    currentMap->removeFromParent();
    // 添加新地图块
    this->addChild(currentMap);
    CCLOG("Switched to new map block: %p", currentMap);
    // 重置玩家初始位置（假设玩家总是从新地图的中心进入）
   const Size mapSize = currentMap->getMapSize();
   const Size tileSize = currentMap->getTileSize();
    playerStartPos = Vec2(mapSize.width * tileSize.width / 2, mapSize.height * tileSize.height / 2);
}

bool MapManager::isAtEdge(const Vec2& playerPos, Vec2& outDirection) {
    // 获取当前地图的尺寸

    if (!currentMap) {
        CCLOG("error : currentMap is nullptr in isAtEdge");
        return false;
    }
    else {
        CCLOG("currentMap : %p ",currentMap);
    }
    const Size mapSize = currentMap->getMapSize();      // 格子数
    const Size tileSize = currentMap->getTileSize();    // 每个格子的大小
    const float mapWidth = mapSize.width * tileSize.width;
   const  float mapHeight = mapSize.height * tileSize.height;

    // 检测玩家是否到达边界
    if (playerPos.x <= 0) {
        outDirection = Vec2(-1, 0); // 左边界
        return true;
    }
    else if (playerPos.x >= mapWidth) {
        outDirection = Vec2(1, 0); // 右边界
        return true;
    }
    else if (playerPos.y <= 0) {
        outDirection = Vec2(0, -1); // 下边界
        return true;
    }
    else if (playerPos.y >= mapHeight) {
        outDirection = Vec2(0, 1); // 上边界
        return true;
    }

    return false;
}