var LibraryAudioEngine = {
    AudioEngine_init: function()
    {
        Module.CocosAudioState = {
            audioMap: {},
            audioMapNum: {},
            numAudioCount: 0,
            formats: [],
            preloadCallbacks: {},
            play2dCallbacks: {},
            finishCallbacks: {}
        };
    },

    AudioEngine_setUseFileExt: function(extFiles)
    {
        var exts = Pointer_stringify(extFiles);
        Module.CocosAudioState.formats = exts.split(',');
    },

    AudioEngine_end: function()
    {
        buzz.all().stop();
    },
    
    __set_callback: function(audioId, event, callback)
    {
        function _callback(audioId, result)
        {
            try
            {
                var sp = Runtime.stackSave();
                Runtime.dynCall('vii', callback, [audioId, result]);
                Runtime.stackRestore(sp);
            }
            catch (e)
            {
                if (e instanceof ExitStatus)
                {
                    return;
                }
                else
                {
                    if (e && typeof e === 'object' && e.stack) 
                        console.log('exception thrown: ' + [e, e.stack]);
                }
            }
        };

        if (event == "play2d")
        {
            Module.CocosAudioState.play2dCallbacks[audioId] = _callback;
        }
        else if (event == "preload")
        {
            Module.CocosAudioState.preloadCallbacks[audioId] = _callback;
        }
        else if (event == "finish")
        {
            Module.CocosAudioState.finishCallbacks[audioId] = _callback;
        }
    },

    AudioEngine_set_callback__deps: ['__set_callback'],
    AudioEngine_set_callback: function(audioId, callback)
    {
        ___set_callback(audioId, "preload", callback);
    },

    AudioEngine_set_callback__deps: ['__set_callback'],
    AudioEngine_set_finish_callback: function(audioId, callback)
    {
        ___set_callback(audioId, "finish", callback);
    },

    AudioEngine_play2d__deps: ['__set_callback'],
    AudioEngine_play2d: function(filenameP, loop, volume, callback)
    {
        var filename = Pointer_stringify(filenameP);
        if (Module.CocosAudioState.formats.length > 0)
            filename = filename.substr(0, filename.lastIndexOf('.'));
        var numAudio = Module.CocosAudioState.numAudioCount;

        if(Module.CocosAudioState.audioMap[filename] === undefined)
        {
            var sound = new buzz.sound(filename, {
                                                formats: Module.CocosAudioState.formats,
                                                preload: true,
                                                autoplay: true,
                                                loop: loop,
                                                volume: volume
                                                });
            Module.CocosAudioState.audioMap[filename] = sound;
            Module.CocosAudioState.audioMapNum[filename] = numAudio;
            Module.CocosAudioState.numAudioCount++;

            ___set_callback(numAudio, "play2d", callback);
            sound.bind("loadeddata", function ()
            {
                if (Module.CocosAudioState.play2dCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.play2dCallbacks[numAudio].call(this, numAudio, true);
            });

            sound.bind("error", function ()
            {
                if (Module.CocosAudioState.play2dCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.play2dCallbacks[numAudio].call(this, numAudio, false); 
            });

            sound.bind("ended", function ()
            {
                if (Module.CocosAudioState.finishCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.finishCallbacks[numAudio].call(this, numAudio, false); 
            });
        }
        else
        {
            numAudio = Module.CocosAudioState.audioMapNum[filename];
            var sound = Module.CocosAudioState.audioMap[filename];

            if (loop) 
            {
                sound.loop();
            }
            else
            {
                sound.unloop();
            }

            ___set_callback(numAudio, "play2d", callback);
            sound.bind("playing", function ()
            {
                if (Module.CocosAudioState.play2dCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.play2dCallbacks[numAudio].call(this, numAudio, true);
            });

            sound.bind("ended", function ()
            {
                if (Module.CocosAudioState.finishCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.finishCallbacks[numAudio].call(this, numAudio, false); 
            });

            sound.setVolume(volume);            
            sound.play();
        }
        return numAudio;
    },

    AudioEngine_preload: function(filenameP)
    {
        var filename = Pointer_stringify(filenameP);
        if( Module.CocosAudioState.formats.length > 0)
            filename = filename.substr(0, filename.lastIndexOf('.'));

        if(Module.CocosAudioState.audioMap[filename] === undefined)
        {
            var numAudio = Module.CocosAudioState.numAudioCount;

            var sound = new buzz.sound(filename, {
                                                formats: Module.CocosAudioState.formats,
                                                preload: true,
                                                autoplay: false,
                                                volume: 100
                                                });

            Module.CocosAudioState.audioMap[filename] = sound;
            Module.CocosAudioState.audioMapNum[filename] = numAudio;
            Module.CocosAudioState.numAudioCount++;

            sound.bind("loadeddata", function ()
            {
                if (Module.CocosAudioState.preloadCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.preloadCallbacks[numAudio].call(this, numAudio, true);
            });

            sound.bind("error", function ()
            {
                if (Module.CocosAudioState.preloadCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.preloadCallbacks[numAudio].call(this, numAudio, false); 
            });

            sound.bind("ended", function ()
            {
                if (Module.CocosAudioState.finishCallbacks[numAudio] != undefined)
                    Module.CocosAudioState.finishCallbacks[numAudio].call(this, numAudio, false); 
            });

            sound.load();
            return numAudio;
        }

        return -1;
    },
    
    AudioEngine_unload: function(filenameP)
    {
        var filename = Pointer_stringify(filenameP);
        if( Module.CocosAudioState.formats.length > 0)
            filename = filename.substr(0, filename.lastIndexOf('.'));
            
        var value = Module.CocosAudioState.audioMap[filename];
        if(value != undefined)
        {
            var audioId = Module.CocosAudioState.audioMapNum[filename];
            if (Module.CocosAudioState.preloadCallbacks[audioId] != undefined)
                Module.CocosAudioState.preloadCallbacks[audioId] = null;

            if (Module.CocosAudioState.play2dCallbacks[audioId] != undefined)
                Module.CocosAudioState.play2dCallbacks[audioId] = null;

            if (Module.CocosAudioState.finishCallbacks[audioId] != undefined)
                Module.CocosAudioState.finishCallbacks[audioId] = null;
            
            delete Module.CocosAudioState.audioMap[filename];
            delete Module.CocosAudioState.audioMapNum[filename];
        }
    },

    AudioEngine_stop: function(audioId)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                Module.CocosAudioState.audioMap[key].stop();
                if (Module.CocosAudioState.preloadCallbacks[audioId] != undefined)
                    Module.CocosAudioState.preloadCallbacks[audioId] = null;

                if (Module.CocosAudioState.play2dCallbacks[audioId] != undefined)
                    Module.CocosAudioState.play2dCallbacks[audioId] = null;

                if (Module.CocosAudioState.finishCallbacks[audioId] != undefined)
                    Module.CocosAudioState.finishCallbacks[audioId] = null;

                delete Module.CocosAudioState.audioMap[key];
                delete Module.CocosAudioState.audioMapNum[key];
            }
        }
    },

    AudioEngine_setVolume: function(audioId, volume)
    {
        if(volume > 100) volume = 100;
        if(volume < 0) volume = 0;

        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                Module.CocosAudioState.audioMap[key].setVolume(volume);
            }
        }
    },

    AudioEngine_getDuration: function(audioId)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                return Module.CocosAudioState.audioMap[key].getDuration();
            }
        }

        return 0;
    },

    AudioEngine_getCurrentTime: function(audioId)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                return Module.CocosAudioState.audioMap[key].getTime();
            }
        }
        
        return 0;
    },

    AudioEngine_setCurrentTime: function(audioId, time)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                Module.CocosAudioState.audioMap[key].setTime(time);
            }
        }
    },

    AudioEngine_setLoop: function(audioId, loop)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                if (loop)
                {
                    Module.CocosAudioState.audioMap[key].loop();
                }
                else
                {
                    Module.CocosAudioState.audioMap[key].unloop();
                }
            }
        }
    },

    AudioEngine_pause: function(audioId)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                Module.CocosAudioState.audioMap[key].pause();
            }
        }
    },

    AudioEngine_resume: function(audioId)
    {
        for(var key in Module.CocosAudioState.audioMapNum)
        {
            var value = Module.CocosAudioState.audioMapNum[key];

            if (value == audioId)
            {
                if (Module.CocosAudioState.audioMap[key].isPaused())
                {
                    Module.CocosAudioState.audioMap[key].togglePlay();
                }
            }
        }
    }
};

mergeInto(LibraryManager.library, LibraryAudioEngine);

