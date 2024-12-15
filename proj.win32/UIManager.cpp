#include "UIManager.h"
#include "cocos2d.h"

USING_NS_CC;

UIManager* UIManager::instance = nullptr;

UIManager::UIManager()
    : dateLabel(nullptr), timeLabel(nullptr), moneyLabel(nullptr),
    energyBar(nullptr), iron(nullptr),
    shortcutKeysLabel(nullptr), money(0), selectedCharacter(0),
    currentMonth(3), currentDay(1), currentWeekday(2), // 3��1�գ��ܶ�
    currentEnergy(100), timeElapsed(0.0f) {}

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

    // ���ں�ʱ����ʾ
    dateLabel = Label::createWithTTF("December 8st, Tuesday", "../Resources/fonts/Marker Felt.ttf", 24);
    dateLabel->setPosition(visibleSize.width - 200, visibleSize.height - 50);
    this->addChild(dateLabel);

    timeLabel = Label::createWithTTF("08:00", "../Resources/fonts/Marker Felt.ttf", 24);
    timeLabel->setPosition(visibleSize.width - 200, visibleSize.height - 150);
    this->addChild(timeLabel);

    // ����ͼ��

    iron = Sprite::create("../Resources/LooseSprites-73/iron123.png");
    if (!iron)
        throw("iron created failed!!");
    iron->setPosition(visibleSize.width - 200, visibleSize.height - 100);
    this->addChild(iron);


    // �ʽ���ʾ
    moneyLabel = Label::createWithTTF("Money: $0", "../Resources/fonts/Marker Felt.ttf", 24);
    moneyLabel->setPosition(visibleSize.width - 200, visibleSize.height - 200);
    this->addChild(moneyLabel);

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
    this->addChild(energyBar);
    //������label
    energyLabel = Label::createWithTTF("energy:100", "../Resources/fonts/Marker Felt.ttf", 18);
    energyLabel->setPosition(visibleSize.width - 50, visibleSize.height * 0.12);
    this->addChild(energyLabel);
    // ��ݼ���ʾ��
    shortcutKeysLabel = Label::createWithTTF("E: Exit\nC: Use Tool\nV: Discard Tool\nM: Mini Map\nF: Task List", "../Resources/fonts/Marker Felt.ttf", 18);
    shortcutKeysLabel->setPosition(100, visibleSize.height - 100);
    this->addChild(shortcutKeysLabel);
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
    miniMap = Sprite::create("../Resources/UI/minimap.png");
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
    positionLabel->setPosition(Vec2(miniMap->getContentSize().width / 2, 10)); // ����С��ͼ�·�
    miniMap->addChild(positionLabel);

    // �������
    taskListPanel = LayerColor::create(Color4B(40, 40, 40, 200)); // ǳ��ɫ����
    taskListPanel->setContentSize(Size(400, 300)); // ������������С
    taskListPanel->setPosition((visibleSize.width - taskListPanel->getContentSize().width) / 2,
        (visibleSize.height - taskListPanel->getContentSize().height) / 2);
    taskListPanel->setVisible(false);
    this->addChild(taskListPanel);

    // ��ʼ�������б�
    for (int i = 0; i < 5; i++) {
        auto taskLabel = Label::createWithTTF("Task  "+ std::to_string(i)+" :", "../Resources/fonts/Marker Felt.ttf", 20);
        taskLabel->setAnchorPoint(Vec2(0, 0.5f));
        taskLabel->setPosition(Vec2(20, taskListPanel->getContentSize().height - 30 * (i + 1)));
        taskListPanel->addChild(taskLabel);
        taskLabels.push_back(taskLabel);
    }

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

void UIManager::setMoney(int money) {
    this->money = money;
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
        Vec2 normalizedPos(playerPos.x / mapSize.width, playerPos.y / mapSize.height);
        playerMarker->setPosition(Vec2(miniMap->getContentSize().width * normalizedPos.x,
            miniMap->getContentSize().height * normalizedPos.y));
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