cmake_minimum_required(VERSION 2.8.9)
project (confreak)
add_definitions(-std=c++11)

#Directories
include_directories("${PROJECT_BINARY_DIR}")

#source files
#set(SOURCES confreak.m.cpp) #Manually include the source file
file(GLOB SOURCES "./*.cpp")
#file(GLOB HEADERS "./*.h")

#executable
add_executable(confreak.tsk  ${SOURCES})
