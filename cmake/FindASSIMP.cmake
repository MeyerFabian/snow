# Based on the FindPhysFS.cmake scipt
# - Try to find Assimp
# Once done this will define
#
#  ASSIMP_FOUND - system has Assimp
#  ASSIMP_INCLUDE_DIR - the Assimp include directory
#  ASSIMP_LIBRARY - Link these to use Assimp

set(_assimp_INCLUDE_SEARCH_DIRS_SYSTEM
    C:/assimp/include
    "$ENV{ProgramFiles}/assimp/include"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/include
  )

set(_assimp_LIB_SEARCH_DIRS_SYSTEM
    C:/assimp/lib
    "$ENV{ProgramFiles}/assimp/lib"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/lib
  )

set(_assimp_BIN_SEARCH_DIRS_SYSTEM
    C:/assimp/bin
    "$ENV{ProgramFiles}/assimp/bin"
    "$ENV{ProgramFiles}/assimp"
    /sw/local/bin
  )
FIND_PATH(ASSIMP_INCLUDE_DIR assimp/ai_assert.h
	$ENV{ASSIMPSDIR}/include
	$ENV{ASSIMPSDIR}
	$ENV{ASSIMPSDIR}/..
	~/Library/Frameworks/AssImp.framework/Headers
	/Library/Frameworks/AssImp.framework/Headers
	/usr/local/include/assimp
	/usr/local/include
	/usr/include/assimp
	/usr/include
	/sw/include/assimp # Fink
	/sw/include
	/opt/local/include/assimp # DarwinPorts
	/opt/local/include
	/opt/csw/include/assimp # Blastwave
	/opt/csw/include
	/opt/include/assimp
	/opt/include
	${_assimp_INCLUDE_SEARCH_DIRS_SYSTEM}
	)
IF(WIN32)
FIND_LIBRARY(ASSIMP_LIBRARY_DEBUG 
	NAMES libassimp.dll.a
	PATHS
	$ENV{ASSIMPSDIR}/lib
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	${_assimp_LIB_SEARCH_DIRS_SYSTEM}
	)

FIND_LIBRARY(ASSIMP_LIBRARY_RELEASE 
	NAMES libassimp.dll.a
	PATHS
	$ENV{ASSIMPSDIR}/lib
	/usr/local/lib
	/usr/lib
	/sw/lib
	/opt/local/lib
	/opt/csw/lib
	/opt/lib
	${_assimp_LIB_SEARCH_DIRS_SYSTEM}
	)
FIND_LIBRARY(ASSIMP_DLL
	NAMES libassimp.dll
	PATHS
	$ENV{ASSIMPSDIR}/bin
	/usr/local/bin
	/usr/bin
	/sw/bin
	/opt/local/bin
	/opt/csw/bin
	/opt/bin
	${_assimp_BIN_SEARCH_DIRS_SYSTEM}
	)
ELSE()	
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