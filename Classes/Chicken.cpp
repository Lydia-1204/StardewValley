#include "Chicken.h"

Chicken::Chicken(const std::string& name, int health, int hunger, int mood, int eggYield)
    : Animal(name, health, hunger, mood, "Egg"), eggYield(eggYield) {}

std::string Chicken::produceProduct() {
    if (hunger > 50 || mood < 50) {
        return "";
    }
    return "Eggs: " + std::to_string(eggYield);
}
