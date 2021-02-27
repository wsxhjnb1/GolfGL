cmake_minimum_required (VERSION 3.8)

if(NOT(EXISTS "${CMAKE_SOURCE_DIR}/build"))
	file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/build")
	
	message("Made ${CMAKE_SOURCE_DIR}/build directory")
endif()

execute_process(COMMAND ${CMAKE_COMMAND} ${CMAKE_SOURCE_DIR} 
				WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/build"
				)
				
string(REPLACE "/" "\\" ROOT_STR ${CMAKE_SOURCE_DIR})
set(CMAKE_WIN ${ROOT_STR})

configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.filters.in
				${CMAKE_SOURCE_DIR}/build/GolfGL.vcxproj.filters @ONLY
				)				
				
message("GolfGL.vcxproj.filters inserted in project")