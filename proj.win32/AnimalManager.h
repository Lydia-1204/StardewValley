/****************************************************************
 * Project Name:  StardewValley
 * File Name:     AnimalManager.h
 * File Function: ʵ��AnimalManager�࣬��ɶ�����ֳ����
 * Author:        ���庬 2351591
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
    cocos2d::Layer* menuLayer; // ���һ����Ա�������洢�˵��������
    friend class Animal;
    static AnimalManager* _instance;


public:
    void openAnimalMenu();
    void closeAnimalMenu(cocos2d::Ref* sender);
    void addAnimalInfoH(Animal* animal, const Vec2& position);
    void addAnimalInfoM(Animal* animal, const Vec2& position);
    void addAnimalInfoA(Animal* animal, const Vec2& position);
    //std::vector<Animal*> animals;  // �����б�
    std::vector<std::unique_ptr<Animal>> animals;  // ʹ��unique_ptr�������ڴ�


    static AnimalManager* getInstance();

    // �������ͱ���Ϊ��ָ��Animal*
    Animal* getAnimal(const std::string& animalName) {
        for (auto& animal : animals) {
            if (animal->getAnimalName() == animalName) {
                return animal.get();  // ͨ��unique_ptr��ȡ��ָ��
            }
        }
        return nullptr;
    }

    // ��Ȼ������ָ��Animal*������ʹ��std::unique_ptr�������ڴ�
    void addAnimal(Animal* animal) {
        animals.push_back(std::unique_ptr<Animal>(animal));  // ����ָ��תΪunique_ptr���洢
    }

};


#endif // ANIMALMANAGER_H
