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
    currentMap = mapPool["upperLeft"];

    if (!currentMap) {
        CCLOG("Error:Faled to load map.tmx");
        throw std::runtime_error("upperLeftMap created failed!!");
    }
    else
        ;
     
    //�����ߴ�
    const Size spriteSize = currentMap->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    auto screenSize = Director::getInstance()->getVisibleSize();
    scaleX = screenSize.width / spriteSize.width;
    scaleY = screenSize.height / spriteSize.height;

    // ѡ���������ű�����ȷ��ͼƬ����������Ļ
    scale = std::max(scaleX, scaleY);
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

    CCLOG("after switch currentMap : %p ", currentMap);

    //�ٴε�����ͼλ��

   //�����ߴ�
    const Size spriteSize = currentMap->getContentSize();

    // �����Ⱥ͸߶ȵ����ű���
    auto screenSize = Director::getInstance()->getVisibleSize();
    scaleX = screenSize.width / spriteSize.width;
    scaleY = screenSize.height / spriteSize.height;

    // ѡ���������ű�����ȷ��ͼƬ����������Ļ
    scale = std::max(scaleX, scaleY);
    currentMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
    currentMap->setScale(scale); // ������Ҫ��������
    currentMap->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��

    CCLOG("currentMap : %p ", currentMap);
}

bool MapManager::isAtEdge(const Vec2& playerPos, Vec2& outDirection) {
    // ��ȡ��ǰ��ͼ�ĳߴ�

    if (!currentMap) {
        CCLOG("error : currentMap is nullptr in isAtEdge");
        return false;
    }
    else {
        ;
    }
    const Size mapSize = currentMap->getMapSize();      // ������
    const Size tileSize = currentMap->getTileSize();    // ÿ�����ӵĴ�С
    const float mapWidth = mapSize.width * tileSize.width* scaleX;
   const  float mapHeight = mapSize.height * tileSize.height* scaleY;

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
Size MapManager::getCurrentMapSize(bool inPixels) const {
    if (!currentMap) {
        throw std::runtime_error("No map loaded.");
    }

    const Size mapGridSize = currentMap->getMapSize();  // ��ͼ�ĸ�����
    const Size tileSize = currentMap->getTileSize();    // ÿ�����ӵĴ�С

    if (inPixels) {
        // ����������Ϊ��λ�ĵ�ͼ��С
        return Size(mapGridSize.width * tileSize.width, mapGridSize.height * tileSize.height);
    }
    else {
        // �����Ը�����Ϊ��λ�ĵ�ͼ��С
        return mapGridSize;
    }
}