#pragma once
#ifndef COW_H
#define COW_H

#include "Animal.h"

class Cow : public Animal {
private:
    int milkYield;  // 牛奶产量

public:
    Cow(const std::string& name, int health, int hunger, int mood, int milkYield);

    // 重写产物生成方法，生成牛奶
    std::string produceProduct() override;
};

#endif // COW_H
