# CMake master file for ANT
#

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.8)

# Project name
project(ant)

# Include the macros file for this project
include(${PROJECT_SOURCE_DIR}/cmake/macros.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/HandleDependencies.cmake)

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules/)

# Build options
set_option(BUILD_EXAMPLES TRUE BOOL "If example applications should be build" )

# setup version numbers - THis is not really needed
set(ANT_VERSION_MAJOR 0)
set(ANT_VERSION_MINOR 22)
set(ANT_VERSION_PATCH 4)
set(ANT_VERSION "${ANT_VERSION_MAJOR}.${ANT_VERSION_MINOR}.${ANT_VERSION_PATCH}")

# Set dependencies dir
SET( ENV{ANT_DEPENDENCIES_DIR} "${FULL_DEPENDECIES_DIR}")  # Set by handle HandleDependencies
SET( ANT_DEPENDENCIES_DIR $ENV{ANT_DEPENDENCIES_DIR})

set( ANT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set( ANT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Set project targets
SET( ANTCORE_TARGET_NAME "antCore")
SET( ANTTILE_TARGET_NAME "antTile")

# find external SFML libraries
find_package(SFML REQUIRED audio graphics window system)

# add include paths of external SFML libraries
IF(SFML_FOUND)
	message("Checking for dependencies")
include_directories(${SFML_INCLUDE_DIR})
	message("SFML: FOUND")
else()
	message(FATAL_ERROR "SFML: NOT FOUND")
endif(SFML_FOUND)

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)

message("dependencies: "  ${ANT_DEPENDENCIES_DIR})

#find directX
find_package(DirectX)
if(DirectX_D3D11_FOUND)
	message("DIRECTX: FOUND")
else()
	message(FATAL_ERROR "DIRECTX: NOT FOUND")
endif(DirectX_D3D11_FOUND)

# find zlib
set(ENV{ZLIB_DIR} ${ANT_DEPENDENCIES_DIR})
find_package(ZLIB)
if(ZLIB_FOUND)
	message("ZLIB: FOUND")
else()
	message(FATAL_ERROR "ZLIB: NOT FOUND" ${ZLIB_INCLUDE_DIR} " :: " ${ZLIB_LIBRARY})
endif(ZLIB_FOUND)
include_directories(${ZLIB_INCLUDE_DIR})

# find TinyXml
set(ENV{TINYXML_DIR} ${ANT_DEPENDENCIES_DIR})
find_package(TINYXML)
if(TINYXML_FOUND)
	message("TinyXml: FOUND")
else()
	message(FATAL_ERROR "TinyXml: NOT FOUND")
endif(TINYXML_FOUND)

# Do it for OGG
set(ENV{OGGDIR} ${ANT_DEPENDENCIES_DIR})
find_package(OGG)
if(OGG_FOUND)
	message("OGG: FOUND")
else()
	message(FATAL_ERROR "OGG: NOT FOUND")
endif(OGG_FOUND)

# add the ANT subdirectory
message("Configuring ANT...")
add_subdirectory(src)

#find bullet

#find fastdelegate

#find lua

#find DXUT11

# add the examples subdirectory
if(BUILD_EXAMPLES)
	message("Configuring examples...")
    add_subdirectory(examples)
endif()


