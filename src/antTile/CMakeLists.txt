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

# link SFML
target_link_libraries(${ANTTILE_TARGET_NAME} ${SFML_LIBRARIES} antCore)

# Set properties for antTile
SET_TARGET_PROPERTIES(${ANTTILE_TARGET_NAME} PROPERTIES LINKER_LANGUAGE CXX)
