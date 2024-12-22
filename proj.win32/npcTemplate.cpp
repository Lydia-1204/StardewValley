#include "npcTemplate.h"
#include <ctime>
USING_NS_CC;


// ��������
NpcTemplate::~NpcTemplate() {

}

Vec2 NpcTemplate::getNpcsPosition() const {
    return this->getPosition();  // ���� NPC ��λ��
}

bool NpcTemplate::init(NPCType type, Vec2 position) {
    if (!Sprite::init()) {
        return false;
    }
    if (type == NPCType::ELLIOTT)name = "Elliott";
    if (type == NPCType::SHANE)name = "Shane";
    if (type == NPCType::SAM)name = "Sam";
    if (type == NPCType::ABIGAIL)name = "Abigail";

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

    this->setPosition(position);
    this->scheduleUpdate();

    //// ������ʾ�øжȵĿ�
    //showAffectionBox();

    return true;
}

NPCType NpcTemplate::getType() const {
    return type;
}

/*
NpcTemplate* NpcTemplate::create(NPCType npcType, Vec2 startPoint) {
    NpcTemplate* npc = new (std::nothrow) NpcTemplate(npcType, startPoint);
    if (npc && npc->init()) {
        npc->autorelease();  // ȷ�������ڲ���ʹ��ʱ�ܹ��Զ��ͷ�
        return npc;
    }
    CC_SAFE_DELETE(npc);  // �����ʼ��ʧ�ܣ���ɾ������
    return nullptr;
}
*/


/***********************************************
NpcTemplate* NpcTemplate::getNpcInstance(NPCType npcType, Vec2 startPoint = cocos2d::Vec2(0, 0)) {
    // ����Ƿ����и����͵� NPC ʵ��
    for (NpcTemplate* npc : npcInstances) {
        if (npc->getType() == npcType) {
            return npc;
        }
    }

    // ���û���ҵ������͵�ʵ���������µ� NPC ʵ��
    NpcTemplate* newNpc = NpcTemplate::create(npcType, startPoint);
    npcInstances.push_back(newNpc);  // ���´����� NPC ���� npcInstances ������
    return newNpc;
    /*
    if (npcInstances.find(npcType) == npcInstances.end()) {
        // ��������͵�ʵ����û�д���
        NpcTemplate* instance = nullptr;
        switch (npcType) {
        case NPCType::ELLIOTT:
            instance = Elliott::getInstance(startPoint);
            break;
        case NPCType::SAM:
            instance = Sam::getInstance(startPoint);
            break;
        case NPCType::ABIGAIL:
            instance = Abigail::getInstance(startPoint);
            break;
        case NPCType::SHANE:
            instance = Shane::getInstance(startPoint);
            break;
        }
        npcInstances[npcType] = instance;
    }
    return npcInstances[npcType];    
}
*/


/*
NpcTemplate* NpcTemplate::getInstance() {
    if (instance == nullptr) { // ���ʵ�������ڣ��򴴽�
        instance = NpcTemplate::create();
    }
    return instance; // ����Ψһʵ��
}
*/

void NpcTemplate::addGeneralDialogue() {
    this->addDialogue("general", "The sun feels nice today.");
    this->addDialogue("general", "Did you know the market is open?");
    this->addDialogue("general", "What a beautiful day to be outside, don't you think?");
    this->addDialogue("general", "The flowers are blooming wonderfully this season.");
    this->addDialogue("general", "Farming must keep you busy. Do you ever get a day off?");
    this->addDialogue("general", "The town feels livelier with you around!");
    this->addDialogue("general", "Sometimes I wonder what lies beyond the mountains.");
    this->addDialogue("general", "Every day is a new adventure in this little town.");
}

/*

void NpcTemplate::createAndPositionNPCs() {
    //NpcTemplate* npc = NpcTemplate::create(type, movePathStartPoint);
    //npc->setPosition(movePathStartPoint);
    npc->setScale(1.4f);
    this->addChild(npc);
    npcInstances.push_back(npc);
    CCLOG("Added NPC : [%s] at position: (%f, %f)", type, movePathStartPoint.x,movePathStartPoint.y);

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
    npc->addDialogue("task", "Could you send a gift to [NPC]? she is around your home, \nHe or she will be happy!\n$450");
    npc->addDialogue("task", "Your tools are looking a bit worn. \nMaybe it's time to buy an upgrade?\n$100");
    npc->addDialogue("task", "There��s an event happening in town. \nCan you attend and represent us?\n$500");            
    


    srand(static_cast<unsigned>(time(0)));

    // ����initialmove��NPC��ʼ�ƶ�
    npc->initialmove();

    // �����Ҫ����������NPC��ͼRect��Ҳ����ֱ�ӵ���setNPCDisplayRect
    // ���磺
    // npc->setNPCDisplayRect(Rect(0, 0, 32, 48)); // ��ȡ32x48����
}
*/


