
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("Temp_test")

add_executable( Temp "main/main.cpp" "TemperatureControl/TemperatureControl.cpp" "TemperatureControl/TemperatureControl.h" )

add_subdirectory(test/googletest)

add_executable( Temp_test "test/TemperatureControl_tests.cpp" "TemperatureControl/TemperatureControl.cpp" "TemperatureControl/TemperatureControl.h" )

target_link_libraries(
  Temp_test
  GTest::gtest_main
  GTest::gmock
)
