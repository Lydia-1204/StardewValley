#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class MapManager : public Node {
private:
    static MapManager* instance;     // ����ʵ��

    TMXTiledMap* currentMap;         // ��ǰ���صĵ�ͼ
    Vec2 playerStartPos;             // ��ҳ�ʼλ��
    std::string nickname;            // ����ǳ�
    int selectedCharacter;           // ���ѡ��Ľ�ɫ
    std::map<std::string, TMXTiledMap*> mapPool; // ��ͼ�����

    Vec2 chickenStartPos;             // С����ʼλ��
    Vec2 catStartPos;             // С����ʼλ��
    Vec2 dogStartPos;             // С����ʼλ��
    Vec2 cowStartPos;             // С����ʼλ��
    Vec2 sheepStartPos;             // С����ʼλ��
    Vec2 pigStartPos;             // С����ʼλ��

    Vec2 cropStartPos;             // ũ�����ʼλ��




public:
    static MapManager* getInstance(); // ��ȡ����ʵ��
    MapManager();                    // ˽�л����캯��
    bool init();                      // ��ʼ��

    void loadMapBlocks(const std::string& mapFolder); // �������е�ͼ
    TMXTiledMap* getCurrentBlock();   // ��ȡ��ǰ��ͼ��
    Vec2 getPlayerStartPos();         // ��ȡ��ҳ�ʼλ��

    void switchToBlock(const Vec2& direction); // �л���ָ����ͼ��
    bool isAtEdge(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ񵽴�߽�

    Vec2 getChickenStartPos();         // ��ȡС����ʼλ��
    Vec2 getCatStartPos();         // ��ȡС����ʼλ��
    Vec2 getDogStartPos();         // ��ȡС����ʼλ��
    Vec2 getCowStartPos();         // ��ȡС����ʼλ��
    Vec2 getSheepStartPos();         // ��ȡС����ʼλ��
    Vec2 getPigStartPos();         // ��ȡС����ʼλ��

    Vec2 getCropStartPos();         // ��ȡС����ʼλ��

    CREATE_FUNC(MapManager);
};

#endif // MAP_MANAGER_H