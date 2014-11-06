project(srccmake)
cmake_minimum_required(VERSION 2.8)

aux_source_directory(. SRC_LIST)


include_directories(
include/glm/glm
include/glew-1.11.0/include/GL
include/glfw-3.0.4/include
include/assimp/include/assimp/
stb_image
)


SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

file(COPY shader DESTINATION /)
file(COPY textures DESTINATION /)
file(COPY model DESTINATION /)
add_executable(main
src/main.cpp 
src/math3d.h 
src/math3d.cpp
src/pipeline/pipeline.h
src/pipeline/pipeline.cpp
src/pipeline/camera.h
src/pipeline/camera.cpp
src/object/texture.h
src/object/texture.cpp
shader/shader.vert
shader/shader.frag
shader/m_shadow.vert
shader/m_shadow.frag
stb_image/stb_image.h
stb_image/stb_image.c
src/technique/technique.h
src/technique/technique.cpp
src/technique/lightingTechnique.h
src/technique/lightingTechnique.cpp
src/technique/shadowMapTechnique.h
src/technique/shadowMapTechnique.cpp
src/object/mesh.h
src/object/mesh.cpp
src/technique/ShadowMapBufferObject.h
src/technique/ShadowMapBufferObject.cpp
src/renderingEngine.h
src/myRenderingEngine.h
src/myRenderingEngine.cpp
src/physicEngine.h
src/myphysicEngine.h
src/myphysicEngine.cpp
)


target_link_libraries(main


${CMAKE_SOURCE_DIR}/include/glew-1.11.0/lib/Release/x64/glew32s.lib
opengl32.lib


${CMAKE_SOURCE_DIR}/include/glfw-3.0.4/lib-mingw/glfw3dll.a

${CMAKE_SOURCE_DIR}/include/assimp/lib/libassimp.dll.a

)
