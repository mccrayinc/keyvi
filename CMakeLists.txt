cmake_minimum_required(VERSION 2.8)
project(keyvi)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -msse4.2")

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -ggdb3")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3 -DNDEBUG")
set(CMAKE_CXX_FLAGS_COVERAGE "${CMAKE_CXX_FLAGS_DEBUG} -O0 --coverage")

set(COMPILE_TEST OFF CACHE BOOL "")
set(TPIE_PARALLEL_SORT 1 CACHE BOOL "")
add_subdirectory(keyvi/3rdparty/tpie)


set(Boost_USE_STATIC_LIBS OFF)
set(Boost_USE_MULTITHREADED ON)
set(Boost_USE_STATIC_RUNTIME OFF)
find_package(Boost REQUIRED COMPONENTS "program_options" "iostreams" "filesystem" "system" "regex" "thread" "unit_test_framework")
if (Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Can not find Boost")
endif ()


find_package(ZLIB REQUIRED)
if (ZLIB_FOUND)
    include_directories(${ZLIB_INCLUDE_DIRS})
else ()
    message(FATAL_ERROR "Can not fing ZLib")
endif (ZLIB_FOUND)


if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    add_definitions(-DOS_MACOSX)
endif ()


include_directories(keyvi/include/keyvi)
include_directories(keyvi/3rdparty/tpie)
include_directories(keyvi/3rdparty/utf8)
include_directories(keyvi/3rdparty/misc)
include_directories(keyvi/3rdparty/rapidjson/include)
include_directories(keyvi/3rdparty/msgpack-c/include)
include_directories(keyvi/3rdparty/xchange/src)
include_directories(${CMAKE_BINARY_DIR}/keyvi/3rdparty/tpie)


FILE(GLOB_RECURSE UNIT_TEST_SOURCES RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} keyvi/tests/keyvi/*.cpp)

add_executable(keyvicompiler keyvi/bin/keyvicompiler/keyvicompiler.cpp)
add_executable(keyviinspector keyvi/bin/keyviinspector/keyviinspector.cpp)
add_executable(keyvimerger keyvi/bin/keyvimerger/keyvimerger.cpp)
add_executable(unit_test_all ${UNIT_TEST_SOURCES})

target_link_libraries(keyvicompiler tpie ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})
target_link_libraries(keyviinspector tpie ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})
target_link_libraries(keyvimerger tpie ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})
target_link_libraries(unit_test_all tpie ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})
