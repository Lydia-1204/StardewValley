/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: 声明UIManager类，用于实现游戏主界面 包括地图加载、移动 角色移动 系统更新 固定UI
 * Author:        王小萌 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#include "Systems/UIManager.h"
#include "cocos2d.h"
#include "Scenes/MenuScene.h"
#include "App/Constant.h"
#include "Inventory/Chest.h"
#include "Characters/Player.h"
#include "Inventory/ItemManager.h"
#include "Characters/NpcTemplate.h"
#include "Characters/Elliott.h"
#include "Characters/Sam.h"
#include "Characters/Shane.h"
#include "Characters/Abigail.h"
#include "World/Map.h"
#include "Inventory/ToolManager.h"

USING_NS_CC;

UIManager* UIManager::instance = nullptr;

UIManager::UIManager()
    : dateLabel(nullptr), timeLabel(nullptr), moneyLabel(nullptr),
    energyBar(nullptr), iron(nullptr),
    shortcutKeysLabel(nullptr), money(0), selectedCharacter(0),
    currentMonth(3), currentDay(1), currentWeekday(2), // 3月1日，周二
    currentEnergy(100), timeElapsed(0.0f), isPriceBoardOpen(0) {}

UIManager* UIManager::getInstance(int& selectedCharacter, const std::string& nickname) {
    if (!instance) {
        // 如果实例不存在，创建并初始化它
        instance = new (std::nothrow) UIManager();
        if (instance && !instance->init(selectedCharacter, nickname)) {
            // 如果初始化失败，释放实例并返回 nullptr
            delete instance;
            instance = nullptr;
        }
    }
    return instance;
}

