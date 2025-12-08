#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class MapManager : public Node {
    friend class UIManager;
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
    //--------------------动物--------------------------//
    Vec2 chickenStartPos;             // 小鸡初始位置
    Vec2 catStartPos;             // 小鸡初始位置
    Vec2 dogStartPos;             // 小鸡初始位置
    Vec2 cowStartPos;             // 小鸡初始位置
    Vec2 sheepStartPos;             // 小鸡初始位置
    Vec2 pigStartPos;             // 小鸡初始位置

    Vec2 cropStartPos;             // 植物初始位置

    MapManager() ;                  // 私有化构造函数


public:
    static MapManager* getInstance(); // 获取单例实例
    int currentMapLabel;
    bool init();                      // 初始化

    void loadMapBlocks(const std::string& mapFolder); // 加载所有地图
    TMXTiledMap* getCurrentBlock();   // 获取当前地图块
    int getCurrentBlockLabel();   // 获取当前地图块label
    Vec2 getPlayerStartPos();         // 获取玩家初始位置

    void switchToBlock(const Vec2& direction); // 切换到指定地图块
    void switchToHouseOrOutside(int label);   //切换到室内/室外地图
    bool isAtEdge(const Vec2& playerPos, Vec2& outDirection); // 检测是否到达边界


    bool isEnterHouse(const Vec2& playerPos, Vec2& outDirection); // 检测是否进入家
    bool isEnterShop(const Vec2& playerPos, Vec2& outDirection); // 检测是否进入商店
    bool isExitHouse(const Vec2& playerPos, Vec2& outDirection); // 检测是否离开家
    bool isExitShop(const Vec2& playerPos, Vec2& outDirection); // 检测是否离开商店

    Size getCurrentMapSize(bool inPixels) const;// inPixels=1时 返回以像素为单位的地图大小，=0返回以格子数为单位的地图大小


    //动物——————————————————
    Vec2 getChickenStartPos();         // 获取小鸡初始位置
    Vec2 getCatStartPos();         // 获取小鸡初始位置
    Vec2 getDogStartPos();         // 获取小鸡初始位置
    Vec2 getCowStartPos();         // 获取小鸡初始位置
    Vec2 getSheepStartPos();         // 获取小鸡初始位置
    Vec2 getPigStartPos();         // 获取小鸡初始位置

    Vec2 getCropStartPos();         // 获取小鸡初始位置


    //与工具类——————————————————
    Vec2 getTileCoordForPosition(const Vec2& position);
    bool isNearWater(const Vec2& playerPos);    //判断是否水边（钓鱼交互）
    bool isObstacleAtPosition(const Vec2& position);//碰撞检测
    bool isDiggable(const Vec2& position); //挖掘检测（挖土交互）
    void setTileState(const Vec2& tileCoord, const std::string& state); //设置tile的state
    //Tree* getTreeAtPosition(const Vec2& position); //返回一个tree类
    //Crop* getCropAtPosition(const Vec2& position); //返回一个crop类

    Vec2 absoluteToTile(const Vec2& position);//世界坐标->地图瓦片坐标
    Vec2 tileToAbsolute(const Vec2& tileCoord);//瓦片坐标->世界坐标（中点）
    int getTileGid(const std::string& layerName, const Vec2& tileCoord);

    CREATE_FUNC(MapManager);
};

#endif // MAP_MANAGER_H
