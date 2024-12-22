/****************************************************************
 * Project Name:  StardewValley
 * File Name:     Crop.h
 * File Function: ʵ��Crop�࣬���ũ������ֲ����
 * Author:        ���庬 2351591
 * Update Date:   2024/12/22
 ****************************************************************/

#pragma once
#ifndef CROP_H
#define CROP_H

#include "cocos2d.h"
#include <string>
#include <vector>
#include <functional>
USING_NS_CC;
class GameScene; // ǰ������ GameScene ��

class Crop : public Sprite {
public:
    enum class CropState {
        Seed, Growing, Mature, Dead
    };

    Crop();  // Ĭ�Ϲ��캯��
    static Crop* create(GameScene* scene, const std::string& nickname);
    void setGameScene(GameScene* scene);
    bool init(const std::string& nickname);
    void onMouseDown(EventMouse* event);

    void grow();  // ��������״̬
    void water();  // ��ˮ
    void fertilize();    // ʩ��
    void update(float dt);  // ÿ֡���£�����ʱ���������״̬
    void changeTexture(const std::string& texturePath);
    void removeSprite(float dt);
    void deleteDead(Sprite* deadSprite);
    void getGameScene(GameScene* scene);
    CropState getState() const;

    // ����ص���������
    using TextureChangedCallback = std::function<void(const std::string&)>;

    // ���ûص�����
    void setTextureChangedCallback(const TextureChangedCallback& callback);
   
private:
    TextureChangedCallback textureChangedCallback;
    GameScene* gameScene;  // ���ڴ洢��GameScene������
    Sprite* myCrop;
    CropState state;   // ��ǰ״̬
    int age;           // ��ǰ����ʱ�䣨����Ϸ�е���Ϊ��λ��
    int newAge = 0;
    int lastWateredTime;
    float timeElapsed;
    Sprite* deadSprite;
    std::vector<std::string> stageTextures;  // ��ͬ�����׶ε�ͼƬ·��
    bool watered;      // �Ƿ�ˮ
};

#endif // CROP_H