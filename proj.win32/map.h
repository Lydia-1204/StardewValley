#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class MapManager : public Node {
private:
    static MapManager* instance;     // 单例实例

    std::map<std::string, TMXTiledMap*> mapPool; // 地图缓存池
    cocos2d::TMXTiledMap* currentMap;         // 当前加载的地图
    Vec2 playerStartPos;             // 玩家初始位置
    std::string nickname;            // 玩家昵称
    int selectedCharacter;           // 玩家选择的角色
    
    float scaleX;
    float scaleY;
    float scale; //缩放比例
   

public:
    static MapManager* getInstance(); // 获取单例实例
    MapManager();                    // 私有化构造函数
    bool init();                      // 初始化

    void loadMapBlocks(const std::string& mapFolder); // 加载所有地图
    TMXTiledMap* getCurrentBlock();   // 获取当前地图块
    Vec2 getPlayerStartPos();         // 获取玩家初始位置

    void switchToBlock(const Vec2& direction); // 切换到指定地图块
    bool isAtEdge(const Vec2& playerPos, Vec2& outDirection); // 检测是否到达边界
    Size getCurrentMapSize(bool inPixels) const;// inPixels=1时 返回以像素为单位的地图大小，=0返回以格子数为单位的地图大小
    CREATE_FUNC(MapManager);
};

#endif // MAP_MANAGER_H