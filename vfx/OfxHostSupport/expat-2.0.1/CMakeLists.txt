CMAKE_MINIMUM_REQUIRED( VERSION 2.6.0 FATAL_ERROR)

#PROJECT SETUP
PROJECT( expat)
SET( VERSION_MAJOR "2")
SET( VERSION_MINOR "0")
SET( VERSION_PATCH "1")
SET( VERSION_SUFFIX "")
SET( VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}${VERSION_SUFFIX})

#disable in source builds
IF( ${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
    MESSAGE(FATAL_ERROR "CMake generation for expat is not allowed within the source directory!")
ENDIF(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})

# compiler options
IF( NOT CMAKE_BUILD_TYPE)
    SET( CMAKE_BUILD_TYPE Release)
ENDIF( NOT CMAKE_BUILD_TYPE)

#IF( MSVC)
#    ADD_DEFINITIONS( -D_CRT_SECURE_NO_DEPRECATE)
#    ADD_DEFINITIONS( -D_SCL_SECURE_NO_DEPRECATE)
#ENDIF( MSVC)

ADD_DEFINITIONS( -DHAVE_MEMMOVE)

MESSAGE( "expat ${VERSION} will be built to install into ${CMAKE_INSTALL_PREFIX}")

SET( EXPAT_INCLUDES lib)
SET( EXPAT_SOURCES  lib/xmlparse.c
		    lib/xmlrole.c
		    lib/xmltok.c
		    lib/xmltok_ns.c
		    )

INCLUDE_DIRECTORIES( ${EXPAT_INCLUDES})

ADD_LIBRARY( expat SHARED ${EXPAT_SOURCES})
SET_TARGET_PROPERTIES( expat PROPERTIES VERSION 2.0.1 SOVERSION 2)

