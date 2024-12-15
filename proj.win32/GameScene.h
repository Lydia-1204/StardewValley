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
#include"tool.h"
USING_NS_CC;

class GameScene : public Scene {
private:
    static GameScene* instance;   // ����ʵ��

    bool isGamePaused; //�Ƿ���ͣ
    Player* player;               // ��ҽ�ɫ
    MapManager* mapManager;       // ��ͼ������
    TMXTiledMap* currentMap; //��ǰ��ͼ��
    UIManager* uiManager;         // UI ������
    ToolManager* toolManager;
    std::string nickname;  //�ǳ�
    int selectedCharacter; // 1 -Amily/2-Harvey
    Size screenSize;
    float gameTime;  //ʱ������ ��λΪ��
    int currentHour;   //Сʱ
    int currentMinute;    //����

public:
    static GameScene* getInstance(int& selectedCharacter, const std::string& nickname);     // ��ȡ����ʵ��
    static GameScene* createScene(int& selectedCharacter, const std::string& nickname);     // ��������

    virtual bool init(int& selectedCharacter, const std::string& nickname);        // ��ʼ��
    virtual void update(float dt) override; // ÿ֡����
    void togglePause();                    //�л���ͣ״̬
    Player* getPlayer();                 // ��ȡ���ʵ��

    void pauseGame();                    // ��ͣ��Ϸ
    void resumeGame();                   // �ָ���Ϸ

    // ��������
    void replaceChild(Node* oldChild, Node* newChild);
    GameScene();
    virtual~GameScene()= default;
    void initKeyboardListener();    //��ݼ�
};

#endif // GAME_SCENE_Hwdas