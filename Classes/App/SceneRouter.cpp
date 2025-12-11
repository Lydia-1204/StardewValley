#include "App/SceneRouter.h"
#include "Scenes/LoadingScene.h"
#include "Scenes/NewSelectScene.h"
#include "Scenes/SaveSelectScene.h"
#include "Scenes/CreateErrorScene.h"
#include "Scenes/MenuScene.h"
#include "Scenes/GameScene.h"

USING_NS_CC;

/* --------------------------------------------------------------------------
 * 【使用外观模式重构】
 * 说明：将场景切换、过渡动画与错误跳转封装为统一入口，隐藏 Director/Transition 等细节，
 *       调用方只需声明路由字符串或参数即可完成切换；集中管理路由逻辑以便统一扩展与维护。
 * -------------------------------------------------------------------------- */

SceneRouter *SceneRouter::instance = nullptr;

SceneRouter *SceneRouter::getInstance()
{
    if (!instance)
        instance = new SceneRouter();
    return instance;
}

void SceneRouter::replaceWithFade(Scene *scene, const Color3B &color)
{
    auto transition = TransitionFade::create(0.5f, scene, color);
    Director::getInstance()->replaceScene(transition);
}

void SceneRouter::goTo(const std::string &route)
{
    if (route == "NewSelect")
    {
        auto scene = NewSelectScene::createScene();
        replaceWithFade(scene, Color3B::BLACK);
        return;
    }
    if (route == "SaveSelect")
    {
        auto scene = SaveSelectScene::createScene();
        replaceWithFade(scene, Color3B::GRAY);
        return;
    }
    if (route == "Menu")
    {
        auto scene = MenuScene::createScene();
        replaceWithFade(scene, Color3B::WHITE);
        return;
    }
    if (route == "Loading")
    {
        auto scene = LoadingScene::createScene();
        auto dir = Director::getInstance();
        if (dir->getRunningScene())
        {
            dir->replaceScene(scene);
        }
        else
        {
            dir->runWithScene(scene);
        }
        return;
    }
}

void SceneRouter::goTo(const std::string &route, int selectedCharacter, const std::string &nickname)
{
    if (route == "Game")
    {
        auto scene = GameScene::createScene(selectedCharacter, nickname);
        replaceWithFade(scene, Color3B::BLACK);
        return;
    }
    goTo(route);
}

void SceneRouter::goToError(const std::string &message)
{
    auto scene = ErrorSceneHelper::createErrorScene(message);
    Director::getInstance()->replaceScene(scene);
}

void SceneRouter::pause()
{
    Director::getInstance()->pause();
}

void SceneRouter::resume()
{
    Director::getInstance()->resume();
}
