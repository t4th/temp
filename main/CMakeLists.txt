
cmake_minimum_required (VERSION 3.8)

set(${PROJECT_ID}_SOURCES
	TemperatureControl.cpp
)

# the PROJECT_ID target shall be an executable
add_executable(${PROJECT_ID})

# set the source files to be built into the executable
target_sources(${PROJECT_ID}
	PRIVATE
		${${PROJECT_ID}_SOURCES}
)