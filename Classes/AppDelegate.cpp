/****************************************************************************
 ��Ȩ���� (c) 2017-2018 �����Ż�������޹�˾.

 http://www.cocos2d-x.org

 �ش���������κλ�ñ����������ĵ��ļ�������������ĸ��������Ƶش��������Ȩ����������������ʹ�á����ơ��޸ġ��ϲ������桢���С�����Ȩ��/����۱�����ĸ������������ṩ���������Ա���������������£�

 ������Ȩ�����ͱ��������Ӧ���������б�����ĸ�������Ҫ�����С�

 ���������ԭ�����ṩ�����ṩ�κ���ʽ����ʾ��ʾ��֤�������������ڶ������ԡ��ض���;�������Ժͷ���Ȩ�Եı�֤�����κ�����£���ʹ���ͬ��Ϊ����Ȩ��Ϊ������ԭ�����߻��Ȩ������Ҳ�����κ����⡢�𺦻��������θ������������ڱ�����������ʹ�û�������������ģ�����֮��صġ�
 ****************************************************************************/

#include "AppDelegate.h"
#include "MainScene.h"

 // #define USE_AUDIO_ENGINE 1
 // #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "��Ҫͬʱʹ��AudioEngine��SimpleAudioEngine������������Ϸ��ֻѡ��һ����"
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

// �������Ҫ��ͬ�������ģ����޸�glContextAttrs��ֵ
// ����Ӱ������ƽ̨
void AppDelegate::initGLContextAttrs()
{
    // ����OpenGL���������ԣ��졢�̡�����alpha����ȡ�ģ�塢����������
    GLContextAttrs glContextAttrs = { 8, 8, 8, 8, 24, 8, 0 };

    GLView::setGLContextAttrs(glContextAttrs);
}

// �������Ҫʹ�ð���������װ�������
// ��Ҫ�޸Ļ�ɾ���������
static int register_all_packages()
{
    return 0; // ���������ı�־
}

bool AppDelegate::applicationDidFinishLaunching() {
    // ��ʼ������
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

    // ����ʾFPS
    director->setDisplayStats(true);

    // ����FPS������㲻���������Ĭ��ֵ��1.0/60
    director->setAnimationInterval(1.0f / 60);

    // ������Ʒֱ���
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);

    // ����������������
    director->setContentScaleFactor(2.0f); // ������Ҫ����

    register_all_packages();

    // ����һ������������һ���Զ��ͷŵĶ���
    auto scene = MainScene::createScene();

    // ����
    director->runWithScene(scene);

    return true;
}
// ��Ӧ�ó����ڷǻ״̬ʱ�������ô˺�����ע�⣬�����յ��绰ʱ������������
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// ��Ӧ�ó����ٴδ��ڻ״̬ʱ�������ô˺���
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}