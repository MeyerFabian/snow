function(assign_source_group)
    foreach(_source IN ITEMS ${ARGN})
        if (IS_ABSOLUTE "${_source}")
            file(RELATIVE_PATH _source_rel "${CMAKE_CURRENT_SOURCE_DIR}" "${_source}")
        else()
            set(source_rel "${_source}")
        endif()
        get_filename_component(_source_path "${_source_rel}" PATH)
        string(REPLACE "/" "\\" _source_path_msvc "${_source_path}")
        source_group("${_source_path_msvc}" FILES "${_source}")
    endforeach()
endfunction(assign_source_group)

project(snowProject)
cmake_minimum_required(VERSION 3.0.1)
set(APPNAME scene)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")
set(SHADERS_PATH "${CMAKE_SOURCE_DIR}/shader" CACHE PATH "Path to code of shaders.")

find_package(GLEW REQUIRED)
find_package(ASSIMP REQUIRED)
find_package(GLFW3 REQUIRED)
find_package(GLM REQUIRED)
#if(GLEW_FOUND)
#	include_directories()
#endif()

include_directories(
${GLEW_INCLUDE_PATH}
${ASSIMP_INCLUDE_DIR}
${GLFW3_INCLUDE_PATH}
${GLM_INCLUDE_PATH}
include/stb_image
include/math3d
)


IF(MINGW)
set(DLLS ${ASSIMP_DLL} ${GLEW_DLL} ${GLFW3_DLL})
file(COPY ${DLLS} DESTINATION /)
file(COPY shader DESTINATION /)
file(COPY resources/textures DESTINATION /)
file(COPY resources/model DESTINATION /)
ENDIF()

if(EXISTS "${SHADERS_PATH}")
        add_subdirectory("${SHADERS_PATH}")
endif()


file(GLOB_RECURSE SOURCE_FILES "src/snow/*.cpp" "src/snow/*.h")
file(GLOB_RECURSE EXE_FILES "src/exe/*.cpp")

add_executable(scene
src/exe/main.cpp
)

target_link_libraries(scene
SNOWGL_LIBRARY
)

add_subdirectory(include/math3d)
add_subdirectory(include/stb_image)
add_subdirectory(src/snow)

IF(MSVC)
  set(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE  INTERNAL FORCE )
  file(COPY shader DESTINATION Release/)
  file(COPY resources/textures DESTINATION Release/)
  file(COPY resources/model DESTINATION Release/)
  # Copy dlls
  add_custom_command(TARGET ${APPNAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    "${ASSIMP_DLL}/assimp.dll"
    "${PROJECT_BINARY_DIR}/$<CONFIGURATION>/assimp.dll"
  )

  add_custom_command(TARGET ${APPNAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    "${GLFW3_DLL}/glfw3.dll"
    "${PROJECT_BINARY_DIR}/$<CONFIGURATION>/glfw3.dll"
  )

  add_custom_command(TARGET ${APPNAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND}
    -E copy_if_different
    "${GLEW_DLL}/glew.dll"
    "${PROJECT_BINARY_DIR}/$<CONFIGURATION>/glew.dll"
  )

ENDIF()
