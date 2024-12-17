#include "map.h"
USING_NS_CC;

MapManager* MapManager::instance = nullptr;

MapManager::MapManager() : currentMap(nullptr), playerStartPos(Vec2::ZERO), selectedCharacter(0) {}

MapManager* MapManager::getInstance() {
  
    if (instance == nullptr) {  // ���ʵ�������ڣ��򴴽�
        instance = new (std::nothrow) MapManager();
        if (instance && instance->init()) {
            instance->autorelease();  // ��ӵ��ڴ����ϵͳ
        }
        else {
            CC_SAFE_DELETE(instance);
        }
    }
    return instance;  // ����Ψһʵ��
}

bool MapManager::init() {
    if (!Node::init()) {
        return false;
    }
    return true;
}

void MapManager::loadMapBlocks(const std::string& mapFolder) {
    // �����ĸ���ͼ��
  
    mapPool["upperLeft"] = TMXTiledMap::create("map_block1.tmx");
    mapPool["upperRight"] = TMXTiledMap::create("map_block2.tmx");
    mapPool["lowerLeft"] = TMXTiledMap::create("map_block3.tmx");
    mapPool["lowerRight"] = TMXTiledMap::create("map_block4.tmx");
    
    // Ĭ�ϼ������ϵ�ͼ��
    currentMap = mapPool["lowerLeft"];
    currentMapLabel = 3;
    if (!currentMap) {
        CCLOG("Error:Faled to load map.tmx");
        throw std::runtime_error("upperLeftMap created failed!!");
    }
    else
        this->addChild(currentMap);;
     
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
    
    TMXTiledMap* newMap = nullptr;

    // ���ݷ����л���ͼ��
    if (direction == Vec2(-1, 0)&&currentMapLabel==2) { // ��߽�
        newMap = TMXTiledMap::create("map_block1.tmx");
        currentMapLabel = 1;
    }
    else if (direction == Vec2(-1, 0) && currentMapLabel == 4) { // ��߽�
        newMap = TMXTiledMap::create("map_block3.tmx");
        currentMapLabel = 3;
    }
    else if (direction == Vec2(1, 0)&&currentMapLabel==1) { // �ұ߽�
        newMap = TMXTiledMap::create("map_block2.tmx");
        currentMapLabel = 2;
    }
    else if (direction == Vec2(1, 0) && currentMapLabel == 3) { // �ұ߽�
        newMap = TMXTiledMap::create("map_block4.tmx");
        currentMapLabel = 4;
    }
    else if (direction == Vec2(0, -1) && currentMapLabel == 1) { // �±߽�
        newMap = TMXTiledMap::create("map_block3.tmx");
        currentMapLabel = 3;
    }
    else if (direction == Vec2(0, -1) && currentMapLabel == 2) { // �±߽�
        newMap = TMXTiledMap::create("map_block4.tmx");
        currentMapLabel = 4;
    }
    else if (direction == Vec2(0, 1) && currentMapLabel == 3) { // �ϱ߽�
        newMap = TMXTiledMap::create("map_block1.tmx");
        currentMapLabel = 1;
    }
    else if (direction == Vec2(0, 1) && currentMapLabel == 4) { // �ϱ߽�
        newMap = TMXTiledMap::create("map_block2.tmx");
        currentMapLabel = 2;
    }
    // ����л���ĵ�ͼ���Ƿ���Ч
    if (newMap) {
        if (currentMap) {
            currentMap->removeFromParentAndCleanup(true);
            currentMap = nullptr;
        }
        this->addChild(newMap);

        // ѡ���������ű�����ȷ��ͼƬ����������Ļ
        auto screenSize = Director::getInstance()->getVisibleSize();
        const Size spriteSize = newMap->getContentSize();
        scaleX = screenSize.width / spriteSize.width;
        scaleY = screenSize.height / spriteSize.height;

       
        scale = std::max(scaleX, scaleY);
        newMap->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));
        newMap->setScale(scale); // ������Ҫ��������
        newMap->setAnchorPoint(Vec2(0.5, 0.5)); // ����ê��


        currentMap = newMap;
        CCLOG("currentMap : %p ", currentMap);
    }
    else
        CCLOG("newMap is Null!");
  
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