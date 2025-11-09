cmake_minimum_required(VERSION 3.26)

project(
  Strategy1.Analyzer
  VERSION 0.1
  DESCRIPTION "Strategy1.Analyzer"
  LANGUAGES CXX)

set(CMAKE_C_COMPILER /opt/rh/gcc-toolset-13/root/usr/bin/gcc)    # C 컴파일러
set(CMAKE_CXX_COMPILER /opt/rh/gcc-toolset-13/root/usr/bin/g++)   # C++ 컴파일러

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_CLANG_TIDY clang-tidy --extra-arg=--gcc-toolchain=/opt/rh/gcc-toolset-13/root/usr)
# set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -save-temps=obj")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O3 -g -DNDEBUG -save-temps=obj")

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -save-temps")

set(CorePath "../Strategy1.Core")

include(${CMAKE_CURRENT_SOURCE_DIR}/IncludeRecursiveHeaders.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/IncludeRecursiveSources.cmake)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

add_custom_target(copy_compile_commands ALL
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    ${CMAKE_BINARY_DIR}/compile_commands.json
    ${CMAKE_SOURCE_DIR}/compile_commands.json
    DEPENDS ${CMAKE_BINARY_DIR}/compile_commands.json
)

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_options(${PROJECT_NAME} PRIVATE -static-libstdc++ -static-libgcc)
target_link_libraries(${PROJECT_NAME} PRIVATE rt)
target_link_libraries(${PROJECT_NAME} PRIVATE pthread)
target_include_directories(${PROJECT_NAME} PRIVATE ${CorePath})

# target_link_libraries(${PROJECT_NAME} PRIVATE /usr/lib64/libpcap.so)
