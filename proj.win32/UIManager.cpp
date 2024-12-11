#include "UIManager.h"
#include "cocos2d.h"

USING_NS_CC;

UIManager* UIManager::instance = nullptr;

UIManager::UIManager()
    : dateLabel(nullptr), timeLabel(nullptr), moneyLabel(nullptr),
    energyBar(nullptr), iron(nullptr),
    shortcutKeysLabel(nullptr), money(0), selectedCharacter(0),
    currentMonth(3), currentDay(1), currentWeekday(2), // 3月1日，周二
    currentEnergy(100), timeElapsed(0.0f) {}
UIManager* UIManager::getInstance(int selectedCharacter, const std::string& nickname) {
    if (!instance) {
        instance = new (std::nothrow) UIManager();
        instance->init(selectedCharacter, nickname);
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
   currentMinute =0;                 // 当前分钟
    currentEnergy = 100;                 // 当前精力值（百分比）
    auto visibleSize = Director::getInstance()->getVisibleSize();
    this->nickname = nickname;
    this->selectedCharacter = selectedCharacter;

    // 日期和时间显示
    dateLabel = Label::createWithTTF("December 8st, Tuesday", "../Resources/fonts/Marker Felt.ttf", 24);
    dateLabel->setPosition(visibleSize.width - 200, visibleSize.height - 50);
    this->addChild(dateLabel);

    timeLabel = Label::createWithTTF("08:00", "../Resources/fonts/Marker Felt.ttf", 24);
    timeLabel->setPosition(visibleSize.width - 200, visibleSize.height - 150);
    this->addChild(timeLabel);

    // 美观图标
    
    iron = Sprite::create("../Resources/LooseSprites-73/iron123.png");
    if (!iron)
        throw("iron created failed!!");
    iron->setPosition(visibleSize.width - 200, visibleSize.height - 100);
    this->addChild(iron);


    // 资金显示
    moneyLabel = Label::createWithTTF("Money: $0", "../Resources/fonts/Marker Felt.ttf", 24);
    moneyLabel->setPosition(visibleSize.width - 200, visibleSize.height - 200);
    this->addChild(moneyLabel);

    // 精力条
    energyBar = ProgressTimer::create(Sprite::create("../Resources/energyBar.png"));
    if (!energyBar)
        throw("energyBar created failed!!");
    energyBar->setScale(0.4f);

    energyBar->setType(ProgressTimer::Type::BAR);

    energyBar->setMidpoint(Vec2(0, 0));
    energyBar->setBarChangeRate(Vec2(1, 0));
    energyBar->setPercentage(100);
    energyBar->setPosition(visibleSize.width - 30, visibleSize.height*0.25);
    this->addChild(energyBar);

    // 快捷键提示框
    shortcutKeysLabel = Label::createWithTTF("E: Exit\nC: Use Tool\nM: Mini Map\nF: Task List", "../Resources/fonts/Marker Felt.ttf", 18);
    shortcutKeysLabel->setPosition(100, visibleSize.height - 100);
    this->addChild(shortcutKeysLabel);

    // 工具栏
    auto tool1 = Sprite::create("../Resources/LooseSprites-73/textBox..png");
    setToolBar({ tool1 }); // 初始化工具栏

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

void UIManager::setMoney(int money) {
    this->money = money;
    moneyLabel->setString("Money: $" + std::to_string(money));
}

void UIManager::setEnergy(int energy) {
    energyBar->setPercentage(energy);
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
}