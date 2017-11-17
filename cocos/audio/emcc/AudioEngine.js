var LibraryAudioEngine = {
    AudioEngine_init: function(playCallback, endCallback, preloadCallback) {
        Module.CocosAudioState = {
            nameToSoundMap: {},
            idToSoundMap: {},
            formats: [],
            preloadCallback: preloadCallback,
            playCallback: playCallback,
            endCallback: endCallback
        };
    },

    AudioEngine_setUseFileExt: function(extFiles) {
        var exts = Pointer_stringify(extFiles);
        Module.CocosAudioState.formats = exts.split(',');
    },

    AudioEngine_end: function() {
        for(var key in Module.CocosAudioState.nameToSoundMap) {
            if (Module.CocosAudioState.nameToSoundMap.hasOwnProperty(key)) {    
                var value = Module.CocosAudioState.nameToSoundMap[key];
                value.unload();
            }
        }
        Module.CocosAudioState.nameToSoundMap = {};
    },

    __createHowl: function(filename) {
        var fileNames = []

        if (Module.CocosAudioState.formats.length > 0) {
            var short_filename = filename.substr(0, filename.lastIndexOf('.'));
            for(var i in Module.CocosAudioState.formats) {
                fileNames.push(short_filename + '.' + Module.CocosAudioState.formats[i]);
            }
        }
        else {
            fileNames.push(filename);
        }

        var sound = new Howl({
            src: fileNames,
            preload: true,
            onload: function() {
                console.log('onload ' + filename);
                var len = lengthBytesUTF8(filename);
                var bufferSize = len+1;
                var buffer = _malloc(bufferSize);
                stringToUTF8(filename, buffer, bufferSize);
                var sp = Runtime.stackSave();
                Runtime.dynCall('vii', Module.CocosAudioState.preloadCallback, [buffer, 1]);
                Runtime.stackRestore(sp);
                console.log('onload1 ' + filename);
                _free(buffer);
                console.log('onload2 ' + filename);
            },
            onloaderror: function(i) {
                console.log('onloaderror ' + filename);
                var len = lengthBytesUTF8(filename);
                var bufferSize = len+1;
                var buffer = _malloc(bufferSize);
                stringToUTF8(filename, buffer, bufferSize);
                var sp = Runtime.stackSave();
                Runtime.dynCall('vii', Module.CocosAudioState.preloadCallback, [buffer, 0]);
                Runtime.stackRestore(sp);
                console.log('onloaderror1 ' + filename);
                _free(buffer);
                console.log('onloaderror2 ' + filename);
            },
            onplay: function(id) {
                console.log('onplay');
                var sp = Runtime.stackSave();
                Runtime.dynCall('vii', Module.CocosAudioState.playCallback, [id, 1]);
                Runtime.stackRestore(sp);
            },
            onplayerror: function(id) {
                console.log('onplayerror');
                var sp = Runtime.stackSave();
                Runtime.dynCall('vii', Module.CocosAudioState.playCallback, [id, 0]);
                Runtime.stackRestore(sp);
            },
            onend: function(id) {
                console.log('onend');
                var sound = Module.CocosAudioState.idToSoundMap[id];

                if(sound !== undefined) {
                    if(!sound.loop(id)) {
                        delete Module.CocosAudioState.idToSoundMap[id];
                        var sp = Runtime.stackSave();
                        Runtime.dynCall('vi', Module.CocosAudioState.endCallback, [id]);
                        Runtime.stackRestore(sp);
                    }
                } 
            }
        });
        return sound;
    },

    AudioEngine_play2d__deps: ['__createHowl'],
    AudioEngine_play2d: function(filenameP, loop, volume)
    {
        var filename = Pointer_stringify(filenameP);
        if (filename.length <= 0)
            return -1;

        console.log('AudioEngine_play2d ' + filename + ' ' + loop + ' ' + volume);

        var sound = Module.CocosAudioState.nameToSoundMap[filename];

        if(sound === undefined)
        {
            sound = ___createHowl(filename);
            Module.CocosAudioState.nameToSoundMap[filename] = sound;
        }

        var id = sound.play();
        Module.CocosAudioState.idToSoundMap[id] = sound;
        sound.loop(loop, id);
        sound.volume(volume, id);
        return id;
    },

    AudioEngine_preload__deps: ['__createHowl'],
    AudioEngine_preload: function(filenameP)
    {
        var filename = Pointer_stringify(filenameP);
        if (filename.length <= 0)
            return ;

        console.log('AudioEngine_preload ' + filename);

        var sound = Module.CocosAudioState.nameToSoundMap[filename];

        if(sound === undefined) {
            sound = ___createHowl(filename);
            Module.CocosAudioState.nameToSoundMap[filename] = sound;
        } 
    },
    
    AudioEngine_unload: function(filenameP)
    {
        var filename = Pointer_stringify(filenameP);
        if (filename.length <= 0)
            return ;

        console.log('AudioEngine_unload ' + filename);

        var sound = Module.CocosAudioState.nameToSoundMap[filename];

        if(sound !== undefined) {

            for(var key in Module.CocosAudioState.idToSoundMap) {
                if (Module.CocosAudioState.idToSoundMap.hasOwnProperty(key)) {    
                    if(Module.CocosAudioState.idToSoundMap[key] === sound)
                        delete Module.CocosAudioState.idToSoundMap[key];
                }
            }

            sound.unload();
            delete Module.CocosAudioState.nameToSoundMap[filename];
        } 
    },

    AudioEngine_stop: function(audioId)
    {
        console.log('AudioEngine_stop ' + audioId);

        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            sound.stop();
            delete Module.CocosAudioState.idToSoundMap[audioId];
        } 
    },

    AudioEngine_setVolume: function(audioId, volume)
    {
        console.log('AudioEngine_setVolume ' + audioId + ' ' + volume);

        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            sound.volume(volume, audioId);
        } 
    },

    AudioEngine_getDuration: function(audioId)
    {
        console.log('AudioEngine_getDuration ' + audioId);
        
        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            return sound.duration();
        } 

        return 0;
    },

    AudioEngine_getCurrentTime: function(audioId)
    {
        console.log('AudioEngine_getCurrentTime ' + audioId);
        
        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            return sound.seek(audioId);
        } 
        
        return 0;
    },

    AudioEngine_setCurrentTime: function(audioId, time)
    {
        console.log('AudioEngine_setCurrentTime ' + audioId + ' ' + time);
        
        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            sound.seek(time, audioId);
        } 
    },

    AudioEngine_setLoop: function(audioId, loop)
    {
        console.log('AudioEngine_setLoop ' + audioId + ' ' + loop);
        
        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            sound.loop(loop, audioId);
        } 
    },

    AudioEngine_pause: function(audioId)
    {
        console.log('AudioEngine_pause ' + audioId);
        
        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            sound.pause(audioId);
        } 
    },

    AudioEngine_resume: function(audioId)
    {
        console.log('AudioEngine_resume ' + audioId);
        
        var sound = Module.CocosAudioState.idToSoundMap[audioId];

        if(sound !== undefined) {
            sound.play(audioId);
        } 
    }
};

mergeInto(LibraryManager.library, LibraryAudioEngine);

