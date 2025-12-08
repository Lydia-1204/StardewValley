#pragma once
#ifndef AUDIOSERVICE_H
#define AUDIOSERVICE_H

#include <string>
#include <vector>


class AudioService {
public:
    static AudioService* getInstance();
    void preloadEffect(const std::string& path);
    void preloadEffects(const std::vector<std::string>& paths);
    void playEffect(const std::string& path);
    void stopAll();
    void playBgm(const std::string& path, bool loop, float volume);

private:
    static AudioService* instance;
};

#endif
