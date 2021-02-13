cmake_minimum_required (VERSION 3.8)

project ("GolfGL")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif(WIN32)

link_directories(${CMAKE_SOURCE_DIR}/External/freetype2/win64 
				 ${CMAKE_SOURCE_DIR}/External/GLFW/lib 
				 ${CMAKE_SOURCE_DIR}/External/assimp/lib)

if (MSVC)	
	list(APPEND CMAKE_CXX_FLAGS " /std:c++17")
	list(APPEND CMAKE_CXX_FLAGS_DEBUG " /std:c++17")	
else()
	list(APPEND CMAKE_CXX_FLAGS " -std=c++17")	
endif(MSVC)

message(STATUS ${CMAKE_CXX_FLAGS})

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")


if(WIN32)
  set(LIBS glfw3 opengl32 assimp freetype)
elseif(UNIX)
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)
  # note that the order is important for setting the libs
  # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
else()
  set(LIBS )
endif(WIN32)


configure_file(configuration/root_directory.h.in configuration/root_directory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)


# first create relevant static libraries requried for other projects
add_library(STB_IMAGE "External/stb_image/stb_image.cpp")
set(LIBS ${LIBS} STB_IMAGE)

add_library(GLAD "External/GLAD/glad.c")
set(LIBS ${LIBS} GLAD)

file(GLOB IMGUI_SRC 
		"External/imgui/*.cpp"
		"External/imgui/backends/*.cpp"
	)
add_library(imgui ${IMGUI_SRC})	
set(LIBS ${LIBS} imgui)


file(GLOB SOURCE
			"Game/Source/*.h"			
            "Game/Source/*.cpp" 
            "Game/Source/**/*.h"		
            "Game/Source/**/*.cpp"
			"Game/Source/**/**/*.h"		
            "Game/Source/**/**/*.cpp"
        )

set(NAME "GolfGL")
add_executable(${NAME} ${SOURCE})
target_link_libraries(${NAME} ${LIBS})
target_precompile_headers(${NAME} PUBLIC Game/Source/precomp.h)

if(WIN32)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}/Debug")
elseif(UNIX)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
endif(WIN32)


add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
							$<TARGET_FILE_DIR:${NAME}>/Resources)
add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND
						${CMAKE_COMMAND} -E copy_directory
						"${CMAKE_SOURCE_DIR}/Game/Resources"
						$<TARGET_FILE_DIR:${NAME}>/Resources)
							

if(WIN32)									
		add_custom_command(TARGET ${NAME} POST_BUILD COMMAND 
								${CMAKE_COMMAND} -E copy
								"${CMAKE_SOURCE_DIR}/External/assimp/lib/assimp-vc140-mt.dll" $<TARGET_FILE_DIR:${NAME}>)	
endif(WIN32)

if(MSVC)
    configure_file(${CMAKE_SOURCE_DIR}/configuration/visualstudio.vcxproj.user.in ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.vcxproj.user @ONLY)
endif(MSVC)    

include_directories(${CMAKE_SOURCE_DIR}/External/assimp/include
                    ${CMAKE_SOURCE_DIR}/External/freetype2/include
                    ${CMAKE_SOURCE_DIR}/External/GLAD
                    ${CMAKE_SOURCE_DIR}/External/GLFW/include                    
                    ${CMAKE_SOURCE_DIR}/External/imgui
                    ${CMAKE_SOURCE_DIR}/External/imgui/backends
					${CMAKE_SOURCE_DIR}/External/stb_image
                    ${CMAKE_SOURCE_DIR}/External
                    ${CMAKE_SOURCE_DIR}/Game/Source
					${CMAKE_SOURCE_DIR}/Game/Source/Render)