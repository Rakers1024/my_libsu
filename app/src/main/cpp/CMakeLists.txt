#声明要求的cmake最低版本
cmake_minimum_required(VERSION 3.4.1)
add_subdirectory(whale)
include_directories(whale/include)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99")
#（3）暴露ANativeActivity_onCreate方法，否则在创建activity时，会crash
set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -u ANativeActivity_onCreate")


#删除执行程序的符号表和重定位信息
#SET(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -s")
#SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -s")
#SET(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -s")
#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -s")
#add_compile_options(-fvisibility=hidden)
add_compile_options(-fvisibility-inlines-hidden)
#set(CMAKE_SHARED_LINKER_FLAGS  "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--version-script=${CMAKE_SOURCE_DIR}/export.map")
set(can_use_assembler TRUE)

#添加内联支持
enable_language(C ASM)

#引入syscall汇编
set(asm_syscall ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/syscall.s)

#find_package(bytehook REQUIRED CONFIG)
#bytehook::bytehook


include_directories(imgui)
include_directories(hide)
include_directories(xhook)
include_directories(include)
include_directories(memory)
include_directories(draw)

add_library(local_dobby STATIC IMPORTED)
set_target_properties(local_dobby PROPERTIES IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/lib/${ANDROID_ABI}/libdobby.a)




add_library( # Sets the name of the library.
        TCUpload

        # Sets the library as a shared library.
        SHARED
        # Provides a relative path to your source file(s).
        draw/imgui_draw_main.cpp
        draw/Draw.cpp
        main-injection.cpp
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

        #Substrate
        #Substrate/hde64.c
        #Substrate/SubstrateDebug.cpp
        #Substrate/SubstrateHook.cpp
        #Substrate/SubstratePosixMemory.cpp
        #Substrate/And64InlineHook.cpp

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
        ${asm_syscall}
        )

target_link_libraries(TCUpload
        GLESv3
        android
        local_dobby
        #whale
        EGL
        log
        z
        )


add_library(CRC32Edit SHARED CRC32Edit.cpp)
find_library(log-lib log)
target_link_libraries(CRC32Edit ${log-lib})
#add_library(bypass SHARED bypass.cpp include/byopen/byopen_android.c include/Substrate/hde64.c include/Substrate/SubstrateDebug.cpp include/Substrate/SubstrateHook.cpp include/Substrate/SubstratePosixMemory.cpp include/Substrate/And64InlineHook.cpp)
#find_library(log-lib log )
#target_link_libraries(bypass ${log-lib})

#-bcf 伪造控制流
#-sub 算数指令替换
# -split 指令块分割
#-fla 控制流平坦化
# -mllvm -sobf 字符串加密


#控制流扁平化 #字符串 算数指令替换
#SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE}${CMAKE_CXX_FLAGS_DEBUG} -mllvm -fla -mllvm -sobf -mllvm -sub")
SET(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} ${CMAKE_CXX_FLAGS_DEBUG} -mllvm -sub  -mllvm -sobf ")
