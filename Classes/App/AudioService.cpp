#include "App/AudioService.h"
#include "AudioEngine.h"
#include "audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;
using namespace cocos2d::experimental;

/* --------------------------------------------------------------------------
 * 【使用外观模式重构】
 * 说明：统一封装音效与背景音乐的预加载、播放与停止接口，屏蔽 SimpleAudioEngine 与 AudioEngine 差异，
 *       让业务层仅依赖 AudioService，降低对底层音频库的耦合并便于后续替换与扩展。
 * -------------------------------------------------------------------------- */

AudioService *AudioService::instance = nullptr;

AudioService *AudioService::getInstance()
{
    if (!instance)
        instance = new AudioService();
    return instance;
}

void AudioService::preloadEffect(const std::string &path)
{
    SimpleAudioEngine::getInstance()->preloadEffect(path.c_str());
}

void AudioService::preloadEffects(const std::vector<std::string> &paths)
{
    for (const auto &p : paths)
        SimpleAudioEngine::getInstance()->preloadEffect(p.c_str());
}

void AudioService::playEffect(const std::string &path)
{
    SimpleAudioEngine::getInstance()->playEffect(path.c_str());
}

void AudioService::stopAll()
{
    SimpleAudioEngine::getInstance()->stopAllEffects();
}

void AudioService::playBgm(const std::string &path, bool loop, float volume)
{
    cocos2d::experimental::AudioEngine::play2d(path, loop, volume);
}
