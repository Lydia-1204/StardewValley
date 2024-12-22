/****************************************************************
 * Project Name:  StardewValley
 * File Name:     AnimalManager.h
 * File Function: 实现AnimalManager类，完成动物养殖功能
 * Author:        刘彦含 2351591
 * Update Date:   2024/12/
 ****************************************************************/

#ifndef ANIMALMANAGER_H
#define ANIMALMANAGER_H


#include <memory>
#include <vector>
#include <string>
#include "Animal.h"
#include "cocos2d.h"
USING_NS_CC;


class AnimalManager : public Node {
private:
    cocos2d::Layer* menuLayer; // 添加一个成员变量来存储菜单层的引用
    friend class Animal;
    static AnimalManager* _instance;


public:
    void openAnimalMenu();
    void closeAnimalMenu(cocos2d::Ref* sender);
    void addAnimalInfoH(Animal* animal, const Vec2& position);
    void addAnimalInfoM(Animal* animal, const Vec2& position);
    void addAnimalInfoA(Animal* animal, const Vec2& position);
    //std::vector<Animal*> animals;  // 动物列表
    std::vector<std::unique_ptr<Animal>> animals;  // 使用unique_ptr管理动物内存


    static AnimalManager* getInstance();

    // 返回类型保持为裸指针Animal*
    Animal* getAnimal(const std::string& animalName) {
        for (auto& animal : animals) {
            if (animal->getAnimalName() == animalName) {
                return animal.get();  // 通过unique_ptr获取裸指针
            }
        }
        return nullptr;
    }

    // 仍然传入裸指针Animal*，并且使用std::unique_ptr来管理内存
    void addAnimal(Animal* animal) {
        animals.push_back(std::unique_ptr<Animal>(animal));  // 将裸指针转为unique_ptr并存储
    }

};


#endif // ANIMALMANAGER_H
