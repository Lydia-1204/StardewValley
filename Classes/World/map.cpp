#include "World/map.h"
USING_NS_CC;

MapManager* MapManager::instance = nullptr;

MapManager::MapManager() : currentMap(nullptr), playerStartPos(Vec2::ZERO), selectedCharacter(0),
chickenStartPos(Vec2(1100, 113)), catStartPos(Vec2(750, 285)), dogStartPos(Vec2(730, 250)), cowStartPos(Vec2(900, 230)),
sheepStartPos(Vec2(1040, 235)), pigStartPos(Vec2(890, 140)), cropStartPos(Vec2(1040, 123)) {}

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
    mapPool["indoors"] = TMXTiledMap::create("Indoors.tmx");
    mapPool["shop"] = TMXTiledMap::create("shop.tmx");
    mapPool["mine"] = TMXTiledMap::create("mine.tmx");

    // 默认加载左上地图块
    currentMap = mapPool["upperRight"];
    //currentMap = mapPool["indoors"];
    currentMapLabel = 2;
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
    currentMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    currentMap->setScale(scale); // 根据需要调整比例
    currentMap->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点


    CCLOG("currentMap : %p ", currentMap);
    //playerStartPos = Vec2(16 * 5, 16 * 5); // 设置默认玩家初始位置
    playerStartPos = Vec2(200, 300);
}


TMXTiledMap* MapManager::getCurrentBlock() {
    if (!currentMap)
        throw std::runtime_error("failed to load currentMap!!");
    CCLOG("currentMap : %p ", currentMap);
    return currentMap;
}


int MapManager::getCurrentBlockLabel() {
    return currentMapLabel;
}

Vec2 MapManager::getPlayerStartPos() {
    return playerStartPos;
}


void MapManager::switchToHouseOrOutside(int label) {
    TMXTiledMap* newMap;
    switch (label) {
    case 1:
        newMap = TMXTiledMap::create("map_block1.tmx");
        currentMapLabel = 1;
        break;
    case 2:
        newMap = TMXTiledMap::create("map_block2.tmx");
        currentMapLabel = 2;
        break;
    case 3:
        newMap = TMXTiledMap::create("map_block3.tmx");
        currentMapLabel = 3;
        break;
    case 5:
        newMap = TMXTiledMap::create("Indoors.tmx");
        currentMapLabel = 5;
        break;
    case 6:
        newMap = TMXTiledMap::create("shop.tmx");
        currentMapLabel = 6;
        break;
    case 7:
        newMap = TMXTiledMap::create("mine.tmx");
        currentMapLabel = 7;
        break;
    }
    // 检查切换后的地图块是否有效
    if (newMap) {
        if (currentMap) {
            currentMap->removeFromParentAndCleanup(true);
        }
        this->addChild(newMap);

        auto screenSize = Director::getInstance()->getVisibleSize();
        const Size spriteSize = newMap->getContentSize();
        scaleX = screenSize.width / spriteSize.width;
        scaleY = screenSize.height / spriteSize.height;

        scale = std::max(scaleX, scaleY);
        newMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
        newMap->setScale(scale); // 根据需要调整比例
        newMap->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点

        currentMap = newMap;
    }
}



