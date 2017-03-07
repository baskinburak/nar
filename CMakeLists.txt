cmake_minimum_required(VERSION 2.8.3)
project(narnode)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_STANDARD 11)
include_directories(.)
link_directories(build)

find_library(CryptoPP REQUIRED)
find_library(Z REQUIRED)
find_library(MYSQLCPPCONN REQUIRED)

file(GLOB FILE_SOURCES nar/narnode/FileKeeper/*cpp)
add_library(narfile SHARED ${FILE_SOURCES})

file(GLOB SOCKET_SOURCES nar/lib/Socket/*cpp)
add_library(narsocket SHARED ${SOCKET_SOURCES})

file(GLOB MESSAGE_SOURCES nar/lib/Messaging/MessageTypes/*cpp)
add_library(narmessage SHARED ${MESSAGE_SOURCES})

set(SERVER_SOURCES
    nar/narserver/Database.cpp
    nar/narserver/server.cpp
    nar/narserver/sockinfo.cpp
    nar/narserver/ServerGlobal.cpp
    nar/narnode/utility.cpp
  )
set(NAR_LIBRARIES libnarmessage.so libnarfile.so libnarsocket.so pthread cryptopp z)

add_executable(narserver ${SERVER_SOURCES})

target_link_libraries(narserver ${NAR_LIBRARIES} mysqlcppconn)

add_dependencies(narserver narsocket narfile narmessage)
