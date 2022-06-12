cmake_minimum_required(VERSION 3.21)
project(SearchEngine)
find_package(Threads REQUIRED)
set(THREADS_PREFER_PTHREAD_FLAG ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")
add_subdirectory(nlohmann_json)
set(SOURCE_FILES main.cpp src/converterjson.cpp src/invertedindex.cpp src/searchserver.cpp)
add_executable(SearchEngine ${SOURCE_FILES})
target_include_directories(SearchEngine PUBLIC include)
set(CMAKE_CXX_STANDARD 14)

set(Boost_INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}\\boost_X_XX_X")
set(Boost_LIBRARY_DIR "${CMAKE_CURRENT_SOURCE_DIR}\\boost_X_XX_X\\stage\\lib")
find_package(Boost COMPONENTS REQUIRED)
include_directories(${Boost_INCLUDE_DIR})
link_directories(${Boost_LIBRARY_DIR})

target_link_libraries(SearchEngine PRIVATE nlohmann_json::nlohmann_json Threads::Threads ${Boost_LIBRARIES} ) # ws2_32)


project(SearchEngineTest)
include(FetchContent)
FetchContent_Declare(
        googletest
        URL
        https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
enable_testing()
set(SOURCE_FILES_TES main_tests.cpp src/converterjson.cpp src/invertedindex.cpp src/searchserver.cpp)
add_executable(SearchEngineTest ${SOURCE_FILES_TES})
target_link_libraries(SearchEngineTest PRIVATE nlohmann_json::nlohmann_json gtest_main Threads::Threads ${Boost_LIBRARIES} ) # ws2_32)
target_include_directories(SearchEngineTest PUBLIC include)
include(GoogleTest)
gtest_discover_tests(SearchEngineTest)
