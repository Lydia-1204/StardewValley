/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Animal.h
 * File Function: 实现Animal类，完成动物养殖功能
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/22
 ****************************************************************/
#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include "cocos2d.h"
#include <chrono>
#include "Inventory/itemManager.h"
#include "Inventory/toolManager.h"
class GameScene; // 前向声明 GameScene 类
USING_NS_CC;

class Animal : public cocos2d::Node {
private:
    friend class AnimalManager;
    Sprite* an_sprite;   // 动物角色精灵
    Sprite* animal[4];  // 声明一个包含 4 个精灵的数组
    int d;                      // 当前精灵的索引（0: 正着, 1: 向右, 2: 向后, 3: 向左）
    Vec2 lastPosition;
    std::string _picturename;  // 动物名称
    AnimalManager* manager;
    bool wasDoubleClick = false;  // 标记是否发生了双击
    std::chrono::steady_clock::time_point lastClickTime;  // 上次点击的时间
    ToolManager* toolManager;
    // 添加获取动物类型的枚举和方法
    enum class AnimalType {
        CHICKEN,
        COW,
        CAT,
        DOG,
        PIG,
        SHEEP
    };

    std::string name;  // 动物名称
    int health;        // 健康值
    int mood;          // 心情值
    int affection;     // 友谊值
    std::string product;  // 动物产物
    cocos2d::Layer* menuLayer; // 添加一个成员变量来存储菜单层的引用

    GameScene* gameScene;  // 用于存储对GameScene的引用

    bool hasWaterInBowl;  // 是否有水
    float timeEscaped;


public:
    Animal(std::string name, int health, int hunger, int mood, std::string product, int affection, bool hasWaterInBowl, bool hasHeart, Vec2 lastPosition)
        : name(name), health(health), mood(mood), product(product), affection(affection), hasWaterInBowl(hasWaterInBowl), lastPosition(lastPosition), timeEscaped(0){
        // 这里可以进行一些初始化操作，比如创建精灵等
        // an_sprite = Sprite::create("path_to_sprite"); // 假设有一个创建精灵的方法
    }

    Animal() : an_sprite(nullptr), health(80), mood(0), affection(0), hasWaterInBowl(false), lastPosition(100, 200) {
        // 默认构造函数，成员变量初始化为默认值
    }
    static Animal* create(int targetAnimal, GameScene* scene);
    bool init(int targetAnimal);
    Vec2 getCurrentDirection();
    void updateDirection(float dt, const std::string& picturename);
    void moveAlongPath(const std::vector<Vec2>& path, const std::string& picturename);
    void update(float dt);
    void initialmove(const std::string& picturename);
    void onMouseDown(EventMouse* event);
    void removeHappySprite(float dt);
    void openAnimalMenu();
    void closeAnimalMenu(cocos2d::Ref* sender);
    void addAnimalInfoH(Animal* animal, const Vec2& position);
    void addAnimalInfoM(Animal* animal, const Vec2& position);
    void addAnimalInfoA(Animal* animal, const Vec2& position);


    void updateFavorability();
    void updateMood();
    void feed();


    std::string getAnimalName() const;
    int getHealth() const;
    int getMood() const;
    int getAffection() const;

    AnimalType getAnimalType() const;

    // 生成产物
     Item::ItemType produceProduct();

    bool getHasWaterInBowl() { return hasWaterInBowl; }
    void setHasWaterInBowl(bool status) { hasWaterInBowl = status; }

    void placeWaterBowl(const Vec2& position);
    bool checkWaterBowlImage(Node* node);
    void getGameScene(GameScene* scene);

    // 判断宠物是否达到1000点好感度
    bool isMaxAffection() {
        return affection == 1000;
    }

    // 更新宠物状态（例如，检查宠物碗是否有水）
    void updateWater(bool isRainyDay) {
        if (isRainyDay) {
            hasWaterInBowl = true;  // 雨天，碗自动加满水
        }

        // 如果没有碗水，减少好感度
        if (!hasWaterInBowl) {
            affection -= 10;
            if (affection < 0) affection = 0;  // 确保好感度不会为负
        }
    }
};

#endif // ANIMAL_H