// NPC��update������ÿ֡���ã����ڸ��·��������״̬
void NpcTemplate::update(float dt) {
    // ���ºøжȼ���Ӧ�Ի�
    if (affection >= 30 && affection < 70) {
        //30~69����friendship
        addDialogue("relationship_friendship", "I always enjoy spending time with you. Thanks for being here.");
        addDialogue("relationship_friendship", "You��re such a reliable person! The town is lucky to have you.");
        addDialogue("relationship_friendship", "I feel like I can always count on you. It means a lot.");
        addDialogue("relationship_friendship", "Your kindness never ceases to amaze me.");
        addDialogue("relationship_friendship", "I hope you know how much your support means to me.");
        addDialogue("relationship_friendship", "Every time I see you, my day feels brighter.");
        relationship = "friendship";
    }
    else if (affection >= 70 ) {
        //30~69����friendship
        addDialogue("relationship_romantic", "I feel so at ease when I��m with you, like everything is just right.");
        addDialogue("relationship_romantic", "You��re always on my mind. It��s a little embarrassing, but I��m glad.");
        addDialogue("relationship_romantic", "I think we share something really special. It��s hard to put into words.");
        addDialogue("relationship_romantic", "Whenever we��re apart, I find myself looking forward to the next time I see you.");
        addDialogue("relationship_romantic", "Being with you makes me feel like I��m the luckiest person in the world.");
        addDialogue("relationship_romantic", "You��ve stolen my heart completely, and I wouldn��t want it any other way.");
        addDialogue("relationship_romantic", "Thank you for always believing in me. You��re my everything.");
        //��ɾ��friendship�ĶԻ�
        removeDialogues("relationship_friendship");

        relationship = "romantic";
        //setRelationship("romantic");
    }


    // ���·���������Լ��߼�������ͼ֡�仯
    updateDirection(dt);
}

