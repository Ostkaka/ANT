#Add the core library include header
set(CORE_HEADER ${ANT_INCLUDE_DIR}/tile_include.hpp)

# core library additional include files
FILE( GLOB_RECURSE 
	INCLUDES
    ${ANT_INCLUDE_DIR}/antTile/*.h
)

FILE( GLOB_RECURSE
	SOURCES
	${ANT_SOURCE_DIR}/antTile/*.cpp
) 

# find external SFML libraries
find_package(SFML REQUIRED audio graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# add MGE library
add_library(antTile ${CORE_HEADER} ${INCLUDES} ${SOURCES})

# link SFML
target_link_libraries(antTile ${SFML_LIBRARIES} antCore)

# Set properties for antTile
SET_TARGET_PROPERTIES(antTile PROPERTIES LINKER_LANGUAGE CXX)
