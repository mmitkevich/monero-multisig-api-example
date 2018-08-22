cmake_minimum_required(VERSION 2.8.7)
set(Boost_USE_MULTITHREADED ON)
project(app-monero)

option(ANDROID "Build for android")
option(IOS "Build for ios")
option(ARCH "arm, arm64, x86_64")

if (IOS)
    INCLUDE(CMakeLists_IOS.txt)
    set(CMAKE_OSX_SYSROOT "${CMAKE_IOS_SDK_ROOT}")
    message("${CMAKE_OSX_SYSROOT}")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(MONERO_ROOT ${PROJECT_SOURCE_DIR}/../monero)

set(SRC  main.cpp)

include_directories(${MONERO_ROOT})
include_directories(${MONERO_ROOT}/external/easylogging++)

set(Boost_USE_STATIC_LIBS ON)
#set(Boost_USE_STATIC_RUNTIME ON)

find_package(Boost 1.58 REQUIRED COMPONENTS system filesystem thread date_time chrono regex serialization program_options locale)

link_directories(./deps)

set(libs
    wallet_merged
    epee
    easylogging
    lmdb
    unbound
    ssl
    crypto
    z)

if (ANDROID)    
    link_directories(
        /opt/android/monero/armeabi-v7a/
        /opt/android/lib/armeabi-v7a/
        /opt/android/lib/armeabi-v7a/boost
    )
elseif(IOS)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -I ${BOOST_ROOT}")
    set(CMAKE_LD_FLAGS "${CMAKE_LD_FLAGS} -v")
    link_directories(${PROJECT_SOURCE_DIR}/../monero-libs/ios-arm64)
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread") #  -fsanitize=thread
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread")

    set(libs ${libs} dl ) #device pcsclite
    find_library(PCSC PCSC)
    find_library(IOKit IOKit)
    link_directories(
        #/usr/lib/x86_64-linux-gnu/
        /usr/local/Cellar/openssl/1.0.2n/lib/
        ${MONERO_ROOT}/build/lib
        ${DEPS_ROOT}
    )
endif()

add_executable(app-monero ${SRC})
target_link_libraries(app-monero ${libs} ${PCSC} ${IOKit} ${Boost_LIBRARIES})