// ���㷽��͸�����ͼ֡�ĺ���(�򻯰�)
void NpcTemplate::updateDirection(float dt) {
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
void NpcTemplate::initialmove() {
    Vec2 startPosition = this->getPosition();
    lastPosition = startPosition;

    // ����һ��ѭ���ƶ�·��
    std::vector<Vec2> path = {
        Vec2(startPosition.x, startPosition.y),
        Vec2(startPosition.x + 100, startPosition.y),
        Vec2(startPosition.x + 50, startPosition.y),
        Vec2(startPosition.x, startPosition.y)
    };

    moveAlongPath(path);
}

// NPC��·���ƶ��ĺ���
void NpcTemplate::moveAlongPath(const std::vector<Vec2>& path) {
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

void NpcTemplate::setAffection(int affection) {
    this->affection = affection;
    /*if (_affectionLabel) {
        _affectionLabel->setString("Affection: " + std::to_string(_affection));
    }*/
}

int NpcTemplate::getAffection() const {
    return affection;
}

void NpcTemplate::setRelationship(const std::string& relationship) {
    this->relationship = relationship;
    if (relationshipLabel) {
        relationshipLabel->setString("Relationship: " + relationship);
    }
}

std::string NpcTemplate::getRelationship() const {
    return relationship;
}

void NpcTemplate::showAffectionBox() {
    // ��ȡ��Ļ�ߴ�
    Size screenSize = Director::getInstance()->getWinSize();

    // ����һ���̶�λ�õ���壬���������һ��͸�������ľ���
    auto panel = Sprite::create("an_menu.png");  // ��������һ������ͼƬ
    panel->setPosition(screenSize.width / 2, screenSize.height / 2);  // �����ʾ����Ļ����
    panel->setScale(3.0f);
    this->getParent()->addChild(panel);


    // ��ȡ���� NPC
    std::vector<NpcTemplate*> npcs;
    npcs.push_back(Elliott::getInstance());
    npcs.push_back(Sam::getInstance());
    npcs.push_back(Shane::getInstance());
    npcs.push_back(Abigail::getInstance());

    // �����ı���ǩ����ʾÿ�� NPC �ĺøжȺ͹�ϵ
    float yOffset = 50.0f;  // ÿ�� NPC ��Ϣ�Ĵ�ֱ���
    int count = 1;

    for (auto npc : npcs) {
        // ����һ����ʾ NPC ���֡��øжȺ͹�ϵ���ı�
        std::string text = "NPC " + npc->name + ":\n" +
            "Affection: " + std::to_string(npc->affection) + "\n" +
            "Relationship: " + npc->relationship;


        npc->affectionLabel = Label::createWithTTF(text, "../Resources/fonts/Marker Felt.ttf", 18);
        if (npc->affectionLabel) {
            npc->affectionLabel->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 100 + yOffset));
            //label->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));

            npc->affectionLabel->setColor(Color3B::BLACK);  // �����ı���ɫ
            this->getParent()->addChild(npc->affectionLabel, 10);  // ��ӵ����ڵ�
            npc->affectionLabel->setName(npc->name);
        }
        
        // ����ƫ������ȷ��ÿ�� NPC ����Ϣ�����ص�
        yOffset -= 100.0f;  // ÿ�� NPC ����Ϣ����
        count++;

    }        
    // �رհ�ť
        auto closeButton = MenuItemImage::create("../Resources/close.png", "../Resources/close.png",
            [=](Ref* sender) {
                this->getParent()->removeChild(panel);  // �Ӹ��ڵ�ɾ��
                this->getParent()->removeChildByName("Elliott");
                this->getParent()->removeChildByName("Shane");
                this->getParent()->removeChildByName("Sam");
                this->getParent()->removeChildByName("Abigail");

            });
        closeButton->setScale(5.0f);

        // ���رհ�ť�����ڶԻ�������Ͻ�
        auto menu = Menu::create(closeButton, nullptr);
        menu->setPosition(panel->getContentSize().width, panel->getContentSize().height);

        closeButton->setScale(2.0f);
        panel->addChild(menu);

}

//*********************************************************************************88888
void NpcTemplate::addDialogue(const std::string& category, const std::string& dialogue) {
    categorizedDialogues[category].push_back(dialogue);
}

//***************************************************************************
std::string NpcTemplate::getRandomDialogue()  {

    // ʹ�������������
    std::srand(std::time(0));  // �����������

    // 1. ���ѡ��һ�����ࣨ����
    std::vector<std::string> categories;
    for (const auto& pair : categorizedDialogues) {
        categories.push_back(pair.first);  // �����з������ƣ�������ӵ�һ�� vector ��
    }

    // ���ѡ��һ�����������
    int categoryIndex = std::rand() % categories.size();
    std::string randomCategory = categories[categoryIndex];
    while (randomCategory == "task") {
        //����������task��������
        categoryIndex = std::rand() % categories.size();
        randomCategory = categories[categoryIndex];
    }

    // 2. �Ӹ÷����Ӧ�� vector �����ѡ��һ���Ի�
    const std::vector<std::string>& dialogues = categorizedDialogues[randomCategory];

    // ���ѡ��һ���Ի�������
    int dialogueIndex = std::rand() % dialogues.size();
    std::string randomDialogue = dialogues[dialogueIndex];
    
    //��������Ի�
    return randomDialogue;
}


