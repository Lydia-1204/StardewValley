#pragma once
#ifndef CHICKEN_H
#define CHICKEN_H

#include "Animal.h"

class Chicken : public Animal {
private:
    int eggYield;  // ��������

public:
    Chicken(const std::string& name, int health, int hunger, int mood, int eggYield);

    // ��д�������ɷ��������ɼ���
    std::string produceProduct() override;
};

#endif // CHICKEN_H
