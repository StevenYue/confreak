cmake_minimum_required(VERSION 2.8.9)
project (confreak)
set(CMAKE_CXX_FLAGS_DEBUG)
add_definitions(-std=c++11)

#Directories
include_directories("${PROJECT_BINARY_DIR}")
include_directories("${PROJECT_BINARY_DIR}/deps")
#include_directories("/usr/lib/x86_64-linux-gnu/")

#source files
#set(SOURCES confreak.m.cpp) #Manually include the source file
file(GLOB SOURCES  "deps/*.cpp" "deps/*.c" "*.cpp")

#executable
add_executable(confreak.tsk  ${SOURCES}  )

#Adding LIBS
target_link_libraries(confreak.tsk curl)
target_link_libraries(confreak.tsk pthread)
target_link_libraries(confreak.tsk jsoncpp)