//******************************************************************88888
void NpcTemplate::displayDialogueBox(int index) {
    isOpenDialogueBox = true;
    std::string dialogue, npcDialogue;

    if (index == 0) {
        if (!haveFinishTask) {
            //����npc������δ��ɣ�����չʾtask��dialogue
            dialogue = getDialogue("task", index);  
            //��ʾxxx: dialogue.
            npcDialogue = this->name + ": \n" + dialogue;
            npcDialogue = "Your task:\n" + npcDialogue;
        }
        else {
            //����������������ȡgeneral/relationship�еĶԻ�
            dialogue = getRandomDialogue();
            //��ʾxxx: dialogue.
            npcDialogue = this->name + ": \n" + dialogue;
            npcDialogue = "You have already finished my task!\n" + dialogue;
        }

        //�����npc�ѷ������Ի�������δ��ɣ���ѯ������������
        if (haveChatBefore && !haveFinishTask) {
            npcDialogue = npcDialogue + "\nHave you finished your task?";
        }
    }
    else {
        //index = 1��ʾ����������
        npcDialogue = this->name + ": \n" + getDialogue("task", index);
    }

    // �����Ի��򱳾�
    Size screenSize = Director::getInstance()->getWinSize();

    auto panel = Sprite::create("an_menu.png"); // �Ի��򱳾�ͼƬ·��
    panel->setPosition(screenSize.width / 2, screenSize.height / 2 ); // ��ʾ����Ļ�·�����
    panel->setScale(2.5f,1.5f);
    this->getParent()->addChild(panel);
    panel->setName("dialogueBox");

    // ���������ʾ�Ի�����
    auto label = Label::createWithTTF(npcDialogue, "fonts/arial.ttf", 12);
    label->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 2);
    label->setColor(Color3B::BLACK);
    //MapManager::getInstance()->addChild(label, 15);
    //Vec2 mapSize = MapManager::getInstance()->getCurrentMapSize(1);
    //label->setPosition(mapSize.x / 2, mapSize.y / 2);
    panel->addChild(label);

    // �رհ�ť
    auto closeButton = MenuItemImage::create("../Resources/close.png", "../Resources/close.png",
        [=](Ref* sender) {
            closeDialogueBox();
        });
    closeButton->setScale(5.0f);

    // ���رհ�ť�����ڶԻ�������Ͻ�
    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition( panel->getContentSize().width, panel->getContentSize().height );

    closeButton->setScale(2.0f);
    panel->addChild(menu);

    if (!haveFinishTask && haveChatBefore) {
        // ���ȷ�ϰ�ť
        auto yesButton = MenuItemImage::create("../Resources/Yes.jpg", "../Resources/Yes.png",
            [=](Ref* sender) {
                taskFinish();
            });
        yesButton->setScale(0.02f);

        // ����ɰ�ť�����ڶԻ�������½�
        auto menu2 = Menu::create(yesButton, nullptr);
        menu2->setPosition(panel->getContentSize().width , 0);
        panel->addChild(menu2);
    }    

    if (!haveChatBefore) {
        haveChat();
    }


    /*
        // ����3����Զ��Ƴ��Ի���
        auto delay = DelayTime::create(3.0f);
        auto remove = CallFunc::create([panel]() { panel->removeFromParent(); });
        auto sequence = Sequence::create(delay, remove, nullptr);
        panel->runAction(sequence);    
    */

}

void NpcTemplate::closeDialogueBox() {
    auto dialogueBox = this->getParent()->getChildByName("dialogueBox");
    if (dialogueBox) {
        this->getParent()->removeChild(dialogueBox);
    }
    isOpenDialogueBox = false;
}

void NpcTemplate::taskFinish() {
    //�ȹرյ�ǰ�Ի���
    closeDialogueBox();
    //�ٿ����µĶԻ��򣬱�ʾף���������
    haveFinishTask = true;
    displayDialogueBox(1);  //task������1���������
    //****************�������Ӿ��顢��Ǯ�ȵ�*********************
    taskFinishReward();
}

void NpcTemplate::taskFinishReward(){
    //******************************************************************
    int x;
    UIManager::getInstance(x, "")->setMoney(1000);
    this->affection += 40;
}


void NpcTemplate::removeDialogues(const std::string& category) {
    // ���ҷ����Ƿ����
    auto itCategory = categorizedDialogues.find(category);

    // ����������
    if (itCategory != categorizedDialogues.end()) {
        // ��ո÷����µ����жԻ�
        itCategory->second.clear();

        // ɾ���÷���
        categorizedDialogues.erase(itCategory);
    }
}


std::string NpcTemplate::getDialogue(const std::string& category, int index) {
    // ��������Ӧ�ĶԻ��б�
    auto it = categorizedDialogues.find(category);
    // ������û���ҵ����׳��쳣�򷵻ؿ�ֵ
    if (it == categorizedDialogues.end()) {
        throw std::out_of_range("Category not found: " + category);
    }
    // ��ȡ��Ӧ�ĶԻ��б�
    std::vector<std::string>& dialogues = it->second;

    // ��������Ƿ���Ч
    if (index < 0 || index >= dialogues.size()) {
        throw std::out_of_range("Index out of range for category: " + category);
    }
    // ����ָ�������ĶԻ�
    return dialogues[index];
}