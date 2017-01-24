cmake_minimum_required(VERSION 2.8.9)
project (confreak)
add_definitions(-std=c++11)

#Directories
include_directories("${PROJECT_BINARY_DIR}")
include_directories("/usr/lib/x86_64-linux-gnu/")
include_directories("/usr/local/include/")

#source files
#set(SOURCES confreak.m.cpp) #Manually include the source file
file(GLOB SOURCES "./*.cpp")
#file(GLOB HEADERS "./*.h")

#executable
add_executable(confreak.tsk  ${SOURCES})

#Adding LIBS
target_link_libraries(confreak.tsk curl)
