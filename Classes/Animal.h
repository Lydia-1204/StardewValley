#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include "cocos2d.h"

class Animal : public cocos2d::Sprite {
protected:
    std::string name;  // ��������
    int health;        // ����ֵ
    int hunger;        // ������
    int mood;          // ����ֵ
    std::string product;  // �������

public:
    Animal(const std::string& name, int health, int hunger, int mood, const std::string& product);  // ֻ�������캯��

    std::string getAnimalName() const;

    // ι��������ӱ�ʳ�Ⱥ�����
    virtual void feed();

    // ������������ߺøж�
    void interact();

    // ���ɲ���
    virtual std::string produceProduct();

    // ���¶���״̬
    virtual void updateState();
};

#endif // ANIMAL_H
