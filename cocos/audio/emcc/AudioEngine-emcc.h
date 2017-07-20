/****************************************************************************
 Copyright (c) 2015 Chukong Technologies Inc.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/
#include "platform/CCPlatformConfig.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_EMSCRIPTEN

#ifndef __AUDIO_ENGINE_EMCC_H_
#define __AUDIO_ENGINE_EMCC_H_

#include <functional>
#include <iostream>
#include <map>
#include "audio/include/AudioEngine.h"

#include "base/CCRef.h"

NS_CC_BEGIN
    namespace experimental{
#define MAX_AUDIOINSTANCES 32

class CC_DLL AudioEngineImpl : public cocos2d::Ref
{
    using Callback = std::function<void(bool isSuccess)>;
    using FinishCallback = std::function<void (int, const std::string &)>;
public:
    AudioEngineImpl();
    ~AudioEngineImpl();
    
    bool init();
    int play2d(const std::string &fileFullPath ,bool loop ,float volume);
    void setVolume(int audioId,float volume);
    void setLoop(int audioId, bool loop);
    bool pause(int audioId);
    bool resume(int audioId);
    bool stop(int audioId);
    void stopAll();
    float getDuration(int audioId);
    float getCurrentTime(int audioId);
    bool setCurrentTime(int audioId, float time);
    void setFinishCallback(int audioId, const FinishCallback &callback);
    
    void uncache(const std::string& filePath);
    void uncacheAll();

    int preload(const std::string& filePath, const Callback &callback);
    
    void setUseFileExt(const std::vector<std::string> &exts);
    
    void update(float dt);
    
protected:
    static void onCallback(int audioId, bool success);
    static void onPlay2dCallback(int audioId, bool success);
    static void onFinishCallback(int audioId, bool success);
    
private:
    std::map<int, std::string> _audioFiles;
    std::map<int, Callback> _preloadCallbacks;
    std::map<int, FinishCallback> _finishCallbacks;
};
}
NS_CC_END
#endif // __AUDIO_ENGINE_EMCC_H_
#endif

