#声明要求的cmake最低版本
cmake_minimum_required(VERSION 3.4.1)
add_subdirectory(whale)
include_directories(whale/include)
add_compile_options(-fvisibility-inlines-hidden)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
#set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_SOURCE_DIR}/export.map")


add_definitions(-DRIRU_MODULE)
add_definitions(-DRIRU_MODULE_API_VERSION=${RIRU_MODULE_API_VERSION})
add_definitions(-DRIRU_MODULE_VERSION=${RIRU_MODULE_VERSION})
add_definitions(-DRIRU_MODULE_VERSION_NAME=${RIRU_MODULE_VERSION_NAME})
add_definitions(-DRIRU_MODULE_MIN_API_VERSION=${RIRU_MODULE_MIN_API_VERSION})
find_package(riru REQUIRED CONFIG)

enable_language(C ASM)

set(asm_syscall ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/syscall.s)


include_directories(imgui)
include_directories(hide)
include_directories(xhook)
include_directories(include)
include_directories(memory)
include_directories(draw)
add_library(local_dobby STATIC IMPORTED)
set_target_properties(local_dobby PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libdobby.a)

add_library(
        ${MODULE_NAME}
        SHARED
        MainEntry.cpp
        draw/Draw.cpp
        draw/imgui_draw_main.cpp
        memory/MemoryUtils.cpp
        memory/GameUtils.cpp
        memory/fake_dlfcn.cpp

        imgui/imgui.cpp
        imgui/imgui_demo.cpp
        imgui/imgui_draw.cpp
        imgui/imgui_tables.cpp
        imgui/imgui_widgets.cpp
        imgui/stb_image.cpp
        imgui/backends/imgui_impl_android.cpp
        imgui/backends/imgui_impl_opengl3.cpp

        #xhook
        #xhook/xh_core.c
        #xhook/xh_elf.c
        #xhook/xh_jni.c
        #xhook/xh_log.c
        #xhook/xh_version.c
        #xhook/xhook.c
        #xhook/xh_util.c

        hide/MapsRedirector.cpp
        hide/IORelocator.cpp

        #Substrate
        #Substrate/hde64.c
        #Substrate/SubstrateDebug.cpp
        #Substrate/SubstrateHook.cpp
        #Substrate/SubstratePosixMemory.cpp
        #Substrate/And64InlineHook.cpp
        ${asm_syscall}
)

target_link_libraries(${MODULE_NAME}
        GLESv3
        android
        local_dobby
        EGL
        log
        z
        riru::riru
        #whale
        )
set_target_properties(${MODULE_NAME} PROPERTIES LINK_FLAGS_RELEASE -s)

#-bcf 伪造控制流
#-sub 算数指令替换
# -split 指令块分割
#-fla 控制流平坦化
# -mllvm -sobf 字符串加密
#控制流扁平化 #字符串 算数指令替换 sobf报错
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG} -mllvm -sobf -mllvm -sub")