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

set(CMAKE_EXPORT_COMPILE_COMMANDS 1)

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




file(GLOB_RECURSE SOURCE_FILES "src/snow/*.cpp" "src/snow/*.hpp")
file(GLOB_RECURSE EXE_FILES "src/exe/*.cpp")

list(GET EXE_FILES 0 FIRST_EXEC)
get_filename_component(FIRST_EXEC_NAME ${FIRST_EXEC} NAME_WE)
foreach (EXEC ${EXE_FILES})

	get_filename_component(EXEC_NAME ${EXEC} NAME_WE)
	add_executable(${EXEC_NAME}
		${EXEC}
		)

	target_link_libraries(${EXEC_NAME}
		SNOWGL_LIBRARY
		)

endforeach(EXEC)

add_subdirectory(include/math3d)
add_subdirectory(include/stb_image)
add_subdirectory(src/snow)
IF(MSVC)
	if(CMAKE_GENERATOR MATCHES "NMake")

		#OVERWRITE in your path_to_cmake\Modules\Platform\Windows.cmake to use compile_commands
		#
		#set(CMAKE_START_TEMP_FILE "")
		#set(CMAKE_END_TEMP_FILE "")
		#
		#SEE https://gitlab.kitware.com/cmake/cmake/issues/17482
		set(BUILD_DESTINATION ./)
		set(DLL_DESTINATION ${PROJECT_BINARY_DIR})
		add_custom_command(TARGET ${FIRST_EXEC_NAME} POST_BUILD
			COMMAND ${CMAKE_COMMAND} -E copy  ${CMAKE_BINARY_DIR}/compile_commands.json ${CMAKE_SOURCE_DIR}
			)
	else()
		set(CMAKE_CONFIGURATION_TYPES Release CACHE TYPE  INTERNAL FORCE)
		set(CMAKE_BUILD_TYPE Release CACHE TYPE  INTERNAL FORCE)
		set(BUILD_DESTINATION Release/)
		set(DLL_DESTINATION ${PROJECT_BINARY_DIR}/$<CONFIGURATION>)
	ENDIF()

	file(COPY shader DESTINATION ${BUILD_DESTINATION})
	file(COPY resources/textures DESTINATION ${BUILD_DESTINATION})
	file(COPY resources/model DESTINATION ${BUILD_DESTINATION})
	# Copy dlls
	add_custom_command(TARGET ${FIRST_EXEC_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND}
		-E copy_if_different
		"${ASSIMP_DLL}/assimp.dll"
		"${DLL_DESTINATION}/assimp.dll"
		)

	add_custom_command(TARGET ${FIRST_EXEC_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND}
		-E copy_if_different
		"${GLFW3_DLL}/glfw3.dll"
		"${DLL_DESTINATION}/glfw3.dll"
		)

	add_custom_command(TARGET ${FIRST_EXEC_NAME} PRE_BUILD
		COMMAND ${CMAKE_COMMAND}
		-E copy_if_different
		"${GLEW_DLL}/glew.dll"
		"${DLL_DESTINATION}/glew.dll"
		)
ENDIF()

