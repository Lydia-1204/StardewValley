/**********************************************************************************************
 * Project Name:  StardewValley
 * File Name:     GameScene.h
 * File Function: ����GameScene�࣬����ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI 
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 **********************************************************************************************/

#ifndef GAME_SCENE_H
#define GAME_SCENE_H

#include "cocos2d.h"
#include "Player.h"
#include "map.h"
#include "UIManager.h"
#include "Animal.h"
#include "AnimalManager.h"

USING_NS_CC;

class GameScene : public Scene {
private:
    static GameScene* instance;   // ����ʵ��

    Player* player;               // ��ҽ�ɫ
    MapManager* mapManager;       // ��ͼ������
    UIManager* uiManager;         // UI ������
    std::string nickname;
    int selectedCharacter;

    Animal* chicken;            // ����    
    Animal* cat;            // ����       
    Animal* dog;            // ����      
    Animal* pig;            // ����       
    Animal* sheep;            // ����       
    Animal* cow;            // ����       
    AnimalManager* manager = new AnimalManager();


    float gameTime;  //ʱ������ ��λΪ��
    int currentHour;   //Сʱ
    int currentMinute;    //����

public:
    static GameScene* getInstance(int selectedCharacter, const std::string& nickname);     // ��ȡ����ʵ��
    static GameScene* createScene(int selectedCharacter, const std::string& nickname);     // ��������

    virtual bool init(int selectedCharacter, const std::string& nickname);        // ��ʼ��
    virtual void update(float dt) override; // ÿ֡����

    Player* getPlayer();                 // ��ȡ���ʵ��

    void pauseGame();                    // ��ͣ��Ϸ
    void resumeGame();                   // �ָ���Ϸ

    // ��������
    void replaceChild(Node* oldChild, Node* newChild);
    GameScene();
    virtual~GameScene()= default;
};

#endif // GAME_SCENE_Hwdas