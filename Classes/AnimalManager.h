#ifndef ANIMALMANAGER_H
#define ANIMALMANAGER_H

#include <vector>
#include <string>
#include "Animal.h"
#include "cocos2d.h"

class AnimalManager {
private:
    std::vector<Animal*> animals;  // �����б�
    int feedSupply;  // ���Ͽ��

public:
    AnimalManager() : feedSupply(10) {}

    // ��ӻ�ȡ����ķ���
    Animal* getAnimal(const std::string& animalName);

    // �������Ͽ��
    void setFeedSupply(int supply) { feedSupply = supply; }

    // ��Ӷ���
    void addAnimal(Animal* animal);

    // ι������
    void feedAnimal(const std::string& animalName);

    // �ռ�����Ĳ���
    void collectProduct(const std::string& animalName);

    // �������ж���״̬
    void updateAnimals();

    // ��Ⱦ���ﲢ����λ��
    void renderAnimals(cocos2d::Layer* layer);
};

#endif // ANIMALMANAGER_H
