cmake_minimum_required (VERSION 3.8)


project ("GolfGL")


IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
ENDIF(NOT CMAKE_BUILD_TYPE)



if (NOT MSVC)	
	list(APPEND CMAKE_CXX_FLAGS " -std=c++17")
endif()

message(STATUS ${CMAKE_CXX_FLAGS})


set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


if(UNIX)
  find_package(Freetype REQUIRED)
  message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIR_ft2build}")
  
  set(GLAD_LIBRARY "${CMAKE_SOURCE_DIR}/External/GLAD/lib/unix/libglad.a")
  set(IMGUI_LIBRARY "${CMAKE_SOURCE_DIR}/External/imgui/lib/unix/libimgui.a")    
endif(UNIX)

if(WIN32)
	set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

  link_directories(${CMAKE_SOURCE_DIR}/External/freetype2/win64 
                  ${CMAKE_SOURCE_DIR}/External/GLFW/lib 
                  ${CMAKE_SOURCE_DIR}/External/assimp/lib
                  ${CMAKE_SOURCE_DIR}/External/GLAD/lib/win64
                  ${CMAKE_SOURCE_DIR}/External/imgui/lib/win64
                  )

  set(FREETYPE_INCLUDE_DIR_ft2build "${CMAKE_SOURCE_DIR}/External/freetype2/include")    
endif(WIN32)

# Include paths for External libs
set(GLAD_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/GLAD")
set(IMGUI_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/imgui")
set(STB_IMAGE_INCLUDE_DIR "${CMAKE_SOURCE_DIR}/External/stb_image")

# find the required packages
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules")
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")



# Grouping libs
if(WIN32)
  set(LIBS glad glfw3 opengl32 assimp freetype imgui)

else() # UNIX
  set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
  find_package(OpenGL REQUIRED)
  add_definitions(${OPENGL_DEFINITIONS})
  find_package(X11 REQUIRED)  
  
  set(LIBS ${GLAD_LIBRARY} ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl 
      pthread freetype ${ASSIMP_LIBRARY} ${IMGUI_LIBRARY})
  set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
endif(WIN32)


set(_src_root_path "${CMAKE_SOURCE_DIR}/Game")
# CLANG-FORMAT
file(GLOB_RECURSE GameSrc
  "Game/*.cpp"
  "Game/*.h"
  )

# foreach(srcFile ${GameSrc})   
#   execute_process(COMMAND "clang-format -i" "${srcFile}"
#                   COMMAND_ECHO STDOUT
#                   )
# endforeach()


# Game files
set(NAME "GolfGL")

file(
    GLOB_RECURSE _source_list 
    LIST_DIRECTORIES false
    "${_src_root_path}/*.cpp"
    "${_src_root_path}/*.h"
    "${_src_root_path}/*.vs"
    "${_src_root_path}/*.fs"
)


add_executable(${NAME} ${_source_list})
foreach(_source IN ITEMS ${_source_list})
    get_filename_component(_source_path "${_source}" PATH)
    file(RELATIVE_PATH _source_path_rel "${_src_root_path}" "${_source_path}")
    string(REPLACE "/" "\\" _group_path "${_source_path_rel}")
    source_group("${_group_path}" FILES "${_source}")
endforeach()


target_link_libraries(${NAME} ${LIBS})


if(MSVC)        
    set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "/Ycprecomp.h /std:c++17")
    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
    set_target_properties(${NAME} PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}/Debug")	    
    set_property(DIRECTORY PROPERTY VS_STARTUP_PROJECT "${NAME}")	        
       
    # CLANG-TIDY
    set_target_properties(${NAME} PROPERTIES VS_GLOBAL_RunCodeAnalysis "false")
    set_target_properties(${NAME} PROPERTIES VS_GLOBAL_EnableClangTidyCodeAnalysis "true")
    set_target_properties(${NAME} PROPERTIES VS_GLOBAL_ClangTidyChecks "--checks=*")
    set_target_properties(${NAME} PROPERTIES VS_GLOBAL_EnableMicrosoftCodeAnalysis "false")
	
else()
    if("${CMAKE_BUILD_TYPE}" STREQUAL "Debug")
      set_target_properties(${NAME} PROPERTIES COMPILE_FLAGS "-g")
      target_compile_definitions(${NAME} PUBLIC _DEBUG)
    else()
      target_compile_definitions(${NAME} PUBLIC NDEBUG)
    endif()


    set_target_properties(${NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/${NAME}")
	
endif(MSVC)


add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND ${CMAKE_COMMAND} -E make_directory
							$<TARGET_FILE_DIR:${NAME}>/Resources)
add_custom_command(TARGET ${NAME} PRE_BUILD COMMAND
						${CMAKE_COMMAND} -E copy_directory
						"${CMAKE_SOURCE_DIR}/Game/Resources"
						$<TARGET_FILE_DIR:${NAME}>/Resources)
							

if(WIN32)									
		add_custom_command(TARGET ${NAME} PRE_LINK COMMAND 
								${CMAKE_COMMAND} -E copy
								"${CMAKE_SOURCE_DIR}/External/assimp/lib/assimp-vc140-mt.dll" $<TARGET_FILE_DIR:${NAME}>)	
endif(WIN32)

file(GLOB_RECURSE externHeaders ${CMAKE_SOURCE_DIR}/External/*.h)
set_source_files_properties(${externHeaders} PROPERTIES COMPILE_FLAGS "-w")

include_directories(${ASSIMP_INCLUDE_DIR}
                    ${FREETYPE_INCLUDE_DIR_ft2build}
                    ${GLAD_INCLUDE_DIR}
                    ${GLFW3_INCLUDE_DIR}
                    ${IMGUI_INCLUDE_DIR}
                    ${IMGUI_INCLUDE_DIR}/backends
					          ${STB_IMAGE_INCLUDE_DIR}
                    ${CMAKE_SOURCE_DIR}/External
                    ${CMAKE_SOURCE_DIR}/Game/Source
					          ${CMAKE_SOURCE_DIR}/Game/Source/Render
                   )


				
