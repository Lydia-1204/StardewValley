#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class MapManager : public Node {
private:
    static MapManager* instance;     // ����ʵ��

    std::map<std::string, TMXTiledMap*> mapPool; // ��ͼ�����
    cocos2d::TMXTiledMap* currentMap;         // ��ǰ���صĵ�ͼ
    Vec2 playerStartPos;             // ��ҳ�ʼλ��
    std::string nickname;            // ����ǳ�
    int selectedCharacter;           // ���ѡ��Ľ�ɫ
    
    float scaleX;
    float scaleY;
    float scale; //���ű���
   

public:
    static MapManager* getInstance(); // ��ȡ����ʵ��
    MapManager();                    // ˽�л����캯��
    bool init();                      // ��ʼ��

    void loadMapBlocks(const std::string& mapFolder); // �������е�ͼ
    TMXTiledMap* getCurrentBlock();   // ��ȡ��ǰ��ͼ��
    Vec2 getPlayerStartPos();         // ��ȡ��ҳ�ʼλ��

    void switchToBlock(const Vec2& direction); // �л���ָ����ͼ��
    bool isAtEdge(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ񵽴�߽�
    Size getCurrentMapSize(bool inPixels) const;// inPixels=1ʱ ����������Ϊ��λ�ĵ�ͼ��С��=0�����Ը�����Ϊ��λ�ĵ�ͼ��С
    CREATE_FUNC(MapManager);
};

#endif // MAP_MANAGER_H