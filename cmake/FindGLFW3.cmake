#
# Try to find GLFW3 library and include path.
# Once done this will define
#
# GLFW3_FOUND
# GLFW3_INCLUDE_PATH
# GLFW3_LIBRARY
#

SET(GLFW3_SEARCH_PATHS
	$ENV{GLFW3_ROOT}
	${DEPENDENCIES_ROOT}
	/usr			# APPLE
	/usr/local		# APPLE
	/opt/local		# APPLE
	$ENV{PROGRAMFILES}/GLFW/
	C:/GLFW
)

IF (MINGW)
FIND_PATH(GLFW3_INCLUDE_PATH
    NAMES
       	GLFW/glfw3.h
    PATHS
        ${GLFW3_SEARCH_PATHS}
    PATH_SUFFIXES
        include
    DOC
        "The directory where GLFW/glfw3.h resides"
)

FIND_LIBRARY( GLFW3_LIBRARY
    NAMES
				glfw3dll.a
    PATHS
        ${GLFW3_SEARCH_PATHS}
		PATH_SUFFIXES
				lib-mingw-w64
		DOC
				"The glfw3dll.a library."
)

FIND_LIBRARY( GLFW3_DLL
    NAMES
				glfw3.dll
    PATHS
        ${GLFW3_SEARCH_PATHS}
		PATH_SUFFIXES
				lib-mingw-w64
		DOC
				"The glfw3dll.a library."
)
ELSEIF (MSVC)

	FIND_PATH(GLFW3_INCLUDE_PATH
    NAMES
       GLFW/glfw3.h
    PATHS
        ${GLFW3_SEARCH_PATHS}
    PATH_SUFFIXES
        include
    DOC
        "The directory where GLFW/glfw3.h resides"
	)
	FIND_LIBRARY(GLFW3_LIBRARY
    NAMES
		glfw3dll.a
    PATHS
        ${GLFW3_SEARCH_PATHS}
    PATH_SUFFIXES
        lib-vc2013
    DOC
        "The directory where GLFW/glfw3.h resides"
	)

ELSEIF(APPLE)

FIND_PATH(GLFW3_INCLUDE_PATH
    NAMES
       GLFW/glfw3.h
    PATHS
        ${GLFW3_SEARCH_PATHS}
    PATH_SUFFIXES
        include
    DOC
        "The directory where GLFW/glfw3.h resides"
)

FIND_LIBRARY(GLFW3_LIBRARY
    NAMES
       libglfw3.a glfw
    PATHS
        ${GLFW3_SEARCH_PATHS}
    PATH_SUFFIXES
        lib
    DOC
        "The directory where GLFW/glfw3.h resides"
)

ELSE()
	FIND_PATH(GLFW3_INCLUDE_PATH GLFW/glfw3.h)
	FIND_LIBRARY(GLFW3_LIBRARY
        NAMES glfw3 glfw
	PATH_SUFFIXES dynamic)
ENDIF ()


SET(GLFW3_FOUND "NO")
IF (GLFW3_INCLUDE_PATH AND GLFW3_LIBRARY)
	SET(GLFW3_LIBRARIES ${GLFW3_LIBRARY})
	SET(GLFW3_FOUND "YES")
    message("EXTERNAL LIBRARY 'GLFW3' FOUND")
ELSE()
    message("ERROR: EXTERNAL LIBRARY 'GLFW3' NOT FOUND")
ENDIF (GLFW3_INCLUDE_PATH AND GLFW3_LIBRARY)
