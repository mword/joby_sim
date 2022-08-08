cmake_minimum_required(VERSION 3.19)

project(google_tests)

add_subdirectory(googletest)
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR} ../)

add_executable(run_tests VehicleTest.cpp ChargingStationTest.cpp)
target_link_libraries(run_tests gtest_main dummy_lib_for_gtest)
