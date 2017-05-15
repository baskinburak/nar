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
find_library(OPENSSL_LIB ssl $ENV{OPENSSL_LIB_PATH} REQUIRED)
find_library(libcrypto REQUIRED)


find_package(Boost COMPONENTS system filesystem iostreams random REQUIRED)

file(GLOB SOCKET_SOURCES nar/lib/Socket/Packet.cpp nar/lib/Socket/Socket.cpp nar/lib/Socket/USocket.cpp)
add_library(narsocket SHARED ${SOCKET_SOURCES})

file(GLOB IPC_MESSAGING_SOURCES nar/lib/Messaging/MessageTypes/IPC*cpp)
add_library(naripcmessaging SHARED ${IPC_MESSAGING_SOURCES})

file(GLOB NAR_MESSAGING_SOURCES
        nar/lib/Messaging/MessageTypes/UserAuthenticationAnswer.cpp
        nar/lib/Messaging/MessageTypes/UserAuthenticationInit.cpp
        nar/lib/Messaging/MessageTypes/FilePush.cpp
        nar/lib/Messaging/MessageTypes/FilePull.cpp
        nar/lib/Messaging/MessageTypes/UserRegister.cpp
        nar/lib/Messaging/messaging_utility.cpp
        nar/lib/Messaging/MessageTypes/ResponseHeader.cpp
        nar/lib/Messaging/MessageTypes/RequestHeader.cpp
        nar/lib/Messaging/MessageTypes/WaitChunkPush.cpp
        nar/lib/Messaging/MessageTypes/WaitChunkPull.cpp
        nar/lib/Messaging/MessageTypes/KeepAlive.cpp
        nar/lib/Messaging/MessageTypes/KeepAliveCheck.cpp
        nar/lib/Messaging/MessageTypes/ResponseHeader.cpp
        nar/lib/Messaging/MessageTypes/RequestHeader.cpp
        nar/lib/Messaging/MessageTypes/UserRegister.cpp
        nar/lib/Messaging/MessageTypes/MachineRegister.cpp
        nar/lib/Messaging/MessageTypes/DirInfo.cpp
        nar/lib/base64/base64.cpp
        nar/lib/Messaging/MessageTypes/DirInfo.cpp
        nar/lib/Messaging/MessageTypes/Mkdir.cpp
        nar/lib/Messaging/MessageTypes/DeleteFile.cpp
        nar/lib/Messaging/MessageTypes/DeleteMachineChunk.cpp
        nar/lib/Messaging/MessageTypes/DaemonShutdown.cpp
        nar/lib/Messaging/MessageTypes/InfoChunkPull.cpp
        )
add_library(narmessaging SHARED ${NAR_MESSAGING_SOURCES})

file(GLOB NAR_CRYPTION_SOURCES
        nar/lib/Cryption/aes.cpp
        nar/lib/Cryption/rsa.cpp
        )
add_library(narcrypto SHARED ${NAR_CRYPTION_SOURCES})


set(SERVER_SOURCES
        nar/narserver/Database.cpp
        nar/narserver/server.cpp
        nar/narserver/ServerGlobal.cpp
        nar/narserver/Actions/authenticate_action.cpp
        nar/narserver/Actions/register_action.cpp
        nar/narnode/utility.cpp
        nar/narnode/uservars.cpp
        nar/narnode/File/File.cpp
        nar/narserver/Actions/AuthActions.cpp
        nar/narserver/peers.cpp
        nar/narserver/sockinfo.cpp
        nar/narserver/Actions/keepalive_action.cpp
        nar/narserver/Actions/daemon_shutdown_action.cpp
        )


set(CLI_SOURCES
        nar/narnode/cli.cpp
        nar/narnode/clitasks.cpp
        nar/narnode/utility.cpp
        nar/narnode/uservars.cpp
        nar/narnode/File/File.cpp
        )

set(DAEMON_SOURCES
        nar/narnode/daemon.cpp
        nar/narnode/utility.cpp
        nar/narnode/uservars.cpp
        nar/narnode/global.cpp
        nar/narnode/File/File.cpp
        nar/narnode/reactive.cpp
        nar/narnode/ActiveTask/ActiveFunctions.cpp
        nar/narnode/ActiveTask/Push.cpp
        nar/narnode/ActiveTask/Register.cpp
        nar/narnode/ActiveTask/Pull.cpp
        nar/narnode/ActiveTask/LS.cpp
        nar/narnode/ActiveTask/Mkdir.cpp
        nar/narnode/ActiveTask/DeleteFile.cpp
        nar/narnode/ActiveTask/Status.cpp
        )

set(DAEMON_LIBRARIES narcrypto.so narmessaging.so libnaripcmessaging.so libnarsocket.so pthread cryptopp z ${OPENSSL_LIB} crypto)
set(SERVER_LIBRARIES narcrypto.so narmessaging.so libnarsocket.so pthread cryptopp mysqlcppconn ${OPENSSL_LIB} crypto)
set(CLI_LIBRARIES libnaripcmessaging.so libnarsocket.so pthread cryptopp ${OPENSSL_LIB} crypto)


add_executable(narcli ${CLI_SOURCES})
target_link_libraries(narcli ${CLI_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY})
add_dependencies(narcli narsocket naripcmessaging narmessaging)

add_executable(narserver ${SERVER_SOURCES})
target_link_libraries(narserver ${SERVER_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_RANDOM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY})
add_dependencies(narserver narcrypto narsocket narmessaging)

add_executable(nardaemon ${DAEMON_SOURCES})
target_link_libraries(nardaemon ${DAEMON_LIBRARIES} ${Boost_FILESYSTEM_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_IOSTREAMS_LIBRARY} ${Boost_RANDOM_LIBRARY})
add_dependencies(nardaemon narcrypto narsocket naripcmessaging narmessaging)
