CMAKE_MINIMUM_REQUIRED (VERSION 2.8)
PROJECT (roptool)

###########################################################
##
## Project requires C++11
IF (CMAKE_COMPILER_IS_GNUCXX)
    SET (CMAKE_CXX_FLAGS "-std=c++11 -fpermissive -static-libstdc++")
ENDIF (CMAKE_COMPILER_IS_GNUCXX)

###########################################################
##
## Requires boost libraries to function
SET(Boost_USE_STATIC_LIBS   ON)
FIND_PACKAGE (Boost REQUIRED COMPONENTS program_options filesystem system regex REQUIRED)

INCLUDE_DIRECTORIES (${Boost_INCLUDE_DIRS} ${CMAKE_CURRENT_LIST_DIR}/tinyxml2)
LINK_DIRECTORIES (${Boost_LIBRARY_DIRS} ${CMAKE_CURRENT_LIST_DIR}/tinyxml2)

# we want to build static lib
SET(BUILD_STATIC_LIBS ON)
ADD_SUBDIRECTORY (tinyxml2)

FILE (GLOB ROPTOOL_SOURCES "src/*.cpp")                                                                              
ADD_EXECUTABLE (roptool ${ROPTOOL_SOURCES})
TARGET_LINK_LIBRARIES (roptool ${Boost_LIBRARIES} tinyxml2static) 

add_dependencies(roptool tinyxml2)
