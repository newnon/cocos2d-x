/**
 * @author cesarpachon
 */ 
#include <cstring>
#include "AudioEngine-emcc.h"
#include "cocos2d.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN)
#include <emscripten.h>

using namespace cocos2d;
using namespace cocos2d::experimental;

extern "C" {
    typedef void (*audio_callback)(int audioId, bool result);
    
    // Methods implemented in AudioEngine.js
    void AudioEngine_init();
    
    int AudioEngine_preload(const char *);
    void AudioEngine_unload(const char *);
    int AudioEngine_play2d(const char *, int, float, audio_callback callback);
    
    void AudioEngine_setLoop(int, bool);
    void AudioEngine_setVolume(int, int);
    void AudioEngine_pause(int);
    void AudioEngine_resume(int);
    void AudioEngine_stop(int);
    void AudioEngine_stopAll();
    int AudioEngine_getDuration(int);
    int AudioEngine_getCurrentTime(int);
    void AudioEngine_setCurrentTime(int, float);
    
    void AudioEngine_set_callback(int audioId, audio_callback callback);
    void AudioEngine_set_finish_callback(int audioId, audio_callback callback);
    
    void AudioEngine_setUseFileExt(const char *);
    
    void AudioEngine_end();
};

AudioEngineImpl * g_AudioEngineImpl = nullptr;

AudioEngineImpl::AudioEngineImpl()
{
    AudioEngine_init();
}

AudioEngineImpl::~AudioEngineImpl()
{
    AudioEngine_end();
}

bool AudioEngineImpl::init()
{
    g_AudioEngineImpl = this;
    return true;
};

void AudioEngineImpl::onCallback(int audioId, bool success)
{
    if (g_AudioEngineImpl)
    {
        auto callback = g_AudioEngineImpl->_preloadCallbacks.find(audioId);
        if (callback != g_AudioEngineImpl->_preloadCallbacks.end())
        {
            callback->second(success);
            g_AudioEngineImpl->_preloadCallbacks.erase(callback);
        }
    }
}

void AudioEngineImpl::onPlay2dCallback(int audioId, bool success)
{
    if (g_AudioEngineImpl && success)
    {
        if (AudioEngine::_audioIDInfoMap.find(audioId) != AudioEngine::_audioIDInfoMap.end())
        {
            AudioEngine::_audioIDInfoMap[audioId].state = AudioEngine::AudioState::PLAYING;
        }
    }
}

void AudioEngineImpl::onFinishCallback(int audioId, bool success)
{
    if (g_AudioEngineImpl)
    {
        auto it = g_AudioEngineImpl->_finishCallbacks.find(audioId);
        if (it != g_AudioEngineImpl->_finishCallbacks.end())
        {
            auto fileIt = g_AudioEngineImpl->_audioFiles.find(audioId);
            if (fileIt != g_AudioEngineImpl->_audioFiles.end())
            {
                it->second(audioId, fileIt->second);
                g_AudioEngineImpl->_finishCallbacks.erase(it);
            }
        }
    }
}

int AudioEngineImpl::play2d(const std::string &fileFullPath, bool loop, float volume)
{
    volume = volume > 1.0 ? 1.0 : volume;
    volume = volume < 0.0 ? 0.0 : volume;
    
    int ret = AudioEngine_play2d(fileFullPath.c_str(), loop, (int)(volume * 100), &AudioEngineImpl::onPlay2dCallback);
    if (ret != -1)
    {
        auto fileIt = _audioFiles.find(ret);
        if (fileIt == _audioFiles.end())
        {
            _audioFiles[ret] = fileFullPath;
        }
    }
    return ret;
};

void AudioEngineImpl::setVolume(int audioId, float volume)
{
    // Ensure volume is between 0.0 and 1.0.
    volume = volume > 1.0 ? 1.0 : volume;
    volume = volume < 0.0 ? 0.0 : volume;
    
    AudioEngine_setVolume(audioId, (int)(volume * 100));
};

void AudioEngineImpl::setLoop(int audioId, bool loop)
{
    AudioEngine_setLoop(audioId, loop);
};

bool AudioEngineImpl::pause(int audioId)
{
    AudioEngine_pause(audioId);
    return true;
};

bool AudioEngineImpl::resume(int audioId)
{
    AudioEngine_resume(audioId);
    return true;
};

bool AudioEngineImpl::stop(int audioId)
{
    AudioEngine_stop(audioId);
    return true;
};

void AudioEngineImpl::stopAll()
{
    AudioEngine_stopAll();
};

float AudioEngineImpl::getDuration(int audioId)
{
    float value = AudioEngine_getDuration(audioId);
    return value;
};

float AudioEngineImpl::getCurrentTime(int audioId)
{
    float value = AudioEngine_getCurrentTime(audioId);
    return value;
};

bool AudioEngineImpl::setCurrentTime(int audioId, float time)
{
    AudioEngine_setCurrentTime(audioId, time);
    return true;
};

void AudioEngineImpl::setFinishCallback(int audioId, const FinishCallback &callback)
{
    if (audioId != -1)
    {
        AudioEngine_set_finish_callback(audioId, &AudioEngineImpl::onFinishCallback);
        
        auto callbackIt = _finishCallbacks.find(audioId);
        if (callbackIt == _finishCallbacks.end())
        {
            _finishCallbacks[audioId] = callback;
        }
    }
};

void AudioEngineImpl::uncache(const std::string& path)
{
    for (auto &it : _audioFiles)
    {
        if (it.second == path)
        {
            AudioEngine_unload(path.c_str());
            _preloadCallbacks.erase(it.first);
            _audioFiles.erase(it.first);
            _finishCallbacks.erase(it.first);
            break;
        }
    }
};

void AudioEngineImpl::uncacheAll()
{
    for (auto &it : _audioFiles)
    {
        AudioEngine_unload(it.second.c_str());
        _audioFiles.erase(it.first);
        _finishCallbacks.erase(it.first);
    }
    
    _preloadCallbacks.clear();
};

int AudioEngineImpl::preload(const std::string& filePath, const Callback &callback)
{
    int ret = AudioEngine_preload(filePath.c_str());
    
    if (ret != -1)
    {
        AudioEngine_set_callback(ret, &AudioEngineImpl::onCallback);
        
        auto callbackIt = _preloadCallbacks.find(ret);
        if (callbackIt == _preloadCallbacks.end())
        {
            _preloadCallbacks[ret] = callback;
        }
        
        auto fileIt = _audioFiles.find(ret);
        if (fileIt == _audioFiles.end())
        {
            _audioFiles[ret] = filePath;
        }
    }
    
    return ret;
};

void AudioEngineImpl::setUseFileExt(const std::vector<std::string> &exts)
{
    std::string transferExts;
    for (int i = 0; i < exts.size(); ++i)
    {
        transferExts += exts[i];
        if (i + 1 < exts.size())
            transferExts += ",";
    }
    
    AudioEngine_setUseFileExt(transferExts.c_str());
}

void AudioEngineImpl::update(float dt)
{
};

#endif
