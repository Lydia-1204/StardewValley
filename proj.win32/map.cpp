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
    // �����ĸ���ͼ��
  
    mapPool["upperLeft"] = TMXTiledMap::create("../Resources/map_block1.tmx");
    mapPool["upperRight"] = TMXTiledMap::create("../Resources/map_block2.tmx");
    mapPool["lowerLeft"] = TMXTiledMap::create("../Resources/map_block3.tmx");
    mapPool["lowerRight"] = TMXTiledMap::create("../Resources/map_block4.tmx");
    
    // Ĭ�ϼ������ϵ�ͼ��
    currentMap = mapPool["upperRight"]; 

    if (!currentMap) {
        CCLOG("Error:Faled to load map.tmx");
        throw std::runtime_error("upperLeftMap created failed!!");
    }
    else  
        this->addChild(currentMap);
     
    //�����ߴ�
    const Size spriteSize = currentMap->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    auto screenSize = Director::getInstance()->getVisibleSize();
    float scaleX = screenSize.width / spriteSize.width;
    float scaleY = screenSize.height / spriteSize.height;

    // ѡ���������ű�����ȷ��ͼƬ����������Ļ
    float scale = std::max(scaleX, scaleY);
    currentMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 ));
    currentMap->setScale(scale); // ������Ҫ��������
    currentMap->setAnchorPoint(Vec2(0.5,0.5)); // ����ê��
   
    CCLOG("currentMap : %p ", currentMap);
    playerStartPos = Vec2(16 * 5, 16 * 5); // ����Ĭ����ҳ�ʼλ��
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
    // ���ݷ����л���ͼ��

    if (!currentMap) {
        CCLOG("Error: currentMap is nullptr");
        return;
    }

    // �Ƴ���ǰ��ͼ�飬�����ڴ�
    this->removeChild(currentMap, false);
    currentMap = nullptr;


    CCLOG("currentMap : %p ", currentMap);
    // ���ݷ����л���ͼ��
    if (direction == Vec2(-1, 0)) { // ��߽�
        currentMap = mapPool["upperLeft"];
    }
    else if (direction == Vec2(1, 0)) { // �ұ߽�
        currentMap = mapPool["upperRight"];
    }
    else if (direction == Vec2(0, -1)) { // �±߽�
        currentMap = mapPool["lowerLeft"];
    }
    else if (direction == Vec2(0, 1)) { // �ϱ߽�
        currentMap = mapPool["lowerRight"];
    }

    // ����л���ĵ�ͼ���Ƿ���Ч
    if (!currentMap) {
        CCLOG("Error: Failed to switch map block");
        return;
    }


    CCLOG("currentMap : %p ", currentMap);
    /* ȷ��û���������ڵ�
    if (currentMap&&currentMap->getParent()) {
        currentMap->removeFromParent();
    }*/
//    currentMap->removeFromParent();
    // ����µ�ͼ��
    this->addChild(currentMap);
    CCLOG("Switched to new map block: %p", currentMap);
    // ������ҳ�ʼλ�ã�����������Ǵ��µ�ͼ�����Ľ��룩
   const Size mapSize = currentMap->getMapSize();
   const Size tileSize = currentMap->getTileSize();
    playerStartPos = Vec2(mapSize.width * tileSize.width / 2, mapSize.height * tileSize.height / 2);
}

bool MapManager::isAtEdge(const Vec2& playerPos, Vec2& outDirection) {
    // ��ȡ��ǰ��ͼ�ĳߴ�

    if (!currentMap) {
        CCLOG("error : currentMap is nullptr in isAtEdge");
        return false;
    }
    else {
        CCLOG("currentMap : %p ",currentMap);
    }
    const Size mapSize = currentMap->getMapSize();      // ������
    const Size tileSize = currentMap->getTileSize();    // ÿ�����ӵĴ�С
    const float mapWidth = mapSize.width * tileSize.width;
   const  float mapHeight = mapSize.height * tileSize.height;

    // �������Ƿ񵽴�߽�
    if (playerPos.x <= 0) {
        outDirection = Vec2(-1, 0); // ��߽�
        return true;
    }
    else if (playerPos.x >= mapWidth) {
        outDirection = Vec2(1, 0); // �ұ߽�
        return true;
    }
    else if (playerPos.y <= 0) {
        outDirection = Vec2(0, -1); // �±߽�
        return true;
    }
    else if (playerPos.y >= mapHeight) {
        outDirection = Vec2(0, 1); // �ϱ߽�
        return true;
    }

    return false;
}