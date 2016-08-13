# Based on the FindPhysFS.cmake scipt
# - Try to find Assimp
# Once done this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIR - the Assimp include directory
#  ASSIMP_LIBRARY - Link these to use Assimp

SET( ASSIMP_SEARCH_PATHS
    $ENV{ASSIMP}
    $ENV{ASSIMP_ROOT}                  # ASSIMP!
    ${DEPENDENCIES_ROOT}
    ./lib/Assimp
    $ENV{PROGRAMFILES}/ASSIMP           # WINDOWS
		C:/assimp
    ~/Library/Frameworks                # MAC
    /Library/Frameworks                 # MAC
    /usr/local                          # LINUX/MAC/UNIX
    /usr                                # LINUX/MAC/UNIX
    /opt                                # LINUX/MAC/UNIX
    /sw                                 # Fink
    /opt/local                          # DarwinPorts
    /opt/csw                            # Blastwave
)

FIND_PATH(ASSIMP_INCLUDE_DIR
NAMES
        assimp/mesh.h
    PATHS
        ${ASSIMP_SEARCH_PATHS}
    PATH_SUFFIXES
        include
    DOC
        "The directory where assimp/mesh.h resides"
	)
IF(WIN32)
FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
	NAMES libassimp.dll.a
	PATHS
        ${ASSIMP_SEARCH_PATHS}
	)

FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
	NAMES libassimp.dll.a
	PATHS
        ${ASSIMP_SEARCH_PATHS}
	)
FIND_LIBRARY(ASSIMP_DLL
	NAMES libassimp.dll
	PATHS
        ${ASSIMP_SEARCH_PATHS}
	)

  IF(MSVC)
      FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG
          NAMES
              assimp.lib
          PATHS
              ${ASSIMP_SEARCH_PATHS}
          PATH_SUFFIXES
              lib/Release
          DOC
              "The libassimp.a library."
      )

      FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE
          NAMES
              assimp.lib
          PATHS
              ${ASSIMP_SEARCH_PATHS}
          PATH_SUFFIXES
  	          lib/Release
          DOC
              "The libassimp.a library."
      )

      FIND_PATH(ASSIMP_DLL
          NAMES
              assimp.dll
          PATHS
              ${ASSIMP_SEARCH_PATHS}
          PATH_SUFFIXES
              bin/Release
          DOC
              "The assimp.dll library."
      )
  ENDIF(MSVC)
ELSE(WIN32)
FIND_LIBRARY(LIBASSIMP
  NAMES
  ${ASSIMP}
  PATHS
  ${ASSIMP_DEPS_LIB_DIR}
  ${PROJECT_SOURCE_DIR}/libraries/assimp/lib
  ENV LD_LIBRARY_PATH
  ENV LIBRARY_PATH
  /usr/lib
  /usr/local/lib
  /opt/local/lib
  NO_DEFAULT_PATH
)
ENDIF()
SET (ASSIMP_LIBRARIES
  ${LIBASSIMP}
)
SET(ASSIMP_FOUND "NO")
IF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_DEBUG AND ASSIMP_LIBRARY_RELEASE)
	SET(ASSIMP_FOUND "YES")
	SET(ASSIMP_LIBRARY debug ${ASSIMP_LIBRARY_DEBUG} optimized ${ASSIMP_LIBRARY_RELEASE})
ENDIF(ASSIMP_INCLUDE_DIR AND ASSIMP_LIBRARY_DEBUG AND ASSIMP_LIBRARY_RELEASE)

if(ASSIMP_DEBUG)
	message(STATUS "assimp inc: ${ASSIMP_INCLUDE_DIR}")
	message(STATUS "assimp lib: ${ASSIMP_LIBRARY}")
ENDIF(ASSIMP_DEBUG)

if(AssImp_FIND_REQUIRED AND NOT (ASSIMP_LIBRARY AND ASSIMP_INCLUDE_DIR))
	message(FATAL_ERROR "Could not find assimp")
ENDIF(AssImp_FIND_REQUIRED AND NOT (ASSIMP_LIBRARY AND ASSIMP_INCLUDE_DIR))

mark_as_advanced(ASSIMP_LIBRARY_DEBUG ASSIMP_LIBRARY_RELEASE ASSIMP_INCLUDE_DIR)
