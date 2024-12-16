/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Animal.h
 * File Function: ʵ��Animal�࣬��ɶ�����ֳ����
 * Author:        ���庬 2351591
 * Update Date:   2024/12/
 ****************************************************************/
#ifndef ANIMAL_H
#define ANIMAL_H

#include <string>
#include "cocos2d.h"
#include <chrono>

USING_NS_CC;

class Animal : public cocos2d::Node {
private:
    friend class AnimalManager;
    Sprite* an_sprite;   // �����ɫ����
    Sprite* animal[4];  // ����һ������ 4 �����������
    int d;                      // ��ǰ�����������0: ����, 1: ����, 2: ���, 3: ����
    Vec2 lastPosition;  
    std::string _picturename;  // ��������
    AnimalManager* manager;
    bool wasDoubleClick = false;  // ����Ƿ�����˫��
    std::chrono::steady_clock::time_point lastClickTime;  // �ϴε����ʱ��

    // ��ӻ�ȡ�������͵�ö�ٺͷ���
    enum class AnimalType {
        CHICKEN,
        COW,
        CAT,
        DOG,
        PIG,
        SHEEP
    };

    std::string name;  // ��������
    int health;        // ����ֵ
    int mood;          // ����ֵ
    int affection;     // ����ֵ
    std::string product;  // �������


    bool hasWaterInBowl;  // �Ƿ���ˮ

   

public:
    Animal(std::string name, int health, int hunger, int mood, std::string product, int affection, bool hasWaterInBowl, bool hasHeart, Vec2 lastPosition)
        : name(name), health(health), mood(mood), product(product), affection(affection), hasWaterInBowl(hasWaterInBowl), lastPosition(lastPosition) {
        // ������Խ���һЩ��ʼ�����������紴�������
        // an_sprite = Sprite::create("path_to_sprite"); // ������һ����������ķ���
    }

    Animal() : an_sprite(nullptr), health(0), mood(0), affection(0), hasWaterInBowl(false), lastPosition(100, 200) {
        // Ĭ�Ϲ��캯������Ա������ʼ��ΪĬ��ֵ
    }
    static Animal* create(int targetAnimal, const std::string& nickname);
    bool init(int targetAnimal, const std::string& nickname);
    Vec2 getCurrentDirection();
    void updateDirection(float dt, const std::string& picturename);
    void moveAlongPath(const std::vector<Vec2>& path, const std::string& picturename);
    void update(float dt);
    void initialmove(const std::string& picturename);
    void onMouseDown(Event* event, std::string animalName);
    void removeHappySprite(float dt);


    void updateFavorability();
    void updateMood();
    void feed();


    std::string getAnimalName() const;
    int getHealth() const;
    int getMood() const;
    int getAffection() const;

    AnimalType getAnimalType() const;

    // ���ɲ���
    virtual std::string produceProduct();

    bool getHasWaterInBowl() { return hasWaterInBowl; }
    void setHasWaterInBowl(bool status) { hasWaterInBowl = status; }

    void placeWaterBowl(const Vec2& position);
    bool checkWaterBowlImage(Node* node);


    // �жϳ����Ƿ�ﵽ1000��øж�
    bool isMaxAffection() {
        return affection == 1000;
    }

    // ���³���״̬�����磬���������Ƿ���ˮ��
    void updateWater(bool isRainyDay) {
        if (isRainyDay) {
            hasWaterInBowl = true;  // ���죬���Զ�����ˮ
        }

        // ���û����ˮ�����ٺøж�
        if (!hasWaterInBowl) {
            affection -= 10;
            if (affection < 0) affection = 0;  // ȷ���øжȲ���Ϊ��
        }
    }
};

#endif // ANIMAL_H
