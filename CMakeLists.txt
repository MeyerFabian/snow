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
src/rendering/pipeline/pipeline.h
src/rendering/pipeline/pipeline.cpp
src/rendering/pipeline/camera.h
src/rendering/pipeline/camera.cpp
src/object/texture.h
src/object/texture.cpp
shader/shader.vert
shader/shader.frag
shader/m_shadow.vert
shader/m_shadow.frag
shader/particleShader.vert
shader/particleShader.frag
stb_image/stb_image.h
stb_image/stb_image.c
src/technique.h
src/technique.cpp
src/rendering/technique/lightingTechnique.h
src/rendering/technique/lightingTechnique.cpp
src/rendering/technique/shadowMapTechnique.h
src/rendering/technique/shadowMapTechnique.cpp
src/rendering/technique/ParticleTechnique.h
src/rendering/technique/ParticleTechnique.cpp
src/object/mesh.h
src/object/mesh.cpp
src/rendering/technique/ShadowMapBufferObject.h
src/rendering/technique/ShadowMapBufferObject.cpp
src/renderingEngine.h
src/rendering/myRenderingEngine.h
src/rendering/myRenderingEngine.cpp
src/physicEngine.h
src/simulation/myPhysicEngine.h
src/simulation/myPhysicEngine.cpp
src/simulation/timeUpdate.h
src/simulation/timeUpdate/explicittimeupdate.h
src/simulation/timeUpdate/explicittimeupdate.cpp
src/simulation/timeUpdate/semiimplicittimeupdate.h
src/simulation/timeUpdate/semiimplicittimeupdate.cpp
src/object/particle.h
src/object/grid.h
src/object/grid.cpp
src/object/gridpoint.h
src/object/particlesystem.h
src/object/particlesystem.cpp
src/simulation/technique/particleCompute.h
src/simulation/technique/particleCompute.cpp
)


target_link_libraries(main


${CMAKE_SOURCE_DIR}/include/glew-1.11.0/lib/Release/x64/glew32s.lib
opengl32.lib


${CMAKE_SOURCE_DIR}/include/glfw-3.0.4/lib-mingw/glfw3dll.a

${CMAKE_SOURCE_DIR}/include/assimp/lib/libassimp.dll.a

)
