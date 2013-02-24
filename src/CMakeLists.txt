# include the ANTspecific macros
include(${PROJECT_SOURCE_DIR}/cmake/macros.cmake)

# add the ANT sources path
include_directories(${PROJECT_SOURCE_DIR}/src)

# set the output directory for ANT libraries 
set(LIBRARY_OUTPUT_PATH "${PROJECT_BINARY_DIR}/lib")

# set the source directory for include and source files
set(ANT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set(ANT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# ANT needs to export symbols when build so define the export flag
add_definitions(-DANT_EXPORTS)

# add the modules subdirectories
add_subdirectory(ant)

add_subdirectory(antTile)

# install Config header include file
install(FILES ${ANT_INCLUDE_DIR}/ant_config.hpp
        DESTINATION include/ant
        COMPONENT dev)
