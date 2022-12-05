set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)
project(sdl2to3 LANGUAGES C CXX)

find_package(LLVM 15.0 REQUIRED CONFIG)
find_package(Clang REQUIRED CONFIG)

include("${LLVM_CMAKE_DIR}/AddLLVM.cmake")
include("${CLANG_CMAKE_DIR}/AddClang.cmake")

add_library(sdl2to3_plugin MODULE
    src/plugin.cpp

    src/includes/SDL3SubfolderIncludeCheck.cpp
    src/deprecated/SDL3RemovedFunctionsCheck.cpp
    src/deprecated/SDL3RemovedSDL_CreateRGBSurfaceCheck.cpp
    src/deprecated/SDL3RemovedSDL_CreateRGBSurfaceWithFormatCheck.cpp
    src/deprecated/SDL3RemovedSDL_CreateRGBSurfaceFromCheck.cpp
    src/deprecated/SDL3RemovedSDL_CreateRGBSurfaceWithFormatFromCheck.cpp
)
clang_target_link_libraries(sdl2to3_plugin
    PRIVATE
        clangAST
        clangBasic
        clangDriver
        clangFrontend
        clangRewriteFrontend
        clangSerialization
        clangStaticAnalyzerFrontend
        clangTooling
        clangToolingSyntax
)
target_link_libraries(sdl2to3_plugin PRIVATE LLVM)
set_target_properties(sdl2to3_plugin PROPERTIES CXX_STANDARD 20)
set_target_properties(sdl2to3_plugin PROPERTIES PREFIX "")
set_target_properties(sdl2to3_plugin PROPERTIES OUTPUT_NAME "sdl2to3")
target_compile_options(sdl2to3_plugin PRIVATE -fno-rtti)
