#ifndef MAP_MANAGER_H
#define MAP_MANAGER_H

#include "cocos2d.h"
#include <string>

USING_NS_CC;

class MapManager : public Node {
    friend class UIManager;
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
    //--------------------����--------------------------//
    Vec2 chickenStartPos;             // С����ʼλ��
    Vec2 catStartPos;             // С����ʼλ��
    Vec2 dogStartPos;             // С����ʼλ��
    Vec2 cowStartPos;             // С����ʼλ��
    Vec2 sheepStartPos;             // С����ʼλ��
    Vec2 pigStartPos;             // С����ʼλ��

    Vec2 cropStartPos;             // ֲ���ʼλ��

    MapManager() ;                  // ˽�л����캯��


public:
    static MapManager* getInstance(); // ��ȡ����ʵ��
    int currentMapLabel;
    bool init();                      // ��ʼ��

    void loadMapBlocks(const std::string& mapFolder); // �������е�ͼ
    TMXTiledMap* getCurrentBlock();   // ��ȡ��ǰ��ͼ��
    int getCurrentBlockLabel();   // ��ȡ��ǰ��ͼ��label
    Vec2 getPlayerStartPos();         // ��ȡ��ҳ�ʼλ��

    void switchToBlock(const Vec2& direction); // �л���ָ����ͼ��
    void switchToHouseOrOutside(int label);   //�л�������/�����ͼ
    bool isAtEdge(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ񵽴�߽�


    bool isEnterHouse(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ�����
    bool isEnterShop(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ�����̵�
    bool isExitHouse(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ��뿪��
    bool isExitShop(const Vec2& playerPos, Vec2& outDirection); // ����Ƿ��뿪�̵�

    Size getCurrentMapSize(bool inPixels) const;// inPixels=1ʱ ����������Ϊ��λ�ĵ�ͼ��С��=0�����Ը�����Ϊ��λ�ĵ�ͼ��С


    //�������������������������������������
    Vec2 getChickenStartPos();         // ��ȡС����ʼλ��
    Vec2 getCatStartPos();         // ��ȡС����ʼλ��
    Vec2 getDogStartPos();         // ��ȡС����ʼλ��
    Vec2 getCowStartPos();         // ��ȡС����ʼλ��
    Vec2 getSheepStartPos();         // ��ȡС����ʼλ��
    Vec2 getPigStartPos();         // ��ȡС����ʼλ��

    Vec2 getCropStartPos();         // ��ȡС����ʼλ��


    //�빤���ࡪ����������������������������������
    Vec2 getTileCoordForPosition(const Vec2& position);
    bool isNearWater(const Vec2& playerPos);    //�ж��Ƿ�ˮ�ߣ����㽻����
    bool isObstacleAtPosition(const Vec2& position);//��ײ���
    bool isDiggable(const Vec2& position); //�ھ��⣨����������
    void setTileState(const Vec2& tileCoord, const std::string& state); //����tile��state
    //Tree* getTreeAtPosition(const Vec2& position); //����һ��tree��
    //Crop* getCropAtPosition(const Vec2& position); //����һ��crop��

    Vec2 absoluteToTile(const Vec2& position);//��������->��ͼ��Ƭ����
    Vec2 tileToAbsolute(const Vec2& tileCoord);//��Ƭ����->�������꣨�е㣩
    int getTileGid(const std::string& layerName, const Vec2& tileCoord);

    CREATE_FUNC(MapManager);
};

#endif // MAP_MANAGER_H