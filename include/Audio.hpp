//MIT License

//Copyright (c) 2026 Z-Multiplier
#ifndef AUDIO_HPP
#define AUDIO_HPP
#include <string>
#include <vector>
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <windows.h>
#include <atomic>
namespace Audio{
    struct Noise{
        std::wstring path;
        std::vector<float> cache;
        Noise()=delete;
        Noise(std::wstring path);
    };
    struct PlayRequest{
        std::shared_ptr<Noise> target;
        float volume;
        float pitch;
        bool removeAfterPlay;
        PlayRequest(Noise *tgt,float volume,float pitch,bool removeFlag):target(tgt),volume(volume),pitch(pitch),removeAfterPlay(removeFlag){}
    };
    struct AudioManager{
        private:
            std::atomic<bool> active;
            std::queue<PlayRequest> requestQueue;
            std::thread audioThread;
            std::condition_variable queueCV;
            std::mutex audioMutex;
        public:
            AudioManager(AudioManager&)=delete;
            AudioManager operator=(AudioManager&)=delete;
            AudioManager(AudioManager&&)=default;
            AudioManager operator=(AudioManager&&)=delete;
            void start();
            void stop();
    };
}

#endif