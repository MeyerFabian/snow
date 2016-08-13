project(snowProject)
cmake_minimum_required(VERSION 3.0.1)
set(APPNAME snow)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake CACHE PATH "Project specific path. Set manually if it was not found.")


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
#${CMAKE_SOURCE_DIR}/include
stb_image
)


IF(MINGW)
set(DLLS ${ASSIMP_DLL} ${GLEW_DLL} ${GLFW3_DLL})
file(COPY ${DLLS} DESTINATION /)
file(COPY shader DESTINATION /)
file(COPY textures DESTINATION /)
file(COPY model DESTINATION /)
ENDIF()

file(GLOB_RECURSE SOURCE_FILES "src/*.cpp")


add_executable(snow
stb_image/stb_image.c
${SOURCE_FILES}
)


target_link_libraries(snow

${GLEW_LIBRARY}

opengl32.lib

${GLFW3_LIBRARY}

${ASSIMP_LIBRARY}

)
IF(MSVC)
  set(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE  INTERNAL FORCE )
  file(COPY shader DESTINATION Release/)
  file(COPY textures DESTINATION Release/)
  file(COPY model DESTINATION Release/)
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
    "${GLEW_DLL}/glew32.dll"
    "${PROJECT_BINARY_DIR}/$<CONFIGURATION>/glew32.dll"
  )
  add_custom_target(SHADERS SOURCES ${SHADER_TARGET})
  add_custom_target(MODELS SOURCES ${MODEL_TARGET})
  add_custom_target(TEXTURES SOURCES ${TEXTURE_TARGET})

ENDIF()
