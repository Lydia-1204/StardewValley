#include "NpcTemplate.h"
#include <ctime>
USING_NS_CC;


// 析构函数
NpcTemplate::~NpcTemplate() {

}

Vec2 NpcTemplate::getNpcsPosition() const {
    return this->getPosition();  // 返回 NPC 的位置
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
        // 假设有4列方向，13行帧
        widthSegment = size.width / 4;
        heightSegment = size.height / 13;
        this->setTextureRect(Rect(0, 0, widthSegment, heightSegment));
    }

    this->setPosition(position);
    this->scheduleUpdate();

    //// 创建显示好感度的框
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
        npc->autorelease();  // 确保对象在不再使用时能够自动释放
        return npc;
    }
    CC_SAFE_DELETE(npc);  // 如果初始化失败，则删除对象
    return nullptr;
}
*/


/***********************************************
NpcTemplate* NpcTemplate::getNpcInstance(NPCType npcType, Vec2 startPoint = cocos2d::Vec2(0, 0)) {
    // 检查是否已有该类型的 NPC 实例
    for (NpcTemplate* npc : npcInstances) {
        if (npc->getType() == npcType) {
            return npc;
        }
    }

    // 如果没有找到该类型的实例，创建新的 NPC 实例
    NpcTemplate* newNpc = NpcTemplate::create(npcType, startPoint);
    npcInstances.push_back(newNpc);  // 将新创建的 NPC 加入 npcInstances 向量中
    return newNpc;
    /*
    if (npcInstances.find(npcType) == npcInstances.end()) {
        // 如果该类型的实例还没有创建
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
    if (instance == nullptr) { // 如果实例不存在，则创建
        instance = NpcTemplate::create();
    }
    return instance; // 返回唯一实例
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

    // 初始化对话
    npc->addDialogue("general", "The sun feels nice today.");
    npc->addDialogue("general", "Did you know the market is open?");
    npc->addDialogue("general", "What a beautiful day to be outside, don't you think?");
    npc->addDialogue("general", "The flowers are blooming wonderfully this season.");
    npc->addDialogue("general", "Farming must keep you busy. Do you ever get a day off?");
    npc->addDialogue("general", "The town feels livelier with you around!");
    npc->addDialogue("general", "Sometimes I wonder what lies beyond the mountains.");
    npc->addDialogue("general", "Every day is a new adventure in this little town.");
    
    npc->addDialogue("general", "The weather looks perfect for fishing today!");
    npc->addDialogue("general", "It’s amazing how quickly the seasons change, isn’t it?");
    npc->addDialogue("general", "I heard someone found an old treasure map in the attic!");
    npc->addDialogue("general", "Nothing beats the smell of freshly baked bread in the morning.");
    npc->addDialogue("general", "I wonder how deep the mines go. Have you explored them yet?");
    


    
    npc->addDialogue("task", "Could you plant a crop for me?\n I need it for a recipe.\n$350");
    npc->addDialogue("task", "The mines hold many secrets. \nCan you explore them and bring back something interesting?\n$1000");
    npc->addDialogue("task", "Could you send a gift to [NPC]? she is around your home, \nHe or she will be happy!\n$450");
    npc->addDialogue("task", "Your tools are looking a bit worn. \nMaybe it's time to buy an upgrade?\n$100");
    npc->addDialogue("task", "There’s an event happening in town. \nCan you attend and represent us?\n$500");            
    


    srand(static_cast<unsigned>(time(0)));

    // 调用initialmove让NPC开始移动
    npc->initialmove();

    // 如果需要在这里设置NPC子图Rect，也可以直接调用setNPCDisplayRect
    // 例如：
    // npc->setNPCDisplayRect(Rect(0, 0, 32, 48)); // 截取32x48区域
}
*/


