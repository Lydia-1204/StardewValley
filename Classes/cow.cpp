#include "Cow.h"

Cow::Cow(const std::string& name, int health, int hunger, int mood, int milkYield)
    : Animal(name, health, hunger, mood, "Milk"), milkYield(milkYield) {}

std::string Cow::produceProduct() {
    if (hunger > 50 || mood < 50) {
        return "";
    }
    return "Milk: " + std::to_string(milkYield);
}
