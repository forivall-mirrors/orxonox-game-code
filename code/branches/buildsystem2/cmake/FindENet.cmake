# - Try to find enet
# Once done this will define
#
#  ENET_FOUND - system has enet
#  ENet_INCLUDE_DIR - the enet include directory
#  ENet_LIBRARIES - the libraries needed to use enet
#
# $ENETDIR is an environment variable used for finding enet.
#
#  Borrowed from The Mana World
#  http://themanaworld.org/
#
# Several changes and additions by Fabian 'x3n' Landau
# Lots of simplifications by Adrian Friedli
#                 > www.orxonox.net <

FIND_PATH(ENet_INCLUDE_DIR enet/enet.h
    PATHS
    $ENV{ENETDIR}
    /usr/local
    /usr
    PATH_SUFFIXES include
)
FIND_LIBRARY(ENet_LIBRARY_OPTIMIZED
    NAMES enet
    PATHS
    $ENV{ENETDIR}
    /usr/local
    /usr
    PATH_SUFFIXES lib
)
FIND_LIBRARY(ENet_LIBRARY_DEBUG
    NAMES enet_${LIBRARY_DEBUG_POSTFIX}
    PATHS
    $ENV{ENETDIR}
    /usr/local
    /usr
    PATH_SUFFIXES lib
)

# handle the QUIETLY and REQUIRED arguments and set ENET_FOUND to TRUE if
# all listed variables are TRUE
FIND_PACKAGE_HANDLE_STANDARD_ARGS(ENet DEFAULT_MSG
    ENet_LIBRARY_OPTIMIZED
    ENet_INCLUDE_DIR
)

# Set optimized and debug libraries
HandleLibraryTypes(ENet ${ENet_ADDITIONAL_LIBRARIES})

MARK_AS_ADVANCED(
    ENet_LIBRARY
    ENet_LIBRARY_OPTIMIZED
    ENet_LIBRARY_DEBUG
    ENet_INCLUDE_DIR
)