// NPC的update函数，每帧调用，用于更新方向和其他状态
void NpcTemplate::update(float dt) {
    // 更新好感度及对应对话
    if (affection >= 30 && affection < 70) {
        //30~69——friendship
        addDialogue("relationship_friendship", "I always enjoy spending time with you. Thanks for being here.");
        addDialogue("relationship_friendship", "You’re such a reliable person! The town is lucky to have you.");
        addDialogue("relationship_friendship", "I feel like I can always count on you. It means a lot.");
        addDialogue("relationship_friendship", "Your kindness never ceases to amaze me.");
        addDialogue("relationship_friendship", "I hope you know how much your support means to me.");
        addDialogue("relationship_friendship", "Every time I see you, my day feels brighter.");
        relationship = "friendship";
    }
    else if (affection >= 70 ) {
        //30~69——friendship
        addDialogue("relationship_romantic", "I feel so at ease when I’m with you, like everything is just right.");
        addDialogue("relationship_romantic", "You’re always on my mind. It’s a little embarrassing, but I’m glad.");
        addDialogue("relationship_romantic", "I think we share something really special. It’s hard to put into words.");
        addDialogue("relationship_romantic", "Whenever we’re apart, I find myself looking forward to the next time I see you.");
        addDialogue("relationship_romantic", "Being with you makes me feel like I’m the luckiest person in the world.");
        addDialogue("relationship_romantic", "You’ve stolen my heart completely, and I wouldn’t want it any other way.");
        addDialogue("relationship_romantic", "Thank you for always believing in me. You’re my everything.");
        //并删除friendship的对话
        removeDialogues("relationship_friendship");

        relationship = "romantic";
        //setRelationship("romantic");
    }


    // 更新方向，这里可以加逻辑决定贴图帧变化
    updateDirection(dt);
}

// 计算方向和更改贴图帧的函数(简化版)
void NpcTemplate::updateDirection(float dt) {
    Vec2 currentPosition = this->getPosition();
    Vec2 direction = currentPosition - lastPosition;
    lastPosition = currentPosition;

    // 根据direction确定方向帧
    // 下(down): col=0
    // 左(left): col=1
    // 右(right):col=2
    // 上(up):   col=3

    Rect rectForDirection;

    int col = 0; // 默认向下
    // 若NPC静止，不修改贴图
    if (direction.x > 0) {
        // 向右移动
        rectForDirection = Rect(0, heightSegment, widthSegment, heightSegment); // 假设右移动方向在第2列
    }
    else if (direction.x < 0) {
        // 向左移动
        rectForDirection = Rect(0, heightSegment * 3, widthSegment, heightSegment); // 假设左移动方向在第3列
    }
    else if (direction.y > 0) {
        // 向上移动
        rectForDirection = Rect(0, heightSegment * 2, widthSegment, heightSegment); // 假设上移动方向在第4列
    }
    else if (direction.y < 0) {
        // 向下移动
        rectForDirection = Rect(0, 0, widthSegment, heightSegment); // 假设下移动方向在第1列
    }

    // 我们用第一行(row=0)的帧作为静态方向显示
    int row = 0;
    float x = col * widthSegment;
    float y = row * heightSegment;
    // 设置对应方向的Frame区域
// 更新NPC的纹理区域
    this->setTextureRect(rectForDirection);
}

// 初始化NPC自动移动。定义一条循环路径并沿其移动
void NpcTemplate::initialmove() {
    Vec2 startPosition = this->getPosition();
    lastPosition = startPosition;

    // 定义一条循环移动路径
    std::vector<Vec2> path = {
        Vec2(startPosition.x, startPosition.y),
        Vec2(startPosition.x + 100, startPosition.y),
        Vec2(startPosition.x + 50, startPosition.y),
        Vec2(startPosition.x, startPosition.y)
    };

    moveAlongPath(path);
}

// NPC沿路径移动的函数
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
    // 获取屏幕尺寸
    Size screenSize = Director::getInstance()->getWinSize();

    // 创建一个固定位置的面板，例如可以用一个透明背景的矩形
    auto panel = Sprite::create("an_menu.png");  // 假设这是一个面板的图片
    panel->setPosition(screenSize.width / 2, screenSize.height / 2);  // 面板显示在屏幕中央
    panel->setScale(3.0f);
    this->getParent()->addChild(panel);


    // 获取所有 NPC
    std::vector<NpcTemplate*> npcs;
    npcs.push_back(Elliott::getInstance());
    npcs.push_back(Sam::getInstance());
    npcs.push_back(Shane::getInstance());
    npcs.push_back(Abigail::getInstance());

    // 创建文本标签并显示每个 NPC 的好感度和关系
    float yOffset = 50.0f;  // 每个 NPC 信息的垂直间隔
    int count = 1;

    for (auto npc : npcs) {
        // 创建一个显示 NPC 名字、好感度和关系的文本
        std::string text = "NPC " + npc->name + ":\n" +
            "Affection: " + std::to_string(npc->affection) + "\n" +
            "Relationship: " + npc->relationship;


        npc->affectionLabel = Label::createWithTTF(text, "../Resources/fonts/Marker Felt.ttf", 18);
        if (npc->affectionLabel) {
            npc->affectionLabel->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 100 + yOffset));
            //label->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2));

            npc->affectionLabel->setColor(Color3B::BLACK);  // 设置文本颜色
            this->getParent()->addChild(npc->affectionLabel, 10);  // 添加到父节点
            npc->affectionLabel->setName(npc->name);
        }
        
        // 更新偏移量，确保每个 NPC 的信息不会重叠
        yOffset -= 100.0f;  // 每个 NPC 的信息下移
        count++;

    }        
    // 关闭按钮
        auto closeButton = MenuItemImage::create("../Resources/close.png", "../Resources/close.png",
            [=](Ref* sender) {
                this->getParent()->removeChild(panel);  // 从父节点删除
                this->getParent()->removeChildByName("Elliott");
                this->getParent()->removeChildByName("Shane");
                this->getParent()->removeChildByName("Sam");
                this->getParent()->removeChildByName("Abigail");

            });
        closeButton->setScale(5.0f);

        // 将关闭按钮放置在对话框的右上角
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

    // 使用随机数生成器
    std::srand(std::time(0));  // 设置随机种子

    // 1. 随机选择一个分类（键）
    std::vector<std::string> categories;
    for (const auto& pair : categorizedDialogues) {
        categories.push_back(pair.first);  // 将所有分类名称（键）添加到一个 vector 中
    }

    // 随机选择一个分类的索引
    int categoryIndex = std::rand() % categories.size();
    std::string randomCategory = categories[categoryIndex];
    while (randomCategory == "task") {
        //如果随机到了task则重新来
        categoryIndex = std::rand() % categories.size();
        randomCategory = categories[categoryIndex];
    }

    // 2. 从该分类对应的 vector 中随机选择一个对话
    const std::vector<std::string>& dialogues = categorizedDialogues[randomCategory];

    // 随机选择一个对话的索引
    int dialogueIndex = std::rand() % dialogues.size();
    std::string randomDialogue = dialogues[dialogueIndex];
    
    //返回随机对话
    return randomDialogue;
}


//******************************************************************88888
void NpcTemplate::displayDialogueBox(int index) {
    isOpenDialogueBox = true;
    std::string dialogue, npcDialogue;

    if (index == 0) {
        if (!haveFinishTask) {
            //若该npc的任务还未完成，优先展示task的dialogue
            dialogue = getDialogue("task", index);  
            //显示xxx: dialogue.
            npcDialogue = this->name + ": \n" + dialogue;
            npcDialogue = "Your task:\n" + npcDialogue;
        }
        else {
            //已完成任务则随机获取general/relationship中的对话
            dialogue = getRandomDialogue();
            //显示xxx: dialogue.
            npcDialogue = this->name + ": \n" + dialogue;
            npcDialogue = "You have already finished my task!\n" + dialogue;
        }

        //如果和npc已发生过对话且任务还未完成，则询问任务完成情况
        if (haveChatBefore && !haveFinishTask) {
            npcDialogue = npcDialogue + "\nHave you finished your task?";
        }
    }
    else {
        //index = 1表示任务完成语句
        npcDialogue = this->name + ": \n" + getDialogue("task", index);
    }

    // 创建对话框背景
    Size screenSize = Director::getInstance()->getWinSize();

    auto panel = Sprite::create("an_menu.png"); // 对话框背景图片路径
    panel->setPosition(screenSize.width / 2, screenSize.height / 2 ); // 显示在屏幕下方中央
    panel->setScale(2.5f,1.5f);
    this->getParent()->addChild(panel);
    panel->setName("dialogueBox");

    // 在面板上显示对话内容
    auto label = Label::createWithTTF(npcDialogue, "fonts/arial.ttf", 12);
    label->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 2);
    label->setColor(Color3B::BLACK);
    //MapManager::getInstance()->addChild(label, 15);
    //Vec2 mapSize = MapManager::getInstance()->getCurrentMapSize(1);
    //label->setPosition(mapSize.x / 2, mapSize.y / 2);
    panel->addChild(label);

    // 关闭按钮
    auto closeButton = MenuItemImage::create("../Resources/close.png", "../Resources/close.png",
        [=](Ref* sender) {
            closeDialogueBox();
        });
    closeButton->setScale(5.0f);

    // 将关闭按钮放置在对话框的右上角
    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition( panel->getContentSize().width, panel->getContentSize().height );

    closeButton->setScale(2.0f);
    panel->addChild(menu);

    if (!haveFinishTask && haveChatBefore) {
        // 完成确认按钮
        auto yesButton = MenuItemImage::create("../Resources/Yes.jpg", "../Resources/Yes.png",
            [=](Ref* sender) {
                taskFinish();
            });
        yesButton->setScale(0.02f);

        // 将完成按钮放置在对话框的右下角
        auto menu2 = Menu::create(yesButton, nullptr);
        menu2->setPosition(panel->getContentSize().width , 0);
        panel->addChild(menu2);
    }    

    if (!haveChatBefore) {
        haveChat();
    }


    /*
        // 设置3秒后自动移除对话框
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
    //先关闭当前对话框
    closeDialogueBox();
    //再开启新的对话框，表示祝贺任务完成
    haveFinishTask = true;
    displayDialogueBox(1);  //task的索引1存放完成语句
    //****************可以增加经验、金钱等等*********************
    taskFinishReward();
}

void NpcTemplate::taskFinishReward(){
    //******************************************************************
    int x;
    UIManager::getInstance(x, "")->setMoney(1000);
    this->affection += 40;
}


void NpcTemplate::removeDialogues(const std::string& category) {
    // 查找分类是否存在
    auto itCategory = categorizedDialogues.find(category);

    // 如果分类存在
    if (itCategory != categorizedDialogues.end()) {
        // 清空该分类下的所有对话
        itCategory->second.clear();

        // 删除该分类
        categorizedDialogues.erase(itCategory);
    }
}


std::string NpcTemplate::getDialogue(const std::string& category, int index) {
    // 查找类别对应的对话列表
    auto it = categorizedDialogues.find(category);
    // 如果类别没有找到，抛出异常或返回空值
    if (it == categorizedDialogues.end()) {
        throw std::out_of_range("Category not found: " + category);
    }
    // 获取对应的对话列表
    std::vector<std::string>& dialogues = it->second;

    // 检查索引是否有效
    if (index < 0 || index >= dialogues.size()) {
        throw std::out_of_range("Index out of range for category: " + category);
    }
    // 返回指定索引的对话
    return dialogues[index];
}
