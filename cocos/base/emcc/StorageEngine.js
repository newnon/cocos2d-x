var LibraryStorageEngine = {    
    StorageEngine_save: function(name, data)
    {
        var strkey = Pointer_stringify(name);
        var strdata = Pointer_stringify(data);
        localStorage.setItem(strkey, strdata);
    },

    StorageEngine_load: function(name)
    {
        var strkey = Pointer_stringify(name);
        var pname = localStorage.getItem(strkey);

        if (pname)
        {
            var ptr = allocate(intArrayFromString(pname), 'i8', ALLOC_STACK);
            return ptr;
        }

        return null;
    },

    StorageEngine_remove: function(name)
    {
        var strkey = Pointer_stringify(name);
        localStorage.removeItem(strkey);
    }
};

mergeInto(LibraryManager.library, LibraryStorageEngine);

