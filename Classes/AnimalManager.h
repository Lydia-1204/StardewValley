#ifndef ANIMALMANAGER_H
#define ANIMALMANAGER_H

#include <vector>
#include <string>
#include "Animal.h"
#include "cocos2d.h"

class AnimalManager {
private:
    std::vector<Animal*> animals;  // 动物列表
    int feedSupply;  // 饲料库存

public:
    AnimalManager() : feedSupply(10) {}

    // 添加获取动物的方法
    Animal* getAnimal(const std::string& animalName);

    // 设置饲料库存
    void setFeedSupply(int supply) { feedSupply = supply; }

    // 添加动物
    void addAnimal(Animal* animal);

    // 喂养动物
    void feedAnimal(const std::string& animalName);

    // 收集动物的产物
    void collectProduct(const std::string& animalName);

    // 更新所有动物状态
    void updateAnimals();

    // 渲染动物并返回位置
    void renderAnimals(cocos2d::Layer* layer);
};

#endif // ANIMALMANAGER_H
