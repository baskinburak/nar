cmake_minimum_required(VERSION 2.8.3)
project(narnode)
set(CMAKE_BUILD_TYPE Release)
set (CMAKE_CXX_STANDARD 14)
include_directories(.)
include_directories()
link_directories(build)

find_library(CryptoPP REQUIRED)
find_library(Z REQUIRED)
find_library(MYSQLCPPCONN REQUIRED)

find_package(Boost COMPONENTS system filesystem iostreams random REQUIRED)

file(GLOB SOCKET_SOURCES nar/lib/Socket/Packet.cpp nar/lib/Socket/Socket.cpp nar/lib/Socket/USocket.cpp)
add_library(narsocket SHARED ${SOCKET_SOURCES})

#file(GLOB MESSAGE_SOURCES nar/lib/Messaging/MessageTypes/*cpp nar/lib/Messaging/messaging_utility.cpp)
#add_library(narmessage SHARED ${MESSAGE_SOURCES})

#set(SERVER_SOURCES
#    nar/narserver/Database.cpp
#    nar/narserver/server.cpp
#    nar/narserver/sockinfo.cpp
#    nar/narserver/ServerGlobal.cpp
#    nar/narnode/utility.cpp
#  )

set(NAR_LIBRARIES libnarsocket.so pthread cryptopp z mysqlcppconn)

#add_executable(narserver ${SERVER_SOURCES})

#target_link_libraries(narserver ${NAR_LIBRARIES} mysqlcppconn)

#add_dependencies(narserver narsocket narfile narmessage)

set(CLI_SOURCES
    nar/narnode/cli.cpp
    nar/narnode/clitasks.cpp
    nar/narnode/utility.cpp
    nar/narnode/uservars.cpp
    nar/narnode/File/File.cpp
    nar/lib/Messaging/MessageTypes/IPCLs.cpp
    nar/lib/Messaging/MessageTypes/IPCBaseRequest.cpp
    nar/lib/Messaging/MessageTypes/IPCBaseResponse.cpp
)

set(DAEMON_SOURCES
    nar/narnode/daemon.cpp
    nar/narnode/utility.cpp
    nar/narnode/uservars.cpp
    nar/narnode/global.cpp
    nar/narnode/File/File.cpp
    nar/lib/Cryption/aes.cpp
    nar/lib/Cryption/rsa.cpp
    nar/lib/Messaging/MessageTypes/FilePush.cpp
    nar/lib/Messaging/MessageTypes/IPCLs.cpp
    nar/lib/Messaging/MessageTypes/IPCBaseRequest.cpp
    nar/lib/Messaging/MessageTypes/IPCBaseResponse.cpp
    nar/lib/Messaging/MessageTypes/IPCLs.cpp
    nar/lib/Messaging/MessageTypes/IPCPush.cpp
    nar/lib/Messaging/MessageTypes/UserAuthenticationAnswer.cpp
    nar/lib/Messaging/MessageTypes/UserAuthenticationInit.cpp
    nar/lib/Messaging/MessageTypes/WaitChunkPush.cpp
    nar/lib/Messaging/MessageTypes/WaitChunkPull.cpp
    nar/lib/Messaging/MessageTypes/KeepAlive.cpp
    nar/lib/Messaging/MessageTypes/ResponseHeader.cpp
    nar/lib/Messaging/MessageTypes/RequestHeader.cpp
    nar/lib/Messaging/MessageTypes/FilePush.cpp
    nar/lib/Messaging/messaging_utility.cpp
    nar/narnode/reactive.cpp
    nar/narnode/ActiveTask/ActiveFunctions.cpp
    nar/narnode/ActiveTask/Push.cpp
)

set(REACT_TEST_SOURCES
    nar/narnode/utility.cpp
    nar/narnode/uservars.cpp
    nar/narnode/global.cpp
    nar/narnode/File/File.cpp
    nar/lib/Messaging/MessageTypes/IPCLs.cpp
    nar/lib/Messaging/MessageTypes/IPCBaseRequest.cpp
    nar/lib/Messaging/MessageTypes/IPCBaseResponse.cpp
    nar/lib/Messaging/MessageTypes/WaitChunkPush.cpp
    nar/lib/Messaging/MessageTypes/WaitChunkPull.cpp
    nar/lib/Messaging/MessageTypes/KeepAlive.cpp
    nar/lib/Messaging/MessageTypes/ResponseHeader.cpp
    nar/lib/Messaging/MessageTypes/RequestHeader.cpp
    nar/lib/Messaging/messaging_utility.cpp
    nar/narnode/reacTest.cpp
)

#add_executable(narcli ${CLI_SOURCES})
#target_link_libraries(narcli ${NAR_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY})
#add_dependencies(narcli narsocket)

add_executable(nardaemon ${DAEMON_SOURCES})
target_link_libraries(nardaemon ${NAR_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_RANDOM_LIBRARY})
add_dependencies(nardaemon narsocket)

#add_executable(narreact ${REACT_TEST_SOURCES})
#target_link_libraries(narreact ${NAR_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_RANDOM_LIBRARY})
#add_dependencies(narreact narsocket)
