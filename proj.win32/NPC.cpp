/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     NPC.cpp
 * File Function: ʵ��NPC�࣬ʵ��NPC�Ĺ��� �Ի��洢������洢
 * Author:        ������  2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#include "NPC.h"
#include <ctime>
USING_NS_CC;

NPC* NPC::instance = nullptr;


// ��������
NPC::~NPC() {

}

Vec2 NPC::getNpcsPosition() const {
    return this->getPosition();  // ���� NPC ��λ��
}


NPC* NPC::create(NPCType type) {
    NPC* npc = new (std::nothrow) NPC();
    if (npc && npc->init(type)) {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

bool NPC::init(NPCType type) {
    if (!Sprite::init()) {
        return false;
    }

    this->type = type;

    std::string texturePath;
    switch (type) {
    case NPCType::ELLIOTT:
        texturePath = "NPC/Elliott.png";
        break;
    case NPCType::SAM:
        texturePath = "NPC/Sam.png";
        break;
    case NPCType::ABIGAIL:
        texturePath = "NPC/Abigail.png";
        break;
    case NPCType::SHANE:
        texturePath = "NPC/Shane.png";
        break;
    default:
        texturePath = "NPC/Default.png";
        CCLOG("No texturePath");
        break;
    }

    if (!this->initWithFile(texturePath)) {
        CCLOG("Failed to load NPC texture: %s", texturePath.c_str());
        return false;
    }

    auto texture = this->getTexture();
    if (texture) {
        auto size = texture->getContentSize();
        // ������4�з���13��֡
        widthSegment = size.width / 4;
        heightSegment = size.height / 13;
        this->setTextureRect(Rect(0, 0, widthSegment, heightSegment));
    }

    this->setPosition(Vec2(0, 0));
    this->scheduleUpdate();

    //// ������ʾ�øжȵĿ�
    //showAffectionBox();

    return true;
}

bool NPC::init() {
    if (!Node::init()) {
        return false;
    }
    // ��ĳ�ʼ���߼�
    createAndPositionNPCs(); // ���ô˺����Դ�������λNPC��
    return true;
}

NPC::NPCType NPC::getType() const {
    return type;
}

NPC* NPC::getInstance() {
    if (instance == nullptr) { // ���ʵ�������ڣ��򴴽�
        instance = NPC::create();
    }
    return instance; // ����Ψһʵ��
}

NPC* NPC::create() {
    NPC* npc = new (std::nothrow) NPC();
    if (npc && npc->init()) {
        npc->autorelease();
        return npc;
    }
    CC_SAFE_DELETE(npc);
    return nullptr;
}

void NPC::createAndPositionNPCs() {

    // ��ȡ��Ļ�ߴ�
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // �����ĸ�λ�ã����ϡ����¡����ϡ�����
    std::vector<Vec2> positions = {
        Vec2(origin.x + visibleSize.width * 0.2f, origin.y + visibleSize.height * 0.8f), // ����
        Vec2(origin.x + visibleSize.width * 0.2f, origin.y + visibleSize.height * 0.1f), // ����
        Vec2(origin.x + visibleSize.width * 0.8f, origin.y + visibleSize.height * 0.8f), // ����
        Vec2(origin.x + visibleSize.width * 0.8f, origin.y + visibleSize.height * 0.2f)  // ����
    };

    CCASSERT(npcTypes.size() == 4, "NPC������������ȷ");
    CCASSERT(positions.size() == 4, "NPCλ����������ȷ");

    for (int i = 0; i < 4; ++i) {
        NPC* npc = NPC::create(npcTypes[i]);
        if (npc) {
            npc->setPosition(positions[i]);
            npc->setScale(1.4f);
            this->addChild(npc);
            npcs.push_back(npc);
            CCLOG("Added NPC [%d] at position: (%f, %f)", i, positions[i].x, positions[i].y);

            // ��ʼ���Ի�
            npc->addDialogue("general", "The sun feels nice today.");
            npc->addDialogue("general", "Did you know the market is open?");
            npc->addDialogue("general", "What a beautiful day to be outside, don't you think?");
            npc->addDialogue("general", "The flowers are blooming wonderfully this season.");
            npc->addDialogue("general", "Farming must keep you busy. Do you ever get a day off?");
            npc->addDialogue("general", "The town feels livelier with you around!");
            npc->addDialogue("general", "Sometimes I wonder what lies beyond the mountains.");
            npc->addDialogue("general", "Every day is a new adventure in this little town.");
            npc->addDialogue("general", "The weather looks perfect for fishing today!");
            npc->addDialogue("general", "It��s amazing how quickly the seasons change, isn��t it?");
            npc->addDialogue("general", "I heard someone found an old treasure map in the attic!");
            npc->addDialogue("general", "Nothing beats the smell of freshly baked bread in the morning.");
            npc->addDialogue("general", "I wonder how deep the mines go. Have you explored them yet?");

            npc->addDialogue("task", "Could you plant a crop for me?\n I need it for a recipe.\n$350");
            npc->addDialogue("task", "The mines hold many secrets. \nCan you explore them and bring back something interesting?\n$1000");
            npc->addDialogue("task", "Could you send a gift to [NPC]? \nHe or she will be happy!\n$450");
            npc->addDialogue("task", "Your tools are looking a bit worn. \nMaybe it's time to buy an upgrade?\n$100");
            npc->addDialogue("task", "There��s an event happening in town. \nCan you attend and represent us?\n$500");

            npc->addDialogue("relationship_friendship", "I always enjoy spending time with you. Thanks for being here.");
            npc->addDialogue("relationship_friendship", "You��re such a reliable person! The town is lucky to have you.");
            npc->addDialogue("relationship_friendship", "I feel like I can always count on you. It means a lot.");
            npc->addDialogue("relationship_friendship", "Your kindness never ceases to amaze me.");
            npc->addDialogue("relationship_friendship", "I hope you know how much your support means to me.");
            npc->addDialogue("relationship_friendship", "Every time I see you, my day feels brighter.");

            npc->addDialogue("relationship_romantic", "I feel so at ease when I��m with you, like everything is just right.");
            npc->addDialogue("relationship_romantic", "You��re always on my mind. It��s a little embarrassing, but I��m glad.");
            npc->addDialogue("relationship_romantic", "I think we share something really special. It��s hard to put into words.");
            npc->addDialogue("relationship_romantic", "Whenever we��re apart, I find myself looking forward to the next time I see you.");
            npc->addDialogue("relationship_romantic", "Being with you makes me feel like I��m the luckiest person in the world.");
            npc->addDialogue("relationship_romantic", "You��ve stolen my heart completely, and I wouldn��t want it any other way.");
            npc->addDialogue("relationship_romantic", "Thank you for always believing in me. You��re my everything.");

            srand(static_cast<unsigned>(time(0)));

            // ����initialmove��NPC��ʼ�ƶ�
            npc->initialmove();

            // �����Ҫ����������NPC��ͼRect��Ҳ����ֱ�ӵ���setNPCDisplayRect
            // ���磺
            // npc->setNPCDisplayRect(Rect(0, 0, 32, 48)); // ��ȡ32x48����
        }
        else {
            CCLOG("Failed to create NPC of type %d", static_cast<int>(npcTypes[i]));
        }
    }
}
// NPC��update������ÿ֡���ã����ڸ��·��������״̬
void NPC::update(float dt) {
    // ���·���������Լ��߼�������ͼ֡�仯
    updateDirection(dt);
}

// ���㷽��͸�����ͼ֡�ĺ���(�򻯰�)
void NPC::updateDirection(float dt) {
    Vec2 currentPosition = this->getPosition();
    Vec2 direction = currentPosition - lastPosition;
    lastPosition = currentPosition;

    // ����directionȷ������֡
    // ��(down): col=0
    // ��(left): col=1
    // ��(right):col=2
    // ��(up):   col=3

    Rect rectForDirection;

    int col = 0; // Ĭ������
    // ��NPC��ֹ�����޸���ͼ
    if (direction.x > 0) {
        // �����ƶ�
        rectForDirection = Rect(0, heightSegment, widthSegment, heightSegment); // �������ƶ������ڵ�2��
    }
    else if (direction.x < 0) {
        // �����ƶ�
        rectForDirection = Rect(0, heightSegment * 3, widthSegment, heightSegment); // �������ƶ������ڵ�3��
    }
    else if (direction.y > 0) {
        // �����ƶ�
        rectForDirection = Rect(0, heightSegment * 2, widthSegment, heightSegment); // �������ƶ������ڵ�4��
    }
    else if (direction.y < 0) {
        // �����ƶ�
        rectForDirection = Rect(0, 0, widthSegment, heightSegment); // �������ƶ������ڵ�1��
    }

    // �����õ�һ��(row=0)��֡��Ϊ��̬������ʾ
    int row = 0;
    float x = col * widthSegment;
    float y = row * heightSegment;
    // ���ö�Ӧ�����Frame����
// ����NPC����������
    this->setTextureRect(rectForDirection);
}

// ��ʼ��NPC�Զ��ƶ�������һ��ѭ��·���������ƶ�
void NPC::initialmove() {
    Vec2 startPosition = this->getPosition();
    lastPosition = startPosition;

    // ����һ��ѭ���ƶ�·��
    std::vector<Vec2> path = {
        Vec2(startPosition.x, startPosition.y),
        Vec2(startPosition.x + 100, startPosition.y),
        Vec2(startPosition.x+50, startPosition.y ),
        Vec2(startPosition.x, startPosition.y)
    };

    moveAlongPath(path);
}

// NPC��·���ƶ��ĺ���
void NPC::moveAlongPath(const std::vector<Vec2>& path) {
    if (path.size() < 2) return;

    FiniteTimeAction* sequence = nullptr;
    for (size_t i = 0; i < path.size() - 1; ++i) {
        auto moveAction = MoveTo::create(3.5f, path[i + 1]);
        if (!sequence) {
            sequence = moveAction;
        }
        else {
            sequence = Sequence::create(sequence, moveAction, nullptr);
        }
    }

    auto repeatAction = RepeatForever::create((ActionInterval*)sequence);
    this->runAction(repeatAction);
}

void NPC::setAffection(int affection) {
    _affection = affection;
    /*if (_affectionLabel) {
        _affectionLabel->setString("Affection: " + std::to_string(_affection));
    }*/
}

int NPC::getAffection() const {
    return _affection;
}

void NPC::setRelationship(const std::string& relationship) {
    _relationship = relationship;
    if (_relationshipLabel) {
        _relationshipLabel->setString("Relationship: " + _relationship);
    }
}

std::string NPC::getRelationship() const {
    return _relationship;
}

void NPC::showAffectionBox() {
    // ��ȡ��Ļ�ߴ�
    Size screenSize = Director::getInstance()->getWinSize();

    // ����һ���̶�λ�õ���壬���������һ��͸�������ľ���
    auto panel = Sprite::create("an_menu.png");  // ��������һ������ͼƬ
    panel->setPosition(screenSize.width / 2, screenSize.height / 2);  // �����ʾ����Ļ����
    panel->setScale(3.0f);
    this->getParent()->addChild(panel);

    // ��ȡ���� NPC
    auto npcManager = NPC::getInstance();
    const auto& npcs = npcManager->getAllNPCs();

    // �����ı���ǩ����ʾÿ�� NPC �ĺøжȺ͹�ϵ
    float yOffset = 50.0f;  // ÿ�� NPC ��Ϣ�Ĵ�ֱ���
    int count = 1;

    for (auto npc : npcs) {
        // ����һ����ʾ NPC ���֡��øжȺ͹�ϵ���ı�
        std::string text = "NPC " + std::to_string(count) + ":\n" +
            "Affection: " + std::to_string(npc->_affection) + "\n" +
            "Relationship: " + npc->_relationship;


        auto label = Label::createWithTTF(text, "../Resources/fonts/Marker Felt.ttf", 18);
        if (label) {

            label->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 100 + yOffset));
            label->setColor(Color3B::BLACK);  // �����ı���ɫ
            this->getParent()->addChild(label);  // ��ӵ����ڵ�
        }

        // ����ƫ������ȷ��ÿ�� NPC ����Ϣ�����ص�
        yOffset -= 100.0f;  // ÿ�� NPC ����Ϣ����
        count++;
    }
}
void NPC::addDialogue(const std::string& category, const std::string& dialogue) {
    categorizedDialogues[category].push_back(dialogue);
}


