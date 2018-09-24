cmake_minimum_required(VERSION 3.12)
project(TunaChat)

set(CMAKE_CXX_STANDARD 17)

add_executable(TunaChat src/main.cpp src/ChatServer.cpp src/ChatServer.h src/ClientConnection.cpp src/ClientConnection.h src/User.cpp src/User.h)