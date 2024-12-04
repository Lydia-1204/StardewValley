/****************************************************************************
 版权所有 (c) 2017-2018 厦门雅基软件有限公司.

 http://www.cocos2d-x.org

 特此免费授予任何获得本软件和相关文档文件（“软件”）的个人无限制地处理本软件的权利，包括但不限于使用、复制、修改、合并、出版、发行、再授权和/或出售本软件的副本，并允许被提供本软件的人员这样做，条件如下：

 上述版权声明和本许可声明应包含在所有本软件的副本或重要部分中。

 本软件按“原样”提供，不提供任何形式的明示或暗示保证，包括但不限于对适销性、特定用途的适用性和非侵权性的保证。在任何情况下，即使因合同行为、侵权行为或其他原因，作者或版权持有人也不对任何索赔、损害或其他责任负责，无论是由于本软件或本软件的使用或其他交易引起的，或与之相关的。
 ****************************************************************************/

#include "AppDelegate.h"
#include "MainScene.h"

 // #define USE_AUDIO_ENGINE 1
 // #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "不要同时使用AudioEngine和SimpleAudioEngine。请在您的游戏中只选择一个！"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(480, 320);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1024, 768);
static cocos2d::Size largeResolutionSize = cocos2d::Size(2048, 1536);

AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

// 如果你想要不同的上下文，请修改glContextAttrs的值
// 它将影响所有平台
void AppDelegate::initGLContextAttrs()
{
    // 设置OpenGL上下文属性：红、绿、蓝、alpha、深度、模板、多样本计数
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// 如果你想要使用包管理器安装更多包，
// 不要修改或删除这个函数
static int register_all_packages()
{
    return 0; // 包管理器的标志
}

bool AppDelegate::applicationDidFinishLaunching() {
    // 初始化导演
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if (!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("StardewValley0", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("StardewValley0");
#endif
        director->setOpenGLView(glview);
    }

    // 打开显示FPS
    director->setDisplayStats(true);

    // 设置FPS。如果你不调用这个，默认值是1.0/60
    director->setAnimationInterval(1.0f / 60);

    // 设置设计分辨率
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    // 设置内容缩放因子
    director->setContentScaleFactor(2.0f); // 根据需要调整

    register_all_packages();

    // 创建一个场景。它是一个自动释放的对象
    auto scene = MainScene::createScene();

    // 运行
    director->runWithScene(scene);

    return true;
}
// 当应用程序处于非活动状态时，将调用此函数。注意，当接收到电话时，将调用它。
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// 当应用程序再次处于活动状态时，将调用此函数
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}