#pragma once
#ifndef CHICKEN_H
#define CHICKEN_H

#include "Animal.h"

class Chicken : public Animal {
private:
    int eggYield;  // 鸡蛋产量

public:
    Chicken(const std::string& name, int health, int hunger, int mood, int eggYield);

    // 重写产物生成方法，生成鸡蛋
    std::string produceProduct() override;
};

#endif // CHICKEN_H