bool UIManager::init(int selectedCharacter, const std::string& nickname) {
    if (!Node::init()) {
        return false;
    }

    currentMonth = 12;                 // 当前月份
    currentDay = 8;                    // 当前日期
    currentWeekday = 6;                // 当前周几（0=周日，1=周一，...，6=周六）
    currentHour = 8;                   // 当前小时
    currentMinute = 0;                 // 当前分钟
    currentEnergy = 100;                 // 当前精力值（百分比）
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->nickname = nickname;
    this->selectedCharacter = selectedCharacter;
    mapManager = MapManager::getInstance();
    // 日期和时间显示
    dateLabel = Label::createWithTTF("December 8st, Tuesday", "../Resources/fonts/Marker Felt.ttf", 24);
    dateLabel->setPosition(visibleSize.width - 200, visibleSize.height - 50);
    this->addChild(dateLabel);

    timeLabel = Label::createWithTTF("08:00", "../Resources/fonts/Marker Felt.ttf", 24);
    timeLabel->setPosition(visibleSize.width - 200, visibleSize.height - 150);
    this->addChild(timeLabel, 20);


    // 初始化NPC*******************
    Elliott* elliott = Elliott::getInstance();
    Sam* sam = Sam::getInstance();
    Shane* shane = Shane::getInstance();
    Abigail* abigail = Abigail::getInstance();

    this->addChild(elliott);
    elliott->setName("elliott");
    this->addChild(sam);
    sam->setName("sam");
    this->addChild(abigail);
    abigail->setName("abigail");
    this->addChild(shane);
    shane->setName("shane");
    elliott->usePatrolStrategy();
    sam->useStandStillStrategy();
    abigail->usePatrolStrategy();
    shane->useStandStillStrategy();
    elliott->initialmove();
    sam->initialmove();
    abigail->initialmove();
    shane->initialmove();

    elliott->addGeneralDialogue();
    sam->addGeneralDialogue();
    abigail->addGeneralDialogue();
    shane->addGeneralDialogue();

    elliott->addTaskDialogues();
    sam->addTaskDialogues();
    abigail->addTaskDialogues();
    shane->addTaskDialogues();

    //初始只让Elliott显示
    sam->setVisible(false);
    abigail->setVisible(false);
    shane->setVisible(false);


    // 美观图标
    iron = Sprite::create("../Resources/LooseSprites-73/iron123.png");
    if (!iron)
        throw("iron created failed!!");
    iron->setPosition(visibleSize.width - 200, visibleSize.height - 100);
    this->addChild(iron, 20);


    // 资金显示
    moneyLabel = Label::createWithTTF("Money: $0", "../Resources/fonts/Marker Felt.ttf", 24);
    moneyLabel->setPosition(visibleSize.width - 200, visibleSize.height - 200);
    this->addChild(moneyLabel, 20);

    //天气显示
    weatherLabel = Label::createWithTTF("Sunny", "../Resources/fonts/Marker Felt.ttf", 24);
    weatherLabel->setPosition(visibleSize.width - 200, visibleSize.height - 250);
    this->addChild(weatherLabel, 20);


    // 精力条
    energyBar = ProgressTimer::create(Sprite::create("../Resources/energyBar.png"));
    if (!energyBar)
        throw("energyBar created failed!!");
    energyBar->setScale(0.4f);

    energyBar->setType(ProgressTimer::Type::BAR);

    energyBar->setMidpoint(Vec2(0, 0));
    energyBar->setBarChangeRate(Vec2(1, 0));
    energyBar->setPercentage(100);
    energyBar->setPosition(visibleSize.width - 30, visibleSize.height * 0.25);
    this->addChild(energyBar, 20);
    //精力条label
    energyLabel = Label::createWithTTF("energy:100", "../Resources/fonts/Marker Felt.ttf", 18);
    energyLabel->setPosition(visibleSize.width - 50, visibleSize.height * 0.12);
    this->addChild(energyLabel, 20);
    // 快捷键提示框
    shortcutKeysLabel = Label::createWithTTF("E: Pause\nC: Use Tool\nV: Discard Tool\nM: Mini Map\nF: Task List\nP:sjill tree", "../Resources/fonts/Marker Felt.ttf", 18);
    shortcutKeysLabel->setPosition(100, visibleSize.height - 100);
    this->addChild(shortcutKeysLabel, 20);
    /*
    // 工具栏
    auto tool1 = Sprite::create("../Resources/LooseSprites-73/textBox..png");
    setToolBar({ tool1 }); // 初始化工具栏
    */


    // 初始化暂停面板
    pausePanel = LayerColor::create(Color4B(20, 20, 20, 220)); // 深色且几乎不透明
    pausePanel->setVisible(false);
    this->addChild(pausePanel);

    // 添加按钮 "Exit to Menu"
    auto exitButton = MenuItemLabel::create(
        Label::createWithTTF("Exit Game", "../Resources/fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            CCLOG("Exit  clicked");
            static bool isSwitchingScene = false;
            if (isSwitchingScene) return;
            isSwitchingScene = true;
            Director::getInstance()->end();
            /*
            Director::getInstance()->resume(); // 恢复游戏状态
            //this->clearResources();
            // 执行返回主菜单的逻辑
            auto menuScene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene, Color3B::BLACK));
            */
        }
    );
    exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50));

    // 添加按钮 "Save Progress"
    auto saveButton = MenuItemLabel::create(
        Label::createWithTTF("Save Progress", "../Resources/fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            CCLOG("Save Progress clicked");
            // 执行保存游戏进度的逻辑
        }
    );
    saveButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));

    auto menu = Menu::create(exitButton, saveButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    pausePanel->addChild(menu);

    // 小地图
    miniMap = Sprite::create("../Resources/UI/miniMap.jpg");
    if (!miniMap) throw std::runtime_error("Failed to load minimap texture!");
    miniMap->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    miniMap->setVisible(false);
    this->addChild(miniMap);

    // 添加红点标注角色位置
    playerMarker = Sprite::create("../Resources/UI/playerStar.png");
    playerMarker->setPosition(Vec2(miniMap->getContentSize().width / 2, miniMap->getContentSize().height / 2)); // 默认居中
    miniMap->addChild(playerMarker);

    // 添加 "Your Position" 文本
    auto positionLabel = Label::createWithTTF("Your Position", "../Resources/fonts/Marker Felt.ttf", 10);
    positionLabel->setColor(Color3B::RED);
    positionLabel->setPosition(Vec2(playerMarker->getContentSize().width / 2, -10)); // 放在星星下方
    playerMarker->addChild(positionLabel, 10);

    // 任务面板
    taskListPanel = LayerColor::create(Color4B(40, 40, 40, 200)); // 浅灰色背景
    taskListPanel->setContentSize(Size(400, 300)); // 设置任务面板大小
    taskListPanel->setPosition((visibleSize.width - taskListPanel->getContentSize().width) / 2,
        (visibleSize.height - taskListPanel->getContentSize().height) / 2);
    taskListPanel->setVisible(false);
    this->addChild(taskListPanel);

    // 初始化任务列表
    for (int i = 0; i < 4; i++) {
        Label* taskLabel;
        switch (i) {
        case 0:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i + 1) + " : plant a crop", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        case 1:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i + 1) + " : go mining ", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        case 2:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i + 1) + " : send a gift to npc", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        case 3:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i + 1) + " : buy a upgrade tool", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        }
        taskLabel->setAnchorPoint(Vec2(0, 0.5f));
        taskLabel->setPosition(Vec2(20, taskListPanel->getContentSize().height - 30 * (i + 1)));
        taskListPanel->addChild(taskLabel);
        taskLabels.push_back(taskLabel);
    }

    // 创建显示对话精灵(从 emotes.png 中截取)
    std::string dialogTexture = "emotes.png";
    dialogSprite = Sprite::create(dialogTexture);
    // 获取图片的原始尺寸
    Size originalSize = dialogSprite->getContentSize();
    // 计算四分之一的宽和1/16高
    float quarterWidth = originalSize.width / 4.0f;
    float quarterHeight = originalSize.height / 16.0f;
    // 设置纹理裁剪区域
    Rect cutRect(3 * quarterWidth, 10 * quarterHeight, quarterWidth, quarterHeight);
    dialogSprite->setTextureRect(cutRect);
    dialogSprite->setScale(1.4f);
    dialogSprite->setOpacity(0); // 初始隐藏
    this->addChild(dialogSprite, 5);

    // 创建对话框精灵(从textBox.png 中截取)
    std::string chatTexture = "textBox.png";
    chatSprite = Sprite::create(chatTexture);
    chatSprite->setPosition(600, 650);
    chatSprite->setScale(3);
    chatSprite->setVisible(0); // 初始隐藏
    this->addChild(chatSprite, 999);
    /*
     // 创建 dialogTextLabel，用于显示对话内容
    dialogTextLabel = Label::createWithTTF("i am NPC", "../Resources/fonts/Marker Felt.ttf", 18);
    dialogTextLabel->setTextColor(Color4B(0, 0, 0, 255));// black
    dialogTextLabel->setPosition(600, 680);
    dialogTextLabel->setVisible(0); // 初始隐藏
    this->addChild(dialogTextLabel, 999);

    // 将对话框和文字添加到场景
    auto scene = Director::getInstance()->getRunningScene();
    //showPriceBoard();
    */

    return true;
}