void MapManager::switchToBlock(const Vec2& direction) {
    TMXTiledMap* newMap = nullptr;

    // 根据方向切换地图块
    switch (currentMapLabel) {
    case 1:
        if (direction == Vec2(1, 0)) {
            newMap = TMXTiledMap::create("map_block2.tmx");
            currentMapLabel = 2;
        }
        else if (direction == Vec2(0, -1)) {
            newMap = TMXTiledMap::create("map_block3.tmx");
            currentMapLabel = 3;
        }
        break;
    case 2:
        if (direction == Vec2(-1, 0)) {
            newMap = TMXTiledMap::create("map_block1.tmx");
            currentMapLabel = 1;
        }
        else if (direction == Vec2(0, -1)) {
            newMap = TMXTiledMap::create("map_block4.tmx");
            currentMapLabel = 4;
        }
        break;
    case 3:
        if (direction == Vec2(1, 0)) {
            newMap = TMXTiledMap::create("map_block4.tmx");
            currentMapLabel = 4;
        }
        else if (direction == Vec2(0, 1)) {
            newMap = TMXTiledMap::create("map_block1.tmx");
            currentMapLabel = 1;
        }
        break;
    case 4:
        if (direction == Vec2(-1, 0)) {
            newMap = TMXTiledMap::create("map_block3.tmx");
            currentMapLabel = 3;
        }
        else if (direction == Vec2(0, 1)) {
            newMap = TMXTiledMap::create("map_block2.tmx");
            currentMapLabel = 2;
        }
        break;

    }
    // 检查切换后的地图块是否有效
    if (newMap) {
        if (currentMap) {
            currentMap->removeFromParentAndCleanup(true);
        }
        this->addChild(newMap);

        auto screenSize = Director::getInstance()->getVisibleSize();
        const Size spriteSize = newMap->getContentSize();
        scaleX = screenSize.width / spriteSize.width;
        scaleY = screenSize.height / spriteSize.height;

        scale = std::max(scaleX, scaleY);
        newMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
        newMap->setScale(scale); // 根据需要调整比例
        newMap->setAnchorPoint(Vec2(0.5, 0.5)); // 设置锚点

        currentMap = newMap;
    }
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
    const float mapWidth = mapSize.width * tileSize.width * scaleX;
    const  float mapHeight = mapSize.height * tileSize.height * scaleY;

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

bool MapManager::isEnterHouse(const Vec2& playerPos, Vec2& outDirection) {
    /*
        player position 688.794922,366.414612
        player position 718.795288, 371.414581

        player position 739.798218,353.391296
        player position 764.798523,353.391296


        player position 661.736389,278.402649
        player position 691.736755,281.735962

        player position 683.403320,295.069214

    */

    if (currentMapLabel == 1 && playerPos.x <= 692 && playerPos.x >= 660 && playerPos.y <= 298 && playerPos.y >= 290) {
        return true;
    }
    else     if (currentMapLabel == 3 && playerPos.x <= 1009 && playerPos.x >= 983 && playerPos.y <= 178 && playerPos.y >= 150) {
        return true;
    }
    else     if (currentMapLabel == 2 && playerPos.x <= 498 && playerPos.x >= 445 && playerPos.y <= 589 && playerPos.y >= 578) {
        return true;
    }
    else { return false; }
}

bool  MapManager::isExitHouse(const Vec2& playerPos, Vec2& outDirection) {
    /*
    player position 643.557434,501.098511
    player position 681.891235,501.098511
    player position 681.891235,516.098511
    player position 663.557678,492.765259

    */
    if (currentMapLabel == 7) { //矿井的退出部位单独处理
        if (playerPos.x <= 805 && playerPos.x >= 605 && playerPos.y <= 160 && playerPos.y >= 157) {
            return true;
        }
        else { return false; }
    }
    else {
        if (playerPos.x <= 683 && playerPos.x >= 597 && playerPos.y <= 510 && playerPos.y >= 420) {
            return true;
        }
        else { return false; }
    }


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


Vec2 MapManager::getTileCoordForPosition(const Vec2& position) {
    // 获取地图的每个瓦片的大小
    Size tileSize = currentMap->getTileSize();

    Size mapSize = currentMap->getContentSize();

    int x = floor(position.x / tileSize.width);
    int y = floor((mapSize.height - position.y) / tileSize.height);

    return Vec2(x, y);
}

Vec2 MapManager::absoluteToTile(const Vec2& position)
{
    //世界坐标->地图瓦片坐标
    float scale = currentMap->getScale();//获取瓦片大小
    Size tileSize = currentMap->getTileSize();

    float tileX = position.x / (tileSize.width * scale);
    float tileY = position.y / (tileSize.height * scale);

    tileY = currentMap->getMapSize().height - tileY;

    return Vec2(floor(tileX), floor(tileY));
}

Vec2 MapManager::tileToAbsolute(const Vec2& tileCoord)//瓦片坐标->世界坐标（中点）
{
    float scale = currentMap->getScale();//获取瓦片大小
    Size tileSize = currentMap->getTileSize();
    //计算瓦片左下角的绝对坐标
    float pixelX = tileCoord.x * tileSize.width * scale;
    float pixelY = (currentMap->getMapSize().height - tileCoord.y - 1) * tileSize.height * scale;

    // 加上瓦片半的宽高，得到瓦片的中点坐标
    pixelX += (tileSize.width * scale) / 2.0f; pixelY += (tileSize.height * scale) / 2.0f;
    return Vec2(pixelX, pixelY);
}


bool MapManager::isObstacleAtPosition(const Vec2& position) {
    // 获取当前地图的Tilemap对象
    if (!currentMap) {
        CCLOG("Error: currentMap is nullptr in isObstacleAtPosition");
        return false;
    }

    // 获取tile坐标
    Vec2 tileCoord = absoluteToTile(position);
    /*


    // 获取多个层进行检查
    std::vector<std::string> layerNames;
    if(currentMapLabel<=4&&currentMapLabel>=1)
        layerNames = { "AlwaysFront", "Front","Buildings","Back"};aaaa
    else if (currentMapLabel >= 5)
        layerNames = { "furniture", "wall","floorEdge","floor"};
    for (const auto& layerName : layerNames) {
        if (currentMapLabel == 5) {
            //室内地图，仅floor可走
            int gid = getTileGid(layerName, tileCoord);
            if (gid) {
                CCLOG("You can't go through here!\n"); return true; }  //在其余层有gid值，说明是障碍物
            //其余情况不是障碍物
        }
        else if (currentMapLabel >= 1 && currentMapLabel <= 4) {
            //室外地图，除Back外均不可走
            int gid = getTileGid(layerName, tileCoord);
            if (gid) {
                CCLOG("You can't go through here!\n"); return true; }  //在其余层gid值，说明是障碍物
            //其余情况不是障碍物
        }
    }
    return false;

    */




    // 获取多个层进行检查
    std::vector<std::string> layerNames;
    if (currentMapLabel <= 4 && currentMapLabel >= 1)
        layerNames = { "AlwaysFront", "Front","Buildings" };
    else if (currentMapLabel >= 5)
        layerNames = { "furniture", "wall","floorEdge" };
    //先对室内地图检查边界
    if (currentMapLabel == 5 || currentMapLabel == 6) {
        if (position.x > 850 || position.x < 428 || position.y < 180) {
            CCLOG("You can't go through here!\n");
            return true;
        }
    }
    //对矿井检查边界
    if (currentMapLabel == 7) {
        if (position.x > 839 || position.x < 439 || position.y > 483) {
            CCLOG("You can't go through here!\n");
            return true;
        }
        if (position.x < 581 && position.x > 439) {
            if (position.y < 240) {
                CCLOG("You can't go through here!\n");
                return true;
            }
        }
        if (position.x > 814 && position.x > 843) {
            if (position.y < 240) {
                CCLOG("You can't go through here!\n");
                return true;
            }
        }
        return false;
    }
    //对四张室外地图检查边界
    if (currentMapLabel <= 4) {
        auto screenSize = Director::getInstance()->getVisibleSize();

        //左边界
        if (currentMapLabel == 1 || currentMapLabel == 3) {
            if (position.x < 10) {
                CCLOG("You can't go through here!\n");
                return true;
            }
        }
        //上边界
        if (currentMapLabel == 1 || currentMapLabel == 2) {
            if (position.y > screenSize.height - 10) {
                CCLOG("screenSize.height = %lf", screenSize.height);
                CCLOG("You can't go through here!\n");
                return true;
            }
        }
        //右边界
        if (currentMapLabel == 2 || currentMapLabel == 4) {
            if (position.x > screenSize.width - 10) {
                CCLOG("screenSize.width = %lf", screenSize.width);
                CCLOG("You can't go through here!\n");
                return true;
            }
        }
        //下边界
        if (currentMapLabel == 3 || currentMapLabel == 4) {
            if (position.y < 10) {
                CCLOG("You can't go through here!\n");
                return true;
            }
        }
    }

    for (const auto& layerName : layerNames) {
        int gid = getTileGid(layerName, tileCoord);
        if (gid == 0) {
            continue;  // 该tile没有有效的GID，跳过
        }
        CCLOG("GID == %d\n", gid);
        // 获取tile的属性dddd
        Value tileProperties = currentMap->getPropertiesForGID(gid);

        // 确保tileProperties是一个ValueMap
        if (tileProperties.getType() == Value::Type::MAP) {
            // 只有在tileProperties是ValueMap时，才调用asValueMap()
            ValueMap properties = tileProperties.asValueMap();

            // 检查是否有障碍物属性
            if (properties.find("obstacle") != properties.end()) {
                if (properties.at("obstacle").asBool()) {
                    CCLOG("You can't go through here!\n");
                    return true;  // 是障碍物
                }
            }
        }
        else {
            CCLOG("Error: Tile properties for GID %d are not a ValueMap", gid);
        }
    }

    return false; // 没有命中任何障碍，允许移动

}


void MapManager::setTileState(const Vec2& tileCoord, const std::string& state) {
    // Set tile state, for example, "dug"
    TMXLayer* layer = currentMap->getLayer("Back");
    int gid = layer->getTileGIDAt(tileCoord);
    if (gid != 0) {
        ValueMap properties = currentMap->getPropertiesForGID(gid).asValueMap();
        properties["state"] = state;
        //currentMap->setPropertiesForGID(gid, properties);
    }
}

bool MapManager::isNearWater(const Vec2& position) {
    // 获取当前地图的Tilemap对象
    if (!currentMap) {
        CCLOG("Error: currentMap is nullptr in isObstacleAtPosition");
        return false;
    }

    // 获取tile坐标
    Vec2 tileCoord = getTileCoordForPosition(position);
    // 获取多个层进行检查
    const std::vector<std::string> layerNames = { "AlwaysFront", "Front","Buildings","Back" };
    for (const auto& layerName : layerNames) {
        TMXLayer* layer = currentMap->getLayer(layerName);
        if (!layer) {
            continue;  // 如果没有该层，跳过
        }

        // 获取GID
        int gid = layer->getTileGIDAt(tileCoord);
        if (gid == 0) {
            continue;  // 该tile没有有效的GID，跳过
        }

        // 获取tile的属性
        ValueMap tileProperties = currentMap->getPropertiesForGID(gid).asValueMap();

        // 检查是否有障碍物属性
        if (tileProperties.find("Water") != tileProperties.end()) {
            if (tileProperties.at("Water").asString() == "T") {
                return true;  // 是水边
            }
        }
    }
    return false;  // 不是水边
}

int MapManager::getTileGid(const std::string& layerName, const Vec2& tileCoord) {
    auto layer = currentMap->getLayer(layerName);
    if (!layer) {
        CCLOG("Invalid Layer name");
        return 0;
    }
    auto map_size = currentMap->getMapSize();
    auto tile_size = currentMap->getTileSize();
    if (tileCoord.x < 0 || tileCoord.y < 0 || tileCoord.x >= map_size.width || tileCoord.y >= map_size.height) {
        return 0;
    }
    return layer->getTileGIDAt(tileCoord);
}


Vec2 MapManager::getChickenStartPos() {
    return chickenStartPos;
}

Vec2 MapManager::getCatStartPos() {
    return catStartPos;
}

Vec2 MapManager::getDogStartPos() {
    return dogStartPos;
}

Vec2 MapManager::getCowStartPos() {
    return cowStartPos;
}

Vec2 MapManager::getSheepStartPos() {
    return sheepStartPos;
}

Vec2 MapManager::getPigStartPos() {
    return pigStartPos;
}

Vec2 MapManager::getCropStartPos() {
    return Vec2(848.0,600.0);
}
