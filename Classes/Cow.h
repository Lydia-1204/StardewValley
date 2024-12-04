#pragma once
#ifndef COW_H
#define COW_H

#include "Animal.h"

class Cow : public Animal {
private:
    int milkYield;  // ţ�̲���

public:
    Cow(const std::string& name, int health, int hunger, int mood, int milkYield);

    // ��д�������ɷ���������ţ��
    std::string produceProduct() override;
};

#endif // COW_H
