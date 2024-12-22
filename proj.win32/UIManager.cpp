/****************************************************************
 * Project Name:  StardewValley
 * File Name:     SystemScene.h
 * File Function: ����UIManager�࣬����ʵ����Ϸ������ ������ͼ���ء��ƶ� ��ɫ�ƶ� ϵͳ���� �̶�UI
 * Author:        ��С�� 2351882
 * Update Date:   2024/12/10
 ****************************************************************/
#include "UIManager.h"
#include "cocos2d.h"
#include"MenuScene.h"
#include"Constant.h"
#include"Chest.h"
#include"Player.h"

USING_NS_CC;

UIManager* UIManager::instance = nullptr;

UIManager::UIManager()
    : dateLabel(nullptr), timeLabel(nullptr), moneyLabel(nullptr),
    energyBar(nullptr), iron(nullptr),
    shortcutKeysLabel(nullptr), money(0), selectedCharacter(0),
    currentMonth(3), currentDay(1), currentWeekday(2), // 3��1�գ��ܶ�
    currentEnergy(100), timeElapsed(0.0f) , isPriceBoardOpen(0){}

UIManager* UIManager::getInstance(int& selectedCharacter, const std::string& nickname) {
    if (!instance) {
        // ���ʵ�������ڣ���������ʼ����
        instance = new (std::nothrow) UIManager();
        if (instance && !instance->init(selectedCharacter, nickname)) {
            // �����ʼ��ʧ�ܣ��ͷ�ʵ�������� nullptr
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

    currentMonth = 12;                 // ��ǰ�·�
    currentDay = 8;                    // ��ǰ����
    currentWeekday = 6;                // ��ǰ�ܼ���0=���գ�1=��һ��...��6=������
    currentHour = 8;                   // ��ǰСʱ
    currentMinute = 0;                 // ��ǰ����
    currentEnergy = 100;                 // ��ǰ����ֵ���ٷֱȣ�
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->nickname = nickname;
    this->selectedCharacter = selectedCharacter;
    mapManager = MapManager::getInstance();
    // ���ں�ʱ����ʾ
    dateLabel = Label::createWithTTF("December 8st, Tuesday", "../Resources/fonts/Marker Felt.ttf", 24);
    dateLabel->setPosition(visibleSize.width - 200, visibleSize.height - 50);
    this->addChild(dateLabel);

    timeLabel = Label::createWithTTF("08:00", "../Resources/fonts/Marker Felt.ttf", 24);
    timeLabel->setPosition(visibleSize.width - 200, visibleSize.height - 150);
    this->addChild(timeLabel,20);


    // ��ʼ��NPC*******************
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

    //��ʼֻ��Elliott��ʾ
    sam->setVisible(false);
    abigail->setVisible(false);
    shane->setVisible(false);


    // ����ͼ��
    iron = Sprite::create("../Resources/LooseSprites-73/iron123.png");
    if (!iron)
        throw("iron created failed!!");
    iron->setPosition(visibleSize.width - 200, visibleSize.height - 100);
    this->addChild(iron,20);


    // �ʽ���ʾ
    moneyLabel = Label::createWithTTF("Money: $0", "../Resources/fonts/Marker Felt.ttf", 24);
    moneyLabel->setPosition(visibleSize.width - 200, visibleSize.height - 200);
    this->addChild(moneyLabel,20);

    //������ʾ
   weatherLabel = Label::createWithTTF("Sunny", "../Resources/fonts/Marker Felt.ttf", 24);
   weatherLabel->setPosition(visibleSize.width - 200, visibleSize.height - 250);
   this->addChild(weatherLabel,20);
   

    // ������
    energyBar = ProgressTimer::create(Sprite::create("../Resources/energyBar.png"));
    if (!energyBar)
        throw("energyBar created failed!!");
    energyBar->setScale(0.4f);

    energyBar->setType(ProgressTimer::Type::BAR);

    energyBar->setMidpoint(Vec2(0, 0));
    energyBar->setBarChangeRate(Vec2(1, 0));
    energyBar->setPercentage(100);
    energyBar->setPosition(visibleSize.width - 30, visibleSize.height * 0.25);
    this->addChild(energyBar,20);
    //������label
    energyLabel = Label::createWithTTF("energy:100", "../Resources/fonts/Marker Felt.ttf", 18);
    energyLabel->setPosition(visibleSize.width - 50, visibleSize.height * 0.12);
    this->addChild(energyLabel,20);
    // ��ݼ���ʾ��
    shortcutKeysLabel = Label::createWithTTF("E: Pause\nC: Use Tool\nV: Discard Tool\nM: Mini Map\nF: Task List\nP:sjill tree", "../Resources/fonts/Marker Felt.ttf", 18);
    shortcutKeysLabel->setPosition(100, visibleSize.height - 100);
    this->addChild(shortcutKeysLabel,20);
    /*
    // ������
    auto tool1 = Sprite::create("../Resources/LooseSprites-73/textBox..png");
    setToolBar({ tool1 }); // ��ʼ��������
    */


    // ��ʼ����ͣ���
    pausePanel = LayerColor::create(Color4B(20, 20, 20, 220)); // ��ɫ�Ҽ�����͸��
    pausePanel->setVisible(false);
    this->addChild(pausePanel);

    // ��Ӱ�ť "Exit to Menu"
    auto exitButton = MenuItemLabel::create(
        Label::createWithTTF("Exit Game", "../Resources/fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            CCLOG("Exit  clicked");
            static bool isSwitchingScene = false;
            if (isSwitchingScene) return;
            isSwitchingScene = true;
            Director::getInstance()->end();
            /*
            Director::getInstance()->resume(); // �ָ���Ϸ״̬
            //this->clearResources();
            // ִ�з������˵����߼�
            auto menuScene = MenuScene::createScene();
            Director::getInstance()->replaceScene(TransitionFade::create(0.5f, menuScene, Color3B::BLACK));
            */
        }
    );
    exitButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50));

    // ��Ӱ�ť "Save Progress"
    auto saveButton = MenuItemLabel::create(
        Label::createWithTTF("Save Progress", "../Resources/fonts/Marker Felt.ttf", 24),
        [](Ref* sender) {
            CCLOG("Save Progress clicked");
            // ִ�б�����Ϸ���ȵ��߼�
        }
    );
    saveButton->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));

    auto menu = Menu::create(exitButton, saveButton, nullptr);
    menu->setPosition(Vec2::ZERO);
    pausePanel->addChild(menu);

    // С��ͼ
    miniMap = Sprite::create("../Resources/UI/miniMap.jpg");
    if (!miniMap) throw std::runtime_error("Failed to load minimap texture!");
    miniMap->setPosition(visibleSize.width / 2,visibleSize.height / 2);
    miniMap->setVisible(false);
    this->addChild(miniMap);

    // ��Ӻ���ע��ɫλ��
    playerMarker = Sprite::create("../Resources/UI/playerStar.png");
    playerMarker->setPosition(Vec2(miniMap->getContentSize().width / 2, miniMap->getContentSize().height / 2)); // Ĭ�Ͼ���
    miniMap->addChild(playerMarker);

    // ��� "Your Position" �ı�
    auto positionLabel = Label::createWithTTF("Your Position", "../Resources/fonts/Marker Felt.ttf", 10);
    positionLabel->setColor(Color3B::RED);
    positionLabel->setPosition(Vec2(playerMarker->getContentSize().width / 2, -10)); // ���������·�
    playerMarker->addChild(positionLabel,10);

    // �������
    taskListPanel = LayerColor::create(Color4B(40, 40, 40, 200)); // ǳ��ɫ����
    taskListPanel->setContentSize(Size(400, 300)); // ������������С
    taskListPanel->setPosition((visibleSize.width - taskListPanel->getContentSize().width) / 2,
        (visibleSize.height - taskListPanel->getContentSize().height) / 2);
    taskListPanel->setVisible(false);
    this->addChild(taskListPanel);

    // ��ʼ�������б�
    for (int i = 0; i < 4; i++) {
        Label* taskLabel;
        switch (i) {
        case 0:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i+1) + " : plant a crop", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        case 1:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i+1) + " : go mining ", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        case 2:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i+1) + " : send a gift to npc", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        case 3:
            taskLabel = Label::createWithTTF("Task  " + std::to_string(i+1) + " : buy a upgrade tool", "../Resources/fonts/Marker Felt.ttf", 20);
            break;
        }
        taskLabel->setAnchorPoint(Vec2(0, 0.5f));
        taskLabel->setPosition(Vec2(20, taskListPanel->getContentSize().height - 30 * (i + 1)));
        taskListPanel->addChild(taskLabel);
        taskLabels.push_back(taskLabel);
    }
    
    // ������ʾ�Ի�����(�� emotes.png �н�ȡ)
    std::string dialogTexture = "emotes.png";
    dialogSprite = Sprite::create(dialogTexture);
    // ��ȡͼƬ��ԭʼ�ߴ�
    Size originalSize = dialogSprite->getContentSize();
    // �����ķ�֮һ�Ŀ��1/16��
    float quarterWidth = originalSize.width / 4.0f;
    float quarterHeight = originalSize.height / 16.0f;
    // ��������ü�����
    Rect cutRect(3 * quarterWidth, 10 * quarterHeight, quarterWidth, quarterHeight);
    dialogSprite->setTextureRect(cutRect);
    dialogSprite->setScale(1.4f);
    dialogSprite->setOpacity(0); // ��ʼ����
    this->addChild(dialogSprite, 5);

    // �����Ի�����(��textBox.png �н�ȡ)
    std::string chatTexture = "textBox.png";
    chatSprite = Sprite::create(chatTexture);
    chatSprite->setPosition(600, 650);
    chatSprite->setScale(3);
    chatSprite->setVisible(0); // ��ʼ����
    this->addChild(chatSprite, 999);
    /*
     // ���� dialogTextLabel��������ʾ�Ի�����
    dialogTextLabel = Label::createWithTTF("i am NPC", "../Resources/fonts/Marker Felt.ttf", 18);
    dialogTextLabel->setTextColor(Color4B(0, 0, 0, 255));// black
    dialogTextLabel->setPosition(600, 680);
    dialogTextLabel->setVisible(0); // ��ʼ����
    this->addChild(dialogTextLabel, 999);

    // ���Ի����������ӵ�����
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
        //��������ʱ����ʾ
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
    // currentEnergy ��ʾ��ǰ����ֵ��0-100֮�䣩
    currentEnergy += deltaEnergy;

    // ��ֹ����ֵС��0
    if (currentEnergy < 0) {
        currentEnergy = 0;
    }

    // ��ֹ����ֵ����100
    else if (currentEnergy > 100) {
        currentEnergy = 100;
    }

    // ���¾�����
    setEnergy(currentEnergy);
}

void UIManager::setDateAndTime(const std::string& date, const std::string& time) {
    if (dateLabel) {
        dateLabel->setString(date); // ����������ʾ
    }
    if (timeLabel) {
        timeLabel->setString(time); // ����ʱ����ʾ
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
    // ��յ�ǰ������
    for (auto& child : getChildren()) {
        if (child->getName() == "Tool") {
            this->removeChild(child);
        }
    }

    // ����µĹ���
    const float startX = 600;  // ��ʼλ��
    for (size_t i = 0; i < tools.size(); ++i) {
        tools[i]->setName("Tool");
        tools[i]->setPosition(Vec2(startX + i * 10, 50)); // ÿ�����ߵļ��Ϊ 50
        this->addChild(tools[i]);
    }
}*/
void UIManager::toggleMiniMap(const Vec2& playerPos, const Size& mapSize) {
    isMiniMapVisible = !isMiniMapVisible;
    miniMap->setVisible(isMiniMapVisible);

    if (isMiniMapVisible) {

        auto screenSize = Director::getInstance()->getVisibleSize();
        
        float addWidth=0, addHeight=0;
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
        else if(mapManager->currentMapLabel==5){
            Vec2 normalizedPos(0.25, 0.8); 
            playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x, miniMap->getContentSize().height * normalizedPos.y));
        }
        else if (mapManager->currentMapLabel == 6) {
            Vec2 normalizedPos(4.0 / 10.5f, 1.0 / 6.5f); 
            playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x, miniMap->getContentSize().height * normalizedPos.y));
        }
        else if (mapManager->currentMapLabel == 7) {
            Vec2 normalizedPos(7.2/10.5f, 5.8/6.5f); 
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

    // �����۸����ͼƬ
    auto priceListBg = Sprite::create("../Resources/priceBoard.jpg");
    priceListBg->setPosition(visibleSize.width / 2, visibleSize.height / 2);
  
    priceListBg->setName("PriceBoard");
    this->addChild(priceListBg, 10);
    isPriceBoardOpen = 1;
    // �رհ�ť
    auto closeButton = MenuItemImage::create("../Resources/close.png", "../Resources/close.png",
        [=](Ref* sender) {
            this->closePriceBoard();
        });
    closeButton->setScale(4.0f);
    auto menu = Menu::create(closeButton, nullptr);
    menu->setPosition(priceListBg->getPositionX() + priceListBg->getContentSize().width / 4,
        priceListBg->getPositionY() + priceListBg->getContentSize().height / 4 );
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
    //  �Ⱦ����ɼ�
    this->getChildByName("sam")->setVisible(false);
    this->getChildByName("elliott")->setVisible(false);
    this->getChildByName("shane")->setVisible(false);
    this->getChildByName("abigail")->setVisible(false);
    switch (label) {
    case 1:
        // 1�ŵ�ͼ����sam
        this->getChildByName("sam")->setVisible(true);
        break;
    case 2:
        // 2�ŵ�ͼ����elliott
        this->getChildByName("elliott")->setVisible(true);
        break;

    case 3:
        // 3�ŵ�ͼ����shane
        this->getChildByName("shane")->setVisible(true);
        break;

    case 4:
        // 4�ŵ�ͼ����abigail
        this->getChildByName("abigail")->setVisible(true);
        break;
    default:
        break;
    }
}