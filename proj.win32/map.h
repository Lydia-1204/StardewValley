#pragma once
#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cocos2d.h"

USING_NS_CC;

enum class Direction {
    UP,
    DOWN,
    LEFT,
    RIGHT
};

class MapManager {
private:
    static MapManager* instance;

    TMXTiledMap* mapBlocks[4];  // �洢�ĸ���ͼ��
    TMXTiledMap* currentBlock;  // ��ǰ���صĵ�ͼ��
    int currentBlockIndex;      // ��ǰ��ͼ������

    MapManager();

public:
    static MapManager* getInstance();

    void loadMapBlocks(const std::string& mapFile);
    TMXTiledMap* getCurrentBlock();
    void switchToBlock(Direction dir);
    bool isAtEdge(const Vec2& position, Direction& outDirection);
    void updatePlayerPositionAfterSwitch(Direction dir);
};

#endif // MAP_MANAGER_H