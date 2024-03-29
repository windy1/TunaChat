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
        src/tuna.h src/tuna.cpp
        src/Client/ChatClient.cpp src/Client/ChatClient.h
        src/Client/Terminal/Terminal.cpp src/Client/Terminal/Terminal.h
        src/Client/Terminal/Window.cpp src/Client/Terminal/Window.h
        src/Client/tests.h src/Client/tests.cpp
        src/Client/Command.cpp src/Client/Command.h
        src/Client/ServerConn.cpp src/Client/ServerConn.h
        src/Client/Terminal/windows.h src/Client/Terminal/windows.cpp
        src/Client/MessageChannel.cpp src/Client/MessageChannel.h
        src/Client/Terminal/MainWindow.cpp src/Client/Terminal/MainWindow.h
        src/Client/Preferences.cpp src/Client/Preferences.h)

target_link_libraries(TunaChat ${CURSES_LIBRARIES})
