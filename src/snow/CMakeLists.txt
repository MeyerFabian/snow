assign_source_group(${SOURCE_FILES})
add_library(SNOWGL_LIBRARY ${CMAKE_SOURCE_DIR} ${SOURCE_FILES} )

target_link_libraries(SNOWGL_LIBRARY

${GLFW3_LIBRARY}

${GLEW_LIBRARY}

stb_image

opengl32.lib

${GLFW3_LIBRARY}

${ASSIMP_LIBRARY}
)