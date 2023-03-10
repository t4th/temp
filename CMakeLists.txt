
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 17)

project("Temp")


add_executable( Temp "main/main.cpp" "TemperatureControl/TemperatureControl.cpp" "TemperatureControl/TemperatureControl.h" )