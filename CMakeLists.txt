cmake_minimum_required(VERSION 3.12)
project(TunaChat)

set(CMAKE_CXX_STANDARD 17)

find_package(Curses REQUIRED)
include_directories(${CURSES_INCLUDE_DIR})

add_executable(TunaChat
        src/main.cpp
        src/Server/ChatServer.cpp src/Server/ChatServer.h
        src/Server/ClientConn.cpp src/Server/ClientConn.h
        src/Server/User.cpp src/Server/User.h
        src/tuna.h
        src/Client/ChatClient.cpp src/Client/ChatClient.h
        src/Client/Terminal.cpp src/Client/Terminal.h src/Client/Window.cpp src/Client/Window.h src/Client/InputWindow.cpp src/Client/InputWindow.h)

target_link_libraries(TunaChat ${CURSES_LIBRARIES})