void UIManager::update(float delta) {
    timeElapsed += delta;

    if (timeElapsed >= 1.0f) {
        timeElapsed = 0.0f;

        currentMinute += 1;

        if (currentMinute >= 60) {
            currentMinute = 0;
            currentHour += 1;

            if (currentHour >= 24) {
                currentHour = 0;
                currentDay += 1;
                currentWeekday = (currentWeekday + 1) % 7;

                if (currentDay > getDaysInMonth(currentMonth)) {
                    currentDay = 1;
                    currentMonth += 1;
                    if (currentMonth > 12) {
                        currentMonth = 1;
                    }
                }
            }
        }

        if (currentMinute % 10 == 0) {
            currentEnergy = std::max(0, currentEnergy - 1);
            setEnergy(currentEnergy);
        }
        //更新日期时间显示
        updateDateAndTime();
    }
}

void UIManager::setMoney(int dmoney) {
    this->money += dmoney;
    moneyLabel->setString("Money: $" + std::to_string(money));
}

void UIManager::setEnergy(int energy) {
    energyBar->setPercentage(energy);
    energyLabel->setString("energy: " + std::to_string(energy));
}

void UIManager::increaseEnergy(float deltaEnergy) {
    // currentEnergy 表示当前精力值（0-100之间）
    currentEnergy += deltaEnergy;

    // 防止精力值小于0
    if (currentEnergy < 0) {
        currentEnergy = 0;
    }

    // 防止精力值大于100
    else if (currentEnergy > 100) {
        currentEnergy = 100;
    }

    // 更新精力条
    setEnergy(currentEnergy);
}

