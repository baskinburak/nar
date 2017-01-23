cmake_minimum_required(VERSION 2.8.3)
project(narnode)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_STANDARD 11)
include_directories(.)
link_directories(build)

find_library(CryptoPP REQUIRED)
find_library(Z REQUIRED)
find_library(MYSQLCPPCONN REQUIRED)

file(GLOB IPC_SOURCES nar/narnode/IPC/*cpp)
add_library(naripc SHARED ${IPC_SOURCES})

file(GLOB FILE_SOURCES nar/narnode/FileKeeper/*cpp)
add_library(narfile SHARED ${FILE_SOURCES})

file(GLOB SOCKET_SOURCES nar/lib/Socket/*cpp)
add_library(narsocket SHARED ${SOCKET_SOURCES})


set(DAEMON_SOURCES
    nar/narnode/Task/ITask.cpp
    nar/narnode/Task/LS.cpp
    nar/narnode/Task/PullFile.cpp
    nar/narnode/Task/PushFile.cpp
    nar/narnode/Task/Register.cpp
    nar/narnode/daemon.cpp
    nar/narnode/global.cpp
    nar/narnode/utility.cpp)

set(CLI_SOURCES
    nar/narnode/cli.cpp
    nar/narnode/utility.cpp
    )

set(SERVER_SOURCES
    nar/narserver/Database.cpp    
    nar/narserver/server.cpp
    nar/narserver/sockinfo.cpp
    nar/narnode/utility.cpp
  )

set(NAR_LIBRARIES libnaripc.so libnarfile.so libnarsocket.so pthread cryptopp z)
add_executable(nardaemon ${DAEMON_SOURCES})
target_link_libraries(nardaemon ${NAR_LIBRARIES})
add_executable(narcli ${CLI_SOURCES})
target_link_libraries(narcli ${NAR_LIBRARIES})
add_executable(narserver ${SERVER_SOURCES})
target_link_libraries(narserver ${NAR_LIBRARIES} mysqlcppconn)

add_dependencies(nardaemon naripc narfile narsocket)
add_dependencies(narcli naripc narfile narsocket)
add_dependencies(narserver narsocket naripc narfile)
