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
    
    
    void AudioEngine_rewindBackgroundMusic();
    void AudioEngine_unloadEffect(const char *);
    
    int AudioEngine_preloadEffect(const char *, audio_callback callback);
    int AudioEngine_play2d(const char *, int, float);
    
    
    void AudioEngine_stopEffect(int);
    void AudioEngine_setEffectsVolume(int);
    void AudioEngine_pauseEffect(int);
    void AudioEngine_resumeEffect(int);
    void AudioEngine_pauseAllEffects();
    void AudioEngine_resumeAllEffects();
    void AudioEngine_stopAllEffects();
    
    void AudioEngine_setUseFileExt(const char *);
    
    void AudioEngine_end();
};


AudioEngineImpl * g_AudioEngineImpl = nullptr;


AudioEngineImpl::AudioEngineImpl()
{
};

AudioEngineImpl::~AudioEngineImpl(){
};

    
bool AudioEngineImpl::init()
{
    g_AudioEngineImpl = this;
    
    AudioEngine_init();
    
    return true;
};

static void onCallback(int audioID, bool success)
{
}

int AudioEngineImpl::play2d(const std::string &fileFullPath, bool loop, float volume)
{
    int ret = AudioEngine_play2d(fileFullPath.c_str(), loop, volume);
    return ret;
};

void AudioEngineImpl::setVolume(int audioID, float volume)
{
    // Ensure volume is between 0.0 and 1.0.
//    volume = volume > 1.0 ? 1.0 : volume;
//    volume = volume < 0.0 ? 0.0 : volume;
//    
//    SimpleAudioEngine_setBackgroundMusicVolume((int) (volume * 100));
//    s_backgroundVolume = volume;
};

void AudioEngineImpl::setLoop(int audioID, bool loop){
};

bool AudioEngineImpl::pause(int audioID){
  return false;
};

bool AudioEngineImpl::resume(int audioID){
  return false;
};

bool AudioEngineImpl::stop(int audioID){
  return false;
};

void AudioEngineImpl::stopAll(){
};

float AudioEngineImpl::getDuration(int audioID){
  return 0.0f;
};

float AudioEngineImpl::getCurrentTime(int audioID){
  return 0.0f;
};

bool AudioEngineImpl::setCurrentTime(int audioID, float time){
  return false;
};

void AudioEngineImpl::setFinishCallback(int audioID, const std::function<void (int, const std::string &)> &callback){
}; 

void AudioEngineImpl::uncache(const std::string& path){
};


void AudioEngineImpl::uncacheAll(){
};

    
int AudioEngineImpl::preload(const std::string& filePath, const Callback &callback)
{
    int ret = AudioEngine_preloadEffect(filePath.c_str(), &onCallback);
    
    auto it = _preloadCallbacks.find(ret);
    if (it == _preloadCallbacks.end())
    {
    }
    
    
    return 0;
};


void AudioEngineImpl::update(float dt){
};

#endif
