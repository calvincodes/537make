cmake_minimum_required(VERSION 3.12)
project(537make C)

set(CMAKE_C_STANDARD 99)

add_executable(537make main.c LinkedList.c LinkedList.h GraphNode.c GraphNode.h constants.c constants.h)
