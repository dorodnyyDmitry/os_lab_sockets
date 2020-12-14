cmake_minimum_required(VERSION 2.8)

project(os_lab_sockets)
add_executable(${PROJECT_NAME} "server.c" "client1.c" "client2.c" "client3.c")

