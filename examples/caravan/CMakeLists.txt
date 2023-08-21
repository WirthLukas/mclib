cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)

#===============================================================
# Caravan Example Application
#===============================================================

add_library(caravan_lib animals.h animals.c)
add_executable(caravan caravan_main.c)
target_link_libraries(caravan caravan_lib)