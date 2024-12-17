#include "map.h"
USING_NS_CC;

MapManager* MapManager::instance = nullptr;

MapManager::MapManager() : currentMap(nullptr), playerStartPos(Vec2::ZERO), selectedCharacter(0) {}

MapManager* MapManager::getInstance() {
  
    if (instance == nullptr) {  // 如果实例不存在，则创建
        instance = new (std::nothrow) MapManager();
        if (instance && instance->init()) {
            instance->autorelease();  // 添加到内存管理系统
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // 返回唯一实例
}

bool MapManager::init() {
    if (!Node::init()) {
        return false;
    }
    return true;
}

void MapManager::loadMapBlocks(const std::string& mapFolder) {
    // 加载四个地图块
  
    mapPool["upperLeft"] = TMXTiledMap::create("map_block1.tmx");
    mapPool["upperRight"] = TMXTiledMap::create("map_block2.tmx");
    mapPool["lowerLeft"] = TMXTiledMap::create("map_block3.tmx");
    mapPool["lowerRight"] = TMXTiledMap::create("map_block4.tmx");
    
    // 默认加载左上地图块
    currentMap = mapPool["lowerLeft"];
    currentMapLabel = 3;
    if (!currentMap) {
        CCLOG("Error:Faled to load map.tmx");
        throw std::runtime_error("upperLeftMap created failed!!");
    }
    else
        this->addChild(currentMap);;
     
    //背景尺寸
    const Size spriteSize = currentMap->getContentSize();

    // 计算宽度和高度的缩放比例
    auto screenSize = Director::getInstance()->getVisibleSize();
    scaleX = screenSize.width / spriteSize.width;
    scaleY = screenSize.height / spriteSize.height;

    // 选择更大的缩放比例，确保图片覆盖整个屏幕
    scale = std::max(scaleX, scaleY);
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
    
    TMXTiledMap* newMap = nullptr;

    // 根据方向切换地图块
    if (direction == Vec2(-1, 0)&&currentMapLabel==2) { // 左边界
        newMap = TMXTiledMap::create("map_block1.tmx");
        currentMapLabel = 1;
    }
    else if (direction == Vec2(-1, 0) && currentMapLabel == 4) { // 左边界
        newMap = TMXTiledMap::create("map_block3.tmx");
        currentMapLabel = 3;
    }
    else if (direction == Vec2(1, 0)&&currentMapLabel==1) { // 右边界
        newMap = TMXTiledMap::create("map_block2.tmx");
        currentMapLabel = 2;
    }
    else if (direction == Vec2(1, 0) && currentMapLabel == 3) { // 右边界
        newMap = TMXTiledMap::create("map_block4.tmx");
        currentMapLabel = 4;
    }
    else if (direction == Vec2(0, -1) && currentMapLabel == 1) { // 下边界
        newMap = TMXTiledMap::create("map_block3.tmx");
        currentMapLabel = 3;
    }
    else if (direction == Vec2(0, -1) && currentMapLabel == 2) { // 下边界
        newMap = TMXTiledMap::create("map_block4.tmx");
        currentMapLabel = 4;
    }
    else if (direction == Vec2(0, 1) && currentMapLabel == 3) { // 上边界
        newMap = TMXTiledMap::create("map_block1.tmx");
        currentMapLabel = 1;
    }
    else if (direction == Vec2(0, 1) && currentMapLabel == 4) { // 上边界
        newMap = TMXTiledMap::create("map_block2.tmx");
        currentMapLabel = 2;
    }
    // 检查切换后的地图块是否有效
    if (newMap) {
        if (currentMap) {
            currentMap->removeFromParentAndCleanup(true);
            currentMap = nullptr;
        }
        this->addChild(newMap);

        // 选择更大的缩放比例，确保图片覆盖整个屏幕
        auto screenSize = Director::getInstance()->getVisibleSize();
        const Size spriteSize = newMap->getContentSize();
        scaleX = screenSize.width / spriteSize.width;
        scaleY = screenSize.height / spriteSize.height;

       
        scale = std::max(scaleX, scaleY);
        newMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
        newMap->setScale(scale); // 根据需要调整比例
        newMap->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点


        currentMap = newMap;
        CCLOG("currentMap : %p ", currentMap);
    }
    else
        CCLOG("newMap is Null!");
  
}

bool MapManager::isAtEdge(const Vec2& playerPos, Vec2& outDirection) {
    // 获取当前地图的尺寸

    if (!currentMap) {
        CCLOG("error : currentMap is nullptr in isAtEdge");
        return false;
    }
    else {
        ;
    }
    const Size mapSize = currentMap->getMapSize();      // 格子数
    const Size tileSize = currentMap->getTileSize();    // 每个格子的大小
    const float mapWidth = mapSize.width * tileSize.width* scaleX;
   const  float mapHeight = mapSize.height * tileSize.height* scaleY;

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
Size MapManager::getCurrentMapSize(bool inPixels) const {
    if (!currentMap) {
        throw std::runtime_error("No map loaded.");
    }

    const Size mapGridSize = currentMap->getMapSize();  // 地图的格子数
    const Size tileSize = currentMap->getTileSize();    // 每个格子的大小

    if (inPixels) {
        // 返回以像素为单位的地图大小
        return Size(mapGridSize.width * tileSize.width, mapGridSize.height * tileSize.height);
    }
    else {
        // 返回以格子数为单位的地图大小
        return mapGridSize;
    }
}