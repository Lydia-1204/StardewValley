/********************************************************************************************************
 * Project Name:  StardewValley
 * File Name:     NPC.cpp
 * File Function: 实现NPC类，实现NPC的管理 对话存储、任务存储
 * Author:        邓语乐  2351273
 * Update Date:   2024/12/22
 *********************************************************************************************************/
#include "Npc.h"
#include <ctime>
USING_NS_CC;

NPC* NPC::instance = nullptr;


// 析构函数
NPC::~NPC() {

}

Vec2 NPC::getNpcsPosition() const {
    return this->getPosition();  // 返回 NPC 的位置
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
        // 假设有4列方向，13行帧
        widthSegment = size.width / 4;
        heightSegment = size.height / 13;
        this->setTextureRect(Rect(0, 0, widthSegment, heightSegment));
    }

    this->setPosition(Vec2(0, 0));
    this->scheduleUpdate();

    //// 创建显示好感度的框
    //showAffectionBox();

    return true;
}

bool NPC::init() {
    if (!Node::init()) {
        return false;
    }
    // 你的初始化逻辑
    createAndPositionNPCs(); // 调用此函数以创建并定位NPC们
    return true;
}

NPC::NPCType NPC::getType() const {
    return type;
}

NPC* NPC::getInstance() {
    if (instance == nullptr) { // 如果实例不存在，则创建
        instance = NPC::create();
    }
    return instance; // 返回唯一实例
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

    // 获取屏幕尺寸
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // 定义四个位置：左上、左下、右上、右下
    std::vector<Vec2> positions = {
        Vec2(origin.x + visibleSize.width * 0.2f, origin.y + visibleSize.height * 0.8f), // 左上
        Vec2(origin.x + visibleSize.width * 0.2f, origin.y + visibleSize.height * 0.1f), // 左下
        Vec2(origin.x + visibleSize.width * 0.8f, origin.y + visibleSize.height * 0.8f), // 右上
        Vec2(origin.x + visibleSize.width * 0.8f, origin.y + visibleSize.height * 0.2f)  // 右下
    };

    CCASSERT(npcTypes.size() == 4, "NPC类型数量不正确");
    CCASSERT(positions.size() == 4, "NPC位置数量不正确");

    for (int i = 0; i < 4; ++i) {
        NPC* npc = NPC::create(npcTypes[i]);
        if (npc) {
            npc->setPosition(positions[i]);
            npc->setScale(1.4f);
            this->addChild(npc);
            npcs.push_back(npc);
            CCLOG("Added NPC [%d] at position: (%f, %f)", i, positions[i].x, positions[i].y);

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
            npc->addDialogue("task", "Could you send a gift to [NPC]? \nHe or she will be happy!\n$450");
            npc->addDialogue("task", "Your tools are looking a bit worn. \nMaybe it's time to buy an upgrade?\n$100");
            npc->addDialogue("task", "There’s an event happening in town. \nCan you attend and represent us?\n$500");

            npc->addDialogue("relationship_friendship", "I always enjoy spending time with you. Thanks for being here.");
            npc->addDialogue("relationship_friendship", "You’re such a reliable person! The town is lucky to have you.");
            npc->addDialogue("relationship_friendship", "I feel like I can always count on you. It means a lot.");
            npc->addDialogue("relationship_friendship", "Your kindness never ceases to amaze me.");
            npc->addDialogue("relationship_friendship", "I hope you know how much your support means to me.");
            npc->addDialogue("relationship_friendship", "Every time I see you, my day feels brighter.");

            npc->addDialogue("relationship_romantic", "I feel so at ease when I’m with you, like everything is just right.");
            npc->addDialogue("relationship_romantic", "You’re always on my mind. It’s a little embarrassing, but I’m glad.");
            npc->addDialogue("relationship_romantic", "I think we share something really special. It’s hard to put into words.");
            npc->addDialogue("relationship_romantic", "Whenever we’re apart, I find myself looking forward to the next time I see you.");
            npc->addDialogue("relationship_romantic", "Being with you makes me feel like I’m the luckiest person in the world.");
            npc->addDialogue("relationship_romantic", "You’ve stolen my heart completely, and I wouldn’t want it any other way.");
            npc->addDialogue("relationship_romantic", "Thank you for always believing in me. You’re my everything.");

            srand(static_cast<unsigned>(time(0)));

            // 调用initialmove让NPC开始移动
            npc->initialmove();

            // 如果需要在这里设置NPC子图Rect，也可以直接调用setNPCDisplayRect
            // 例如：
            // npc->setNPCDisplayRect(Rect(0, 0, 32, 48)); // 截取32x48区域
        }
        else {
            CCLOG("Failed to create NPC of type %d", static_cast<int>(npcTypes[i]));
        }
    }
}
// NPC的update函数，每帧调用，用于更新方向和其他状态
void NPC::update(float dt) {
    // 更新方向，这里可以加逻辑决定贴图帧变化
    updateDirection(dt);
}

// 计算方向和更改贴图帧的函数(简化版)
void NPC::updateDirection(float dt) {
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
void NPC::initialmove() {
    Vec2 startPosition = this->getPosition();
    lastPosition = startPosition;

    // 定义一条循环移动路径
    std::vector<Vec2> path = {
        Vec2(startPosition.x, startPosition.y),
        Vec2(startPosition.x + 100, startPosition.y),
        Vec2(startPosition.x+50, startPosition.y ),
        Vec2(startPosition.x, startPosition.y)
    };

    moveAlongPath(path);
}

// NPC沿路径移动的函数
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
    // 获取屏幕尺寸
    Size screenSize = Director::getInstance()->getWinSize();

    // 创建一个固定位置的面板，例如可以用一个透明背景的矩形
    auto panel = Sprite::create("an_menu.png");  // 假设这是一个面板的图片
    panel->setPosition(screenSize.width / 2, screenSize.height / 2);  // 面板显示在屏幕中央
    panel->setScale(3.0f);
    this->getParent()->addChild(panel);

    // 获取所有 NPC
    auto npcManager = NPC::getInstance();
    const auto& npcs = npcManager->getAllNPCs();

    // 创建文本标签并显示每个 NPC 的好感度和关系
    float yOffset = 50.0f;  // 每个 NPC 信息的垂直间隔
    int count = 1;

    for (auto npc : npcs) {
        // 创建一个显示 NPC 名字、好感度和关系的文本
        std::string text = "NPC " + std::to_string(count) + ":\n" +
            "Affection: " + std::to_string(npc->_affection) + "\n" +
            "Relationship: " + npc->_relationship;


        auto label = Label::createWithTTF(text, "../Resources/fonts/Marker Felt.ttf", 18);
        if (label) {

            label->setPosition(Vec2(screenSize.width / 2, screenSize.height / 2 + 100 + yOffset));
            label->setColor(Color3B::BLACK);  // 设置文本颜色
            this->getParent()->addChild(label);  // 添加到父节点
        }

        // 更新偏移量，确保每个 NPC 的信息不会重叠
        yOffset -= 100.0f;  // 每个 NPC 的信息下移
        count++;
    }
}
void NPC::addDialogue(const std::string& category, const std::string& dialogue) {
    categorizedDialogues[category].push_back(dialogue);
}


std::string NPC::getRandomDialogue(const std::string& category) const {
    // 检查是否存在该分类
    auto it = categorizedDialogues.find(category);
    if (it == categorizedDialogues.end() || it->second.empty()) {
        CCLOG("No dialogues found for category: %s", category.c_str());
        return ""; // 返回空字符串表示没有可用对话
    }

    // 从对应分类中随机选择一条对话
    const auto& dialogues = it->second;
    int randomIndex = rand() % dialogues.size(); // 使用随机索引
    return dialogues[randomIndex];
}

 
void NPC::displayDialogueBox(const std::string& category) {
    // 获取随机对话
    std::string dialogue = getRandomDialogue(category);

    // 创建对话框背景
    Size screenSize = Director::getInstance()->getWinSize();
    auto panel = Sprite::create("dialogue_panel.png"); // 对话框背景图片路径
    panel->setPosition(Vec2(screenSize.width / 2, 0)); // 显示在屏幕下方中央
    panel->setScale(2.0f);
    this->getParent()->addChild(panel);

    // 在面板上显示对话内容
    auto label = Label::createWithTTF(dialogue, "fonts/Marker Felt.ttf", 20);
    label->setPosition(panel->getContentSize().width / 2, panel->getContentSize().height / 3);
    label->setColor(Color3B::BLACK);
    panel->addChild(label);

    // 设置3秒后自动移除对话框
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
    // 查找类别对应的对话列表
    auto it = categorizedDialogues.find(category);
    if (it != categorizedDialogues.end()) {
        return it->second; // 返回对应类别的对话
    }
    // 如果类别不存在，返回一个空列表
    static const std::vector<std::string> emptyList;
    return emptyList;
}