std::string NPC::getRandomDialogue(const std::string& category) const {
    // ����Ƿ���ڸ÷���
    auto it = categorizedDialogues.find(category);
    if (it == categorizedDialogues.end() || it->second.empty()) {
        CCLOG("No dialogues found for category: %s", category.c_str());
        return ""; // ���ؿ��ַ�����ʾû�п��öԻ�
    }

    // �Ӷ�Ӧ���������ѡ��һ���Ի�
    const auto& dialogues = it->second;
    int randomIndex = rand() % dialogues.size(); // ʹ���������
    return dialogues[randomIndex];
}

 
void NPC::displayDialogueBox(const std::string& category) {
    // ��ȡ����Ի�
    std::string dialogue = getRandomDialogue(category);

    // �����Ի��򱳾�
    Size screenSize = Director::getInstance()->getWinSize();
    auto panel = Sprite::create("dialogue_panel.png"); // �Ի��򱳾�ͼƬ·��
    panel->setPosition(Vec2(screenSize.width / 2, 0)); // ��ʾ����Ļ�·�����
    panel->setScale(2.0f);
    this->getParent()->addChild(panel);

    // ���������ʾ�Ի�����
    auto label = Label::createWithTTF(dialogue, "fonts/Marker Felt.ttf", 20);
    label->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 3);
    label->setColor(Color3B::BLACK);
    panel->addChild(label);

    // ����3����Զ��Ƴ��Ի���
    auto delay = DelayTime::create(3.0f);
    auto remove = CallFunc::create([panel]() { panel->removeFromParent(); });
    auto sequence = Sequence::create(delay, remove, nullptr);
    panel->runAction(sequence);
}



void NPC::removeDialogue(const std::string& category, const std::string& dialogue) {
    auto itCategory = categorizedDialogues.find(category);
    if (itCategory != categorizedDialogues.end()) {
        auto& dialogues = itCategory->second;
        auto it = std::find(dialogues.begin(), dialogues.end(), dialogue);
        if (it != dialogues.end()) {
            dialogues.erase(it);
        }
    }
}


const std::vector<std::string>& NPC::getDialogues(const std::string& category) const {
    // ��������Ӧ�ĶԻ��б�
    auto it = categorizedDialogues.find(category);
    if (it != categorizedDialogues.end()) {
        return it->second; // ���ض�Ӧ���ĶԻ�
    }
    // �����𲻴��ڣ�����һ�����б�
    static const std::vector<std::string> emptyList;
    return emptyList;
}