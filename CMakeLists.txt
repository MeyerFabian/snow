project(srccmake)
cmake_minimum_required(VERSION 2.8)


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
set(DLLS ${ASSIMP_DLL} ${GLEW_DLL} ${GLFW3_DLL})
file(COPY ${DLLS} DESTINATION /)

file(COPY shader DESTINATION /)
file(COPY textures DESTINATION /)
file(COPY model DESTINATION /)

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
