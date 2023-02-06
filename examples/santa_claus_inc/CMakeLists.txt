cmake_minimum_required(VERSION 3.16)

set(CMAKE_C_STANDARD 11)

#===============================================================
# Santa Claus Inc Example Application
#===============================================================

# mclib gets linked at root CMakeLists.txt
add_library(santa_claus_inc_lib
        child_data_mgmt.c child_data_mgmt.h
        tour_plan.c tour_plan.h)

add_executable(santa_claus_inc santa_claus_main.c)
target_link_libraries(santa_claus_inc santa_claus_inc_lib)

add_executable(santa_claus_inc_test stanta_claus_test_driver.c tests_santa_claus.h)
target_link_libraries(santa_claus_inc_test santa_claus_inc_lib)