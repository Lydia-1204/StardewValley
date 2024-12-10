#include "map.h"
#include "GameScene.h"


MapManager* MapManager::instance = nullptr;

MapManager::MapManager() {
    // ��ʼ����ͼ������
    for (int i = 0; i < 4; i++) {
        mapBlocks[i] = nullptr;
    }
}

MapManager* MapManager::getInstance() {
    if (instance == nullptr) {
        instance = new MapManager();
    }
    return instance;
}

void MapManager::loadMapBlocks(const std::string& mapFile) {
    // ����ԭʼ��ͼ�ļ��ֿ�����Ŀ��ͼ
    for (int i = 0; i < 4; i++) {
        std::string blockName = mapFile + "_block" + std::to_string(i + 1) + ".tmx";
        mapBlocks[i] = TMXTiledMap::create(blockName);
    }
    // Ĭ�ϼ������Ͻǿ�
    currentBlock = mapBlocks[0];
    currentBlockIndex = 0;
}

TMXTiledMap* MapManager::getCurrentBlock() {
    return currentBlock;
}

void MapManager::switchToBlock(Direction dir) {
    int nextBlockIndex = currentBlockIndex;

    // ���ݷ���ȷ��Ŀ���
    switch (dir) {
    case Direction::UP:
        if (currentBlockIndex == 2) nextBlockIndex = 0;
        if (currentBlockIndex == 3) nextBlockIndex = 1;
        break;
    case Direction::DOWN:
        if (currentBlockIndex == 0) nextBlockIndex = 2;
        if (currentBlockIndex == 1) nextBlockIndex = 3;
        break;
    case Direction::LEFT:
        if (currentBlockIndex == 1) nextBlockIndex = 0;
        if (currentBlockIndex == 3) nextBlockIndex = 2;
        break;
    case Direction::RIGHT:
        if (currentBlockIndex == 0) nextBlockIndex = 1;
        if (currentBlockIndex == 2) nextBlockIndex = 3;
        break;
    }

    if (nextBlockIndex != currentBlockIndex) {
        // �л���ͼ��
        auto gameScene = GameScene::getInstance();
        gameScene->replaceChild(currentBlock, mapBlocks[nextBlockIndex]);
        currentBlock = mapBlocks[nextBlockIndex];
        currentBlockIndex = nextBlockIndex;

        // ���½�ɫλ�ã���ֹ�л�������ڴ���λ�ã�
        updatePlayerPositionAfterSwitch(dir);
    }
}

void MapManager::updatePlayerPositionAfterSwitch(Direction dir) {
    auto player = GameScene::getInstance(selectedCharacter, nickname)->getPlayer();
    Vec2 position = player->getPosition();

    switch (dir) {
    case Direction::UP:
        position.y = currentBlock->getMapSize().height * currentBlock->getTileSize().height - 1;
        break;
    case Direction::DOWN:
        position.y = 0;
        break;
    case Direction::LEFT:
        position.x = currentBlock->getMapSize().width * currentBlock->getTileSize().width - 1;
        break;
    case Direction::RIGHT:
        position.x = 0;
        break;
    }

    player->setPosition(position);
}

bool MapManager::isAtEdge(const Vec2& position, Direction& outDirection) {
    // ��ȡ��ɫ�ĵ�ǰλ��
    Size mapSize = currentBlock->getMapSize();
    Size tileSize = currentBlock->getTileSize();

    // ���߽�
    if (position.x <= 0) {
        outDirection = Direction::LEFT;
        return true;
    }
    else if (position.x >= mapSize.width * tileSize.width - 1) {
        outDirection = Direction::RIGHT;
        return true;
    }
    else if (position.y <= 0) {
        outDirection = Direction::DOWN;
        return true;
    }
    else if (position.y >= mapSize.height * tileSize.height - 1) {
        outDirection = Direction::UP;
        return true;
    }

    return false;
}