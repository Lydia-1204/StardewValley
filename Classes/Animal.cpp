#include "Animal.h"
#include <iostream>

Animal::Animal(const std::string& name, int health, int hunger, int mood, const std::string& product)
    : name(name), health(health), hunger(hunger), mood(mood), product(product) {}

// getAnimalName ����ʵ��
std::string Animal::getAnimalName() const {
    return name;
}

void Animal::feed() {
    hunger = std::max(0, hunger - 10);
    mood = std::min(100, mood + 10);
    std::cout << name << " has been fed. Hunger: " << hunger << ", Mood: " << mood << ".\n";
}

void  Animal::interact() {
    mood = std::min(mood + 5, 100);  // �øж����Ӻ�����Ҳ����
}


std::string Animal::produceProduct() {
    if (hunger > 50 || mood < 50) {
        return ""; // ���Ｂ����ʱ�򲻲�����Ʒ
    }
    return product;
}

void Animal::updateState() {
    hunger = std::min(100, hunger + 5);
    mood = std::max(0, mood - 5);
    if (hunger > 80) {
        health = std::max(0, health - 10);
    }
    std::cout << name << " updated. Health: " << health << ", Hunger: " << hunger << ", Mood: " << mood << ".\n";
}
