#include "AnimalManager.h"
#include "Animal.h"
#include "cocos2d.h"

USING_NS_CC;

Animal* AnimalManager::getAnimal(const std::string& animalName) {
    for (auto* animal : animals) {
        if (animal->getAnimalName() == animalName) {
            return animal;
        }
    }
    return nullptr;
}

void AnimalManager::addAnimal(Animal* animal) {
    animals.push_back(animal);
}

void AnimalManager::feedAnimal(const std::string& animalName) {
    for (auto* animal : animals) {
        if (animal->getAnimalName() == animalName) {
            if (feedSupply > 0) {
                animal->feed();
                --feedSupply;
            }
            return;
        }
    }
}

void AnimalManager::collectProduct(const std::string& animalName) {
    for (auto* animal : animals) {
        if (animal->getAnimalName() == animalName) {
            std::string product = animal->produceProduct();
            if (!product.empty()) {
                CCLOG("%s collected: %s", animalName.c_str(), product.c_str());
            }
            return;
        }
    }
}

void AnimalManager::updateAnimals() {
    for (auto* animal : animals) {
        animal->updateState();
    }
}

void AnimalManager::renderAnimals(Layer* layer) {
    // 渲染动物图标的代码可以在这里进行
    for (auto* animal : animals) {
        // 动物图像使用Sprite加载
        auto sprite = Sprite::create(animal->getAnimalName() + ".png");
        animal->addChild(sprite);
        layer->addChild(sprite);
    }
}
