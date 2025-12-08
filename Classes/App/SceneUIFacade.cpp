#include "App/SceneUIFacade.h"
#include "cocos2d.h"
using namespace cocos2d;

USING_NS_CC;

SceneUIFacade* SceneUIFacade::instance = nullptr;

SceneUIFacade* SceneUIFacade::getInstance() {
    if (!instance) instance = new SceneUIFacade();
    return instance;
}

bool SceneUIFacade::applyBackground(Scene* target, const std::string& imagePath) {
    auto background = Sprite::create(imagePath);
    if (!background) return false;

    auto screenSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    auto spriteSize = background->getContentSize();

    float scaleX = screenSize.width / spriteSize.width;
    float scaleY = screenSize.height / spriteSize.height;
    float scale = std::max(scaleX, scaleY);

    background->setPosition(Vec2(screenSize.width / 2 + origin.x, screenSize.height / 2 + origin.y));
    background->setScale(scale);
    target->addChild(background);
    return true;
}

MenuItemImage* SceneUIFacade::createLabeledButton(
    const std::string& imagePath,
    const std::string& text,
    const std::string& fontPath,
    float fontSize,
    const Size& screenSize,
    float widthRatio,
    float heightRatio,
    const Vec2& position,
    const std::function<void(Ref*)>& onClick) {
    auto button = MenuItemImage::create(imagePath, imagePath, onClick);
    if (!button) return nullptr;
    auto label = Label::createWithTTF(text, fontPath, fontSize);
    if (label) {
        label->setPosition(button->getContentSize() / 2);
        button->addChild(label);
    }
    float scaleX = (screenSize.width * widthRatio) / button->getContentSize().width;
    float scaleY = (screenSize.height * heightRatio) / button->getContentSize().height;
    button->setScale(scaleX, scaleY);
    button->setPosition(position);
    return button;
}
