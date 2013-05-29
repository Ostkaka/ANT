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

# add ANT core library
add_library(${ANTTILE_TARGET_NAME} ${CORE_HEADER} ${INCLUDES} ${SOURCES})

# link dependecies
ANT_LINK_LIBRARIES(${ANTTILE_TARGET_NAME} ${DEPENDECY_LIBS} ${ANTCORE_TARGET_NAME})

# Need to link SFML separately just cause it is easier
target_link_libraries(${ANTTILE_TARGET_NAME} ${SFML_LIBRARIES})

# Set properties for antTile
SET_TARGET_PROPERTIES(${ANTTILE_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
