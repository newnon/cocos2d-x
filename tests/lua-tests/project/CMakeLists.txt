set(APP_NAME lua-tests)

set(SAMPLE_SRC
    Classes/AppDelegate.cpp
    Classes/lua_assetsmanager_test_sample.cpp
    Classes/lua_test_bindings.cpp
)

if(LINUX)
    set(SAMPLE_SRC ${SAMPLE_SRC} proj.linux/main.cpp)
    set(RES_PREFIX "/Resources")
elseif(WINDOWS)
    list(APPEND SAMPLE_SRC proj.win32/main.cpp)
    set(RES_PREFIX "")
elseif(MACOSX)
    set(SAMPLE_SRC ${SAMPLE_SRC} proj.ios_mac/mac/main.cpp)

    file(GLOB_RECURSE APP_RESOURCES ../res/*)
    cocos_mark_resources(FILES ${APP_RESOURCES} BASEDIR ../res RESOURCEBASE Resources/res)
    file(GLOB_RECURSE APP_SCRIPTS ../src/*)
    cocos_mark_resources(FILES ${APP_SCRIPTS} BASEDIR ../src RESOURCEBASE Resources/src)
    file(GLOB_RECURSE COCOS_SCRIPTS ${CMAKE_SOURCE_DIR}/cocos/scripting/lua-bindings/script/*)
    cocos_mark_resources(FILES ${COCOS_SCRIPTS} BASEDIR ${CMAKE_SOURCE_DIR}/cocos/scripting/lua-bindings/script RESOURCEBASE Resources/src/cocos)
    file(GLOB_RECURSE TEST_RESOURCES ${CMAKE_SOURCE_DIR}/tests/cpp-tests/Resources/*)
    cocos_mark_resources(FILES ${TEST_RESOURCES} BASEDIR ${CMAKE_SOURCE_DIR}/tests/cpp-tests/Resources RESOURCEBASE Resources/res)

    list(APPEND SAMPLE_SRC ${APP_RESOURCES} ${APP_SCRIPTS} ${COCOS_SCRIPTS} ${TEST_RESOURCES})
endif()

include_directories(
  Classes
  ../../../cocos/scripting/lua-bindings/manual
  ../../../cocos/scripting/lua-bindings/auto
  ../../../external/lua/lua
  ../../../external/lua/tolua
)

# add the executable
add_executable(${APP_NAME} ${SAMPLE_SRC})

target_link_libraries(${APP_NAME} luacocos2d cocos2d)

if(MACOSX OR APPLE)
  set_target_properties(${APP_NAME} PROPERTIES
      MACOSX_BUNDLE 1
      RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
      )

else()

    set(APP_BIN_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}")

    set_target_properties(${APP_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${APP_BIN_DIR}")

    if(EMSCRIPTEN)
        set(CMAKE_EXECUTABLE_SUFFIX ".html")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -s TOTAL_MEMORY=268435456")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../res@/res")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_CURRENT_SOURCE_DIR}/../src@/src")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/cocos/scripting/lua-bindings/script@/Resources/src/cocos")
        set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} --preload-file ${CMAKE_SOURCE_DIR}/tests/cpp-tests/Resources@/res")
        file(GLOB preJsFiles ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos/audio/emcc/buzz.js)
        file(GLOB libraryJsFiles 
          ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos/audio/emcc/SimpleAudioEngine.js
          ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos/base/emcc/StorageEngine.js
          ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos/network/WebSocket.js
          ${CMAKE_CURRENT_SOURCE_DIR}/../../cocos/network/HttpClient.js
          )
    
    em_link_js_library(${APP_NAME} ${libraryJsFiles})
    em_link_pre_js(${APP_NAME} ${preJsFiles})
    else()
		pre_build(${APP_NAME}
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../res ${APP_BIN_DIR}${RES_PREFIX}/res
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/../src ${APP_BIN_DIR}${RES_PREFIX}/src
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/cocos/scripting/lua-bindings/script/ ${APP_BIN_DIR}/Resources/src/cocos
		  COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/tests/cpp-tests/Resources ${APP_BIN_DIR}${RES_PREFIX}/res
		  )
    endif()

endif()

