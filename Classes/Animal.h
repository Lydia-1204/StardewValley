#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include "cocos2d.h"

class Animal : public cocos2d::Sprite {
protected:
    std::string name;  // 动物名称
    int health;        // 健康值
    int hunger;        // 饥饿度
    int mood;          // 心情值
    std::string product;  // 动物产物

public:
    Animal(const std::string& name, int health, int hunger, int mood, const std::string& product);  // 只声明构造函数

    std::string getAnimalName() const;

    // 喂养动物，增加饱食度和心情
    virtual void feed();

    // 交互方法：提高好感度
    void interact();

    // 生成产物
    virtual std::string produceProduct();

    // 更新动物状态
    virtual void updateState();
};

#endif // ANIMAL_H
