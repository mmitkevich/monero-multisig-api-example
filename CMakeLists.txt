if(IOS)
    INCLUDE(CMakeLists_IOS.txt)
endif()

cmake_minimum_required(VERSION 2.8.7)

project(monero-multisig-api-example)

set(SRC ExaWallet.cpp main.cpp)

set(ARCH native CACHE STRING "CPU to build for: -march value or 'default' to not pass -march at all")
message(STATUS "Building on ${CMAKE_SYSTEM_PROCESSOR} for ${ARCH}")
if(ARCH STREQUAL "default")
    set(ARCH_FLAG "")
elseif(IOS AND ARCH STREQUAL "arm64")
    message(STATUS "IOS: Changing arch from arm64 to armv8")
    set(ARCH_FLAG "-march=armv8")
elseif(IOS AND ARCH STREQUAL "x86_64")
    set(ARCH_FLAG "")
else()
    set(ARCH_FLAG "-march=${ARCH}")
endif()
set(WARNINGS "-Wall -Wextra -Wpointer-arith -Wundef -Wvla -Wwrite-strings -Wno-error=extra -Wno-error=deprecated-declarations -Wno-unused-parameter -Wno-unused-variable -Wno-error=unused-variable -Wno-error=undef -Wno-error=uninitialized -Wno-user-defined-warnings")
if(CMAKE_C_COMPILER_ID STREQUAL "Clang")
if(ARM)
    set(WARNINGS "${WARNINGS} -Wno-error=inline-asm")
endif()
else()
    set(WARNINGS "${WARNINGS} -Wlogical-op -Wno-error=maybe-uninitialized -Wno-error=cpp")
endif()

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 ${WARNINGS} ${ARCH_FLAG}")
#set(CMAKE_OSX_SYSROOT "/Applications/Xcode.app/Contents/Developer/Platforms/iPhoneSimulator.platform/Developer/SDKs/iPhoneSimulator11.4.sdk")

#find_library(PCSC PCSC)
#find_library(IOKit IOKit)

include_directories(${MONERO_INCLUDEDIR} ${BOOST_INCLUDEDIR} ${OPENSSL_INCLUDEDIR})

set(libs
    wallet_merged
    epee
    easylogging
    lmdb
    boost_serialization
    boost_thread
    boost_system
    boost_date_time
    boost_filesystem
    boost_chrono
    boost_program_options
    boost_regex
    unbound
    ssl
    crypto
    z)

link_directories(${MONERO_LIBRARYDIR} ${OPENSSL_LIBRARYDIR} ${BOOST_LIBRARYDIR})

add_executable(monero-multisig-api-example ${SRC})
target_link_libraries(monero-multisig-api-example ${libs} ${PCSC} ${IOKit})
