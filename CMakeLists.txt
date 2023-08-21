cmake_minimum_required(VERSION 3.16)
project(modernc C)

set(CMAKE_C_STANDARD 11)

#===============================================================
# MCLIB Library
#===============================================================

#set(MCLIB "mclib/dynarray.h" mclib/dynarray.c mclib/params.h mclib/utest.h mclib/utest.c)

#file(GLOB MCLIB_SRC ${CMAKE_SOURCE_DIR} "mclib/*.c" "mclib/*.h")
#add_library(mclib ${MCLIB_SRC})

add_subdirectory(mclib)

#===============================================================
# MODERNC Example Program
#===============================================================

add_executable(modernc main.c memcheck.h memcheck.c)
target_link_libraries(modernc mclib)

add_executable(test_driver test_driver.c tests/dynarray_tests.h)
target_link_libraries(test_driver mclib)

#===============================================================
# Santa Claus Inc Example Application
#===============================================================

# moved this to subdirectory examples/santa_claus_inc
#add_library(santa_claus_inc_lib
#        santa_claus_inc/child_data_mgmt.c santa_claus_inc/child_data_mgmt.h
#        santa_claus_inc/tour_plan.c santa_claus_inc/tour_plan.h)
#target_link_libraries(santa_claus_inc_lib mclib)
#
#add_executable(santa_claus_inc santa_claus_inc/santa_claus_main.c)
#target_link_libraries(santa_claus_inc santa_claus_inc_lib)
#
#add_executable(santa_claus_inc_test santa_claus_inc/stanta_claus_test_driver.c santa_claus_inc/tests_santa_claus.h)
#target_link_libraries(santa_claus_inc_test santa_claus_inc_lib)

add_subdirectory(examples/santa_claus_inc)
target_link_libraries(santa_claus_inc mclib)
target_link_libraries(santa_claus_inc_test mclib)

#===============================================================
# Caravan Example Application
#===============================================================

add_subdirectory(examples/caravan)
target_link_libraries(caravan mclib)