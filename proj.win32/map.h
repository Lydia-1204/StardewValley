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

    TMXTiledMap* mapBlocks[4];  // 存储四个地图块
    TMXTiledMap* currentBlock;  // 当前加载的地图块
    int currentBlockIndex;      // 当前地图块索引

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