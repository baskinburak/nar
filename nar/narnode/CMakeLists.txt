cmake_minimum_required(VERSION 2.8.3)
project(narnode)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_STANDARD 11)
include_directories(../../)
link_directories(build)

find_library(CryptoPP REQUIRED)
find_library(Z REQUIRED)

file(GLOB IPC_SOURCES IPC/*)
add_library(naripc SHARED ${IPC_SOURCES})

file(GLOB FILE_SOURCES FileKeeper/*)
add_library(narfile SHARED ${FILE_SOURCES})


set(DAEMON_SOURCES
    ../../nar/lib/Socket/Socket.cpp
    ../../nar/lib/USocket/Packet.cpp
    ../../nar/lib/USocket/USocket.cpp
    ../../nar/narnode/Task/ITask.cpp
    ../../nar/narnode/Task/LS.cpp
    ../../nar/narnode/Task/PullFile.cpp
    ../../nar/narnode/Task/PushFile.cpp
    ../../nar/narnode/Task/Register.cpp
    ../../nar/narnode/daemon.cpp
    ../../nar/narnode/global.cpp
    ../../nar/narnode/utility.cpp)

set(CLI_SOURCES
    ../../nar/lib/USocket/Packet.cpp
    ../../nar/lib/USocket/USocket.cpp
    ../../nar/narnode/cli.cpp
    ../../nar/narnode/utility.cpp
    ../../nar/lib/Socket/Socket.cpp
    )

add_executable(nardaemon ${DAEMON_SOURCES})
target_link_libraries(nardaemon libnaripc.so libnarfile.so pthread cryptopp z)
add_executable(narcli ${CLI_SOURCES})
target_link_libraries(narcli libnaripc.so libnarfile.so cryptopp pthread z)

add_dependencies(nardaemon naripc narfile)
add_dependencies(narcli naripc narfile)
