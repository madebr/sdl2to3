set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
cmake_minimum_required(VERSION 3.16)
project(sdl2to3 LANGUAGES C CXX)

find_package(Clang REQUIRED CONFIG)
find_package(LLVM REQUIRED CONFIG)

include("${LLVM_CMAKE_DIR}/AddLLVM.cmake")
include("${CLANG_CMAKE_DIR}/AddClang.cmake")

add_executable(sdl2to3
    main.cpp
)
clang_target_link_libraries(sdl2to3
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
target_link_libraries(sdl2to3 PRIVATE LLVM)
set_target_properties(sdl2to3 PROPERTIES CXX_STANDARD 20)
target_compile_options(sdl2to3 PRIVATE -fno-rtti)