void UIManager::setDateAndTime(const std::string& date, const std::string& time) {
    if (dateLabel) {
        dateLabel->setString(date); // 更新日期显示
    }
    if (timeLabel) {
        timeLabel->setString(time); // 更新时间显示
    }
}

void UIManager::updateDateAndTime() {
    static const char* months[] = { "January", "February", "March", "April", "May", "June",
                                   "July", "August", "September", "October", "November", "December" };
    static const char* weekdays[] = { "Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday" };

    char dateStr[50];
    snprintf(dateStr, sizeof(dateStr), "%s %d, %s", months[currentMonth - 1], currentDay, weekdays[currentWeekday]);

    char timeStr[10];
    snprintf(timeStr, sizeof(timeStr), "%02d:%02d", currentHour, currentMinute);

    setDateAndTime(dateStr, timeStr);
}

int UIManager::getDaysInMonth(int month) {
    static const int daysInMonth[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
    return daysInMonth[month - 1];
}

UIManager* UIManager::getLayer() {
    return this;
}
/*
void UIManager::setToolBar(const std::vector<Sprite*>& tools) {
    // 清空当前工具栏
    for (auto& child : getChildren()) {
        if (child->getName() == "Tool") {
            this->removeChild(child);
        }
    }

    // 填充新的工具
    const float startX = 600;  // 起始位置
    for (size_t i = 0; i < tools.size(); ++i) {
        tools[i]->setName("Tool");
        tools[i]->setPosition(Vec2(startX + i * 10, 50)); // 每个工具的间距为 50
        this->addChild(tools[i]);
    }
}*/
void UIManager::toggleMiniMap(const Vec2& playerPos, const Size& mapSize) {
    isMiniMapVisible = !isMiniMapVisible;
    miniMap->setVisible(isMiniMapVisible);

    if (isMiniMapVisible) {

        auto screenSize = Director::getInstance()->getVisibleSize();

        float addWidth = 0, addHeight = 0;
        CCLOG("currentMapLabel:%d", mapManager->currentMapLabel);
        if (mapManager->currentMapLabel >= 1 && mapManager->currentMapLabel <= 4) {
            if (mapManager->currentMapLabel == 2 || mapManager->currentMapLabel == 4) {
                addWidth = screenSize.width;
            }
            if (mapManager->currentMapLabel == 1 || mapManager->currentMapLabel == 2) {
                addHeight = screenSize.height;
            }
            Vec2 normalizedPos((playerPos.x + addWidth) / (screenSize.width * 2), (playerPos.y + addHeight) / (screenSize.height * 2));
            playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x, miniMap->getContentSize().height * normalizedPos.y));
        }
        else if (mapManager->currentMapLabel == 5) {
            Vec2 normalizedPos(0.25, 0.8);
            playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x, miniMap->getContentSize().height * normalizedPos.y));
        }
        else if (mapManager->currentMapLabel == 6) {
            Vec2 normalizedPos(4.0 / 10.5f, 1.0 / 6.5f);
            playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x, miniMap->getContentSize().height * normalizedPos.y));
        }
        else if (mapManager->currentMapLabel == 7) {
            Vec2 normalizedPos(7.2 / 10.5f, 5.8 / 6.5f);
            playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x, miniMap->getContentSize().height * normalizedPos.y));
        }

    }
}

void UIManager::toggleTaskList() {
    isTaskListVisible = !isTaskListVisible;
    taskListPanel->setVisible(isTaskListVisible);
}

void UIManager::updateTaskList(const std::vector<std::string>& tasks) {
    for (int i = 0; i < taskLabels.size(); i++) {
        if (i < tasks.size()) {
            taskLabels[i]->setString(tasks[i]);
            taskLabels[i]->setVisible(true);

            auto listener = EventListenerTouchOneByOne::create();
            listener->onTouchBegan = [=](Touch* touch, Event* event) {
                if (taskLabels[i]->getBoundingBox().containsPoint(touch->getLocation())) {
                    auto detailBox = LayerColor::create(Color4B(0, 0, 0, 200), 250, 100);
                    detailBox->setPosition(taskListPanel->getPosition() + Vec2(320, 30 * (4 - i)));
                    auto detailLabel = Label::createWithTTF("Details: " + tasks[i],
                        "../Resources/fonts/Marker Felt.ttf", 16);
                    detailLabel->setPosition(detailBox->getContentSize() / 2);
                    detailBox->addChild(detailLabel);
                    this->addChild(detailBox);

                    detailBox->runAction(Sequence::create(DelayTime::create(2.0f), RemoveSelf::create(), nullptr));
                    return true;
                }
                return false;
                };
            _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, taskLabels[i]);
        }
        else {
            taskLabels[i]->setVisible(false);
        }
    }
}

void UIManager::showPausePanel() {
    pausePanel->setVisible(true);
}

void UIManager::hidePausePanel() {
    pausePanel->setVisible(false);
}

void UIManager::showPriceBoard() {
    auto visibleSize = Director::getInstance()->getVisibleSize();
    itemManager = ItemManager::getInstance(selectedCharacter, nickname);

    // 创建价格表背景图片
    Sprite* priceListBg;
    if (itemManager->getItemQuantity(Item::ItemType::MINERAL) < 30) {
        priceListBg = Sprite::create("../Resources/priceBoard.jpg");
    }
    else {
        priceListBg = Sprite::create("../Resources/priceBoard_lower.jpg");
    }

    priceListBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);

    priceListBg->setName("PriceBoard");
    this->addChild(priceListBg, 10);
    isPriceBoardOpen = 1;
    // 关闭按钮
    auto closeButton = MenuItemImage::create("../Resources/close.png", "../Resources/close.png",
        [=](Ref* sender) {
            this->closePriceBoard();
        });
    closeButton->setScale(4.0f);
    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(priceListBg->getPositionX() + priceListBg->getContentSize().width / 4,
        priceListBg->getPositionY() + priceListBg->getContentSize().height / 4);
    priceListBg->addChild(menu);
}

void UIManager::closePriceBoard() {
    auto priceList = this->getChildByName("PriceBoard");
    if (priceList) {
        this->removeChild(priceList);
    }
    isPriceBoardOpen = 0;
}



void UIManager::setNpcVision() {
    int label = MapManager::getInstance()->getCurrentBlockLabel();
    //  先均不可见
    this->getChildByName("sam")->setVisible(false);
    this->getChildByName("elliott")->setVisible(false);
    this->getChildByName("shane")->setVisible(false);
    this->getChildByName("abigail")->setVisible(false);
    switch (label) {
    case 1:
        // 1号地图——sam
        this->getChildByName("sam")->setVisible(true);
        break;
    case 2:
        // 2号地图——elliott
        this->getChildByName("elliott")->setVisible(true);
        break;

    case 3:
        // 3号地图——shane
        this->getChildByName("shane")->setVisible(true);
        break;

    case 4:
        // 4号地图——abigail
        this->getChildByName("abigail")->setVisible(true);
        break;
    default:
        break;
    }
}
