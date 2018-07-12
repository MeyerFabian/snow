#
# Try to find GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_PATH
# GLEW_LIBRARY
#
SET(GLEW_SEARCH_PATHS
    $ENV{GLEW}
    $ENV{GLEW}                  # ASSIMP!
    ${DEPENDENCIES_ROOT}
    ./lib/glew
    $ENV{PROGRAMFILES}/glew           # WINDOWS
		C:/glew
    ~/Library/Frameworks                # MAC
    /Library/Frameworks                 # MAC
    /usr/local                          # LINUX/MAC/UNIX
    /usr                                # LINUX/MAC/UNIX
    /opt                                # LINUX/MAC/UNIX
    /sw                                 # Fink
    /opt/local                          # DarwinPorts
    /opt/csw                            # Blastwave
)

IF (WIN32)
	FIND_PATH( GLEW_INCLUDE_PATH
		NAMES
				GL/glew.h
		PATHS
				${GLEW_SEARCH_PATHS}
    PATH_SUFFIXES
        include
		DOC
				"The directory where GL/glew.h resides"
		)

	FIND_LIBRARY( GLEW_LIBRARY
		NAMES
				glew.lib
		PATHS
				${GLEW_SEARCH_PATHS}
		PATH_SUFFIXES
				lib/Release/x64
		DOC
				"The glew32s.lib library"
		)
	FIND_LIBRARY( GLEW_DLL
		NAMES
				glew32.dll
		PATHS
				${GLEW_SEARCH_PATHS}
		PATH_SUFFIXES
				bin/Release/x64
		DOC
				"The glew32.dll")
        IF(MSVC)
          FIND_PATH( GLEW_DLL
        		NAMES
        				glew.dll
        		PATHS
        				${GLEW_SEARCH_PATHS}
        		PATH_SUFFIXES
        				bin/Release/x64
        		DOC
        				"The glew32.dll")
        ENDIF()

ELSE (WIN32)
	FIND_PATH( GLEW_INCLUDE_PATH GL/glew.h
		/usr/include
		/usr/local/include
		/sw/include
		/opt/local/include
		DOC "The directory where GL/glew.h resides")
	FIND_LIBRARY( GLEW_LIBRARY
		NAMES GLEW glew
		PATHS
		/usr/lib64
		/usr/lib
		/usr/local/lib64
		/usr/local/lib
		/sw/lib
		/opt/local/lib
		DOC "The GLEW library")
ENDIF (WIN32)

IF (GLEW_INCLUDE_PATH)
	SET( GLEW_FOUND 1 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ELSE (GLEW_INCLUDE_PATH)
	SET( GLEW_FOUND 0 CACHE STRING "Set to 1 if GLEW is found, 0 otherwise")
ENDIF (GLEW_INCLUDE_PATH)

MARK_AS_ADVANCED( GLEW_FOUND )
