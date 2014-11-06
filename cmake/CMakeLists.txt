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
srccmake/main.cpp 
srccmake/math3d.h 
srccmake/math3d.cpp
srccmake/pipeline/pipeline.h
srccmake/pipeline/pipeline.cpp
srccmake/pipeline/camera.h
srccmake/pipeline/camera.cpp
srccmake/object/texture.h
srccmake/object/texture.cpp
shader/shader.vert
shader/shader.frag
shader/m_shadow.vert
shader/m_shadow.frag
stb_image/stb_image.h
stb_image/stb_image.c
srccmake/technique/technique.h
srccmake/technique/technique.cpp
srccmake/technique/lightingTechnique.h
srccmake/technique/lightingTechnique.cpp
srccmake/technique/shadowMapTechnique.h
srccmake/technique/shadowMapTechnique.cpp
srccmake/object/mesh.h
srccmake/object/mesh.cpp
srccmake/technique/ShadowMapBufferObject.h
srccmake/technique/ShadowMapBufferObject.cpp
srccmake/renderingEngine.h
srccmake/myRenderingEngine.h
srccmake/myRenderingEngine.cpp
srccmake/physicEngine.h
srccmake/myphysicEngine.h
srccmake/myphysicEngine.cpp
)


target_link_libraries(main


${CMAKE_SOURCE_DIR}/include/glew-1.11.0/lib/Release/x64/glew32s.lib
opengl32.lib


${CMAKE_SOURCE_DIR}/include/glfw-3.0.4/lib-mingw/glfw3dll.a

${CMAKE_SOURCE_DIR}/include/assimp/lib/libassimp.dll.a

)
