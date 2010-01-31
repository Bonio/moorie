# - Find mash
# Find the native MHASH includes and library
#
#  MHASH_INCLUDE_DIR - where to find mhash.h, etc.
#  MHASH_LIBRARIES   - List of libraries when using mhash.
#  MHASH_FOUND       - True if mhash found.


IF (MHASH_INCLUDE_DIR)
  # Already in cache, be silent
  SET(MHASH_FIND_QUIETLY TRUE)
ENDIF (MHASH_INCLUDE_DIR)

FIND_PATH(MHASH_INCLUDE_DIR mhash.h)

SET(MHASH_NAMES mhash)
FIND_LIBRARY(MHASH_LIBRARY NAMES ${MHASH_NAMES} )

# handle the QUIETLY and REQUIRED arguments and set MHASH_FOUND to TRUE if
# all listed variables are TRUE
INCLUDE(FindPackageHandleStandardArgs)
FIND_PACKAGE_HANDLE_STANDARD_ARGS(MHASH DEFAULT_MSG MHASH_LIBRARY MHASH_INCLUDE_DIR)

IF(MHASH_FOUND)
  SET( MHASH_LIBRARIES ${MHASH_LIBRARY} )
  MESSAGE(STATUS "Found mhash: ${MHASH_LIBRARY}")
ELSE(MHASH_FOUND)
  SET( MHASH_LIBRARIES )
  MESSAGE(FATAL_ERROR "Could not find mhash")
ENDIF(MHASH_FOUND)
MARK_AS_ADVANCED( MHASH_LIBRARY MHASH_INCLUDE_DIR )
