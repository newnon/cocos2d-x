var LibraryWebSocket = {
    WebSocket_init: function(address)
    {
        Module['websocket'] = (Module['websocket'] && 
                             ('object' === typeof Module['websocket'])) ? Module['websocket'] : {};
        Module['websocket']._callbacks = {};
        Module['websocket']['on'] = function(event, callback)
        {
            if ('function' === typeof callback)
            {
                this._callbacks[event] = callback;
            }

            return this;
        };

        Module['websocket'].emit = function(event, data)
        {
            if ('function' === typeof this._callbacks[event])
            {
                this._callbacks[event].call(this, data);
            }
        };

        var serverAddress = Pointer_stringify(address);
        socket = new WebSocket(serverAddress);
        socket.binaryType = "arraybuffer";

        socket.onopen = function()
        {
            Module['websocket'].emit('open', [null, 0]);
        };

        socket.onmessage = function(event)
        {
            if (typeof event.data === "string")
            {
                var msg = allocate(intArrayFromString(event.data), 'i8', ALLOC_STACK);
                Module['websocket'].emit('message', [msg, event.data.length]);
            }
            else if (event.data instanceof ArrayBuffer)
            {
                var byteArray = new Uint8Array(event.data);
                var buffer = _malloc(byteArray.length);
                HEAPU8.set(byteArray, buffer);
                Module['websocket'].emit('message', [buffer, event.data.byteLength]);
            }
        };

        socket.onclose = function(event)
        {
            var msg = event.reason;
            Module['websocket'].emit('close', [event.code, msg, msg.length]);
        };
        
        socket.onerror = function(error)
        {
            var msg = error.message;
            Module['websocket'].emit('error', [error.code, msg, msg.length]);
        };
    },
    WebSocket_close: function()
    {
        socket.close();
    },
    WebSocket_send: function(data)
    {
        var sendData = Pointer_stringify(data);
        socket.send(sendData);
    },
    WebSocket_send_data: function(data, length)
    {
        var binary = new Uint8Array(length);
        for (var i = 0; i < length; i++)
        {
            binary[i] = getValue(data + i, 'i8');
        }

        socket.send(binary);
    },
    __set_network_callback: function(event, userData, callback)
    {
        function _callback(data)
        {
            try
            {
                if (event === 'error')
                {
                    var sp = Runtime.stackSave();
                    var msg = allocate(intArrayFromString(data[2]), 'i8', ALLOC_STACK);
                    Runtime.dynCall('viiii', callback, [data[0], data[1], msg, userData]);
                    Runtime.stackRestore(sp);
                }
                else
                {
                    var sp = Runtime.stackSave();
                    Runtime.dynCall('viii', callback, [userData, data[0], data[1]]);
                    Runtime.stackRestore(sp);
                }
            }
            catch (e)
            {
                if (e instanceof ExitStatus)
                {
                    return;
                }
                else
                {
                    if (e && typeof e === 'object' && e.stack) Module.printErr('exception thrown: ' + [e, e.stack]);
                    throw e;
                }
            }
        };

        Module['noExitRuntime'] = true;
        Module['websocket']['on'](event, callback ? _callback : null);
    },
    WebSocket_set_socket_error_callback__deps: ['__set_network_callback'],
    WebSocket_set_socket_error_callback: function(userData, callback)
    {
        ___set_network_callback('error', userData, callback);
    },
    WebSocket_set_socket_open_callback__deps: ['__set_network_callback'],
    WebSocket_set_socket_open_callback: function(userData, callback)
    {
        ___set_network_callback('open', userData, callback);
    },
    WebSocket_set_socket_listen_callback__deps: ['__set_network_callback'],
    WebSocket_set_socket_listen_callback: function(userData, callback)
    {
        ___set_network_callback('listen', userData, callback);
    },
    WebSocket_set_socket_connection_callback__deps: ['__set_network_callback'],
    WebSocket_set_socket_connection_callback: function(userData, callback)
    {
        ___set_network_callback('connection', userData, callback);
    },
    WebSocket_set_socket_message_callback__deps: ['__set_network_callback'],
    WebSocket_set_socket_message_callback: function(userData, callback)
    {
        ___set_network_callback('message', userData, callback);
    },
    WebSocket_set_socket_close_callback__deps: ['__set_network_callback'],
    WebSocket_set_socket_close_callback: function(userData, callback)
    {
        ___set_network_callback('close', userData, callback);
    }
};

mergeInto(LibraryManager.library, LibraryWebSocket);

