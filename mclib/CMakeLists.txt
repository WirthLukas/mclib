cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)

#===============================================================
# MCLIB Library
#===============================================================

file(GLOB MCLIB_SRC ${CMAKE_SOURCE_DIR} "./*.c" "./*.h")

add_library(mclib ${MCLIB_SRC})