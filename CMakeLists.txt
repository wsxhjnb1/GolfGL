cmake_minimum_required (VERSION 3.8)

project ("GolfGL")

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

if(UNIX)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIR_ft2build}")
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

  link_directories(${CMAKE_SOURCE_DIR}/External/freetype2/win64 
          ${CMAKE_SOURCE_DIR}/External/GLFW/lib 
          ${CMAKE_SOURCE_DIR}/External/assimp/lib)

  set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_SOURCE_DIR}/External/freetype2/include")          
endif(WIN32)

if (MSVC)	
	list(APPEND CMAKE_CXX_FLAGS " /std:c++17")  	
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
  
  set(LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
else()
  set(LIBS )
endif(WIN32)


configure_file(configuration/rootDirectory.h.in configuration/rootDirectory.h)
include_directories(${CMAKE_BINARY_DIR}/configuration)


# first create relevant static libraries requried for other projects
set(STB_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/stb_image")

set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/GLAD")
add_library(glad "${GLAD_INCLUDE_DIR}/glad.c")
set(LIBS ${LIBS} glad)

set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/imgui")
file(GLOB_RECURSE IMGUI_SRC 
    "${IMGUI_INCLUDE_DIR}/*.cpp"
		"${IMGUI_INCLUDE_DIR}/backends/*.cpp"
	)
add_library(imgui ${IMGUI_SRC})	
set(LIBS ${LIBS} imgui)


file(GLOB_RECURSE SOURCE
			"Game/Source/*.h"			
            "Game/Source/*.cpp" 
        )

set(NAME "GolfGL")
add_executable(${NAME} ${SOURCE})


target_link_libraries(${NAME} ${LIBS})
if(UNIX)
  
endif()

if(MSVC)
    set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "/Ycprecomp.h")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}/Debug")	
	  set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT ${NAME})	
	
else()
    target_precompile_headers(${NAME} PUBLIC Game/Source/precomp.h)
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
	
endif(MSVC)


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


include_directories(${ASSIMP_INCLUDE_DIR}
                    ${FREETYPE_INCLUDE_DIR_ft2build}
                    ${GLAD_INCLUDE_DIR}
                    ${GLFW3_INCLUDE_DIR}
                    ${IMGUI_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/External/imgui/backends
					          ${STB_IMAGE_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/External
                    ${CMAKE_SOURCE_DIR}/Game/Source
					          ${CMAKE_SOURCE_DIR}/Game/Source/Render
                   )
												