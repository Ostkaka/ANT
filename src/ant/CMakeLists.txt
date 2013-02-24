#Add the core library include header
set(CORE_HEADER ${ANT_INCLUDE_DIR}/core_include.hpp)


MESSAGE ( ${CORE_HEADER})


# core library additional include files
FILE( GLOB_RECURSE 
	INCLUDES
    ${ANT_INCLUDE_DIR}/ant/*.hpp
)

FILE( GLOB_RECURSE
	SOURCES
	${ANT_SOURCE_DIR}/ant/*.cpp
) 

# find external SFML libraries
find_package(SFML REQUIRED audio graphics window system)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# add ANT library
add_library(antCore ${CORE_HEADER} ${INCLUDES} ${SOURCES} ${ANT_INCLUDE_DIR}/ant_config.hpp)

# link SFML
target_link_libraries(antCore ${SFML_LIBRARIES})

# Set properties for antCore
SET_TARGET_PROPERTIES(antCore PROPERTIES LINKER_LANGUAGE CXX)
