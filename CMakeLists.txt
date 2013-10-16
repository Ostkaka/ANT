# CMake master file for ANT
#

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.8)

# Project name
project(ant)

# Include the macros file for this project
include(${PROJECT_SOURCE_DIR}/cmake/antMacros.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/HandleDependencies.cmake)

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
set(EP_PREFIX ${CMAKE_BINARY_DIR}/dependecies)
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${EP_PREFIX})

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules/)

# Build options
option(BUILD_TILE "If unit tests should be buildt" ON)
option(BUILD_EXAMPLES "If example applications should be build" ON)
option(BUILD_UNITTESTS "If unit tests should be buildt" ON)

# setup version numbers - THis is not really needed
set(ANT_VERSION_MAJOR 0)
set(ANT_VERSION_MINOR 22)
set(ANT_VERSION_PATCH 4)
set(ANT_VERSION "${ANT_VERSION_MAJOR}.${ANT_VERSION_MINOR}.${ANT_VERSION_PATCH}")

# Set directory paths for data
set( ANT_DATA_PATH "${PROJECT_SOURCE_DIR}/data")
set( ANT_ROOT_PATH "${PROJECT_SOURCE_DIR}")

# Properties
SET_PROPERTY(GLOBAL PROPERTY USE_FOLDERS ON)

# Set dependencies dir
SET( ENV{ANT_DEPENDENCIES_DIR} ${FULL_DEPENDENCIES_DIR})  # Set by handle HandleDependencies
SET( ANT_DEPENDENCIES_DIR $ENV{ANT_DEPENDENCIES_DIR})

set( ANT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set( ANT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Offer the user the choice of overriding the installation directories
set(INSTALL_LIB_DIR lib PATH "Installation directory for libraries")
set(INSTALL_BIN_DIR bin PATH "Installation directory for executables")
set(INSTALL_DATA_DIR bin PATH "Installation directory for data")
set(INSTALL_INCLUDE_DIR include PATH  "Installation directory for header files")

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
  "${PROJECT_SOURCE_DIR}/include/ant_config.hpp.in"
  "${PROJECT_BINARY_DIR}/include/ant_config.hpp"
  )

# Set project targets
SET( ANTCORE_TARGET_NAME "antCore")
SET( ANTTILE_TARGET_NAME "antTile")

set(ANT_LIBS ${ANT_LIBS} ${ANTCORE_TARGET_NAME} ${ANTTILE_TARGET_NAME})

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)

# Visual studio specific
if (MSVC)
	# This is ugly but set the compiler flags for RelDebInfo -- THIS CANNOT BE SET?!?
	set ( CMAKE_CXX_FLAGS_RELWITHDEBINFO "/MD /Zi /Od" ) 

	add_definitions(-DUNICODE -D_UNICODE)
	set(CMAKE_DEBUG_POSTFIX "d")
    add_definitions(-D_CRT_SECURE_NO_DEPRECATE)
    add_definitions(-D_CRT_NONSTDC_NO_DEPRECATE)
endif(MSVC)



# add include paths of external SFML libraries
MESSAGE("Using dependecies dir: " ${ANT_DEPENDENCIES_DIR})

set(DEPENDECY_LIBS ${DEPENDECY_LIBS})

# TinyXML
set(TINYXML_TARGET_NAME "TinyXml")
include_directories(${PROJECT_SOURCE_DIR}/tinyxml)
add_subdirectory(tinyxml)

#luaplusplus
set( LUAPLUSPLUS_TARGET_NAME "luaplusplus" )
include_directories(${PROJECT_SOURCE_DIR}/luaplusplus)
add_subdirectory(luaplusplus)

# find external SFML libraries
#set(SFML_STATIC_LIBRARIES TRUE)
find_package(SFML REQUIRED audio graphics window system)
IF(SFML_FOUND)
	include_directories(${SFML_INCLUDE_DIR})
	message("SFML: FOUND")
else()
	message(FATAL_ERROR "SFML: NOT FOUND")
endif(SFML_FOUND)

#find directX
find_package(DirectX)
if(DirectX_D3D11_FOUND)
	message("DIRECTX: FOUND")
	include_directories(${DirectX_D3D11_INCLUDE_DIR})
	# Just assume that dxerr has been found too
	set(DEPENDECY_LIBS ${DEPENDECY_LIBS} 
					${DirectX_D3DX11_LIBRARY}
					${DirectX_D3DX9_LIBRARY} 
					${DirectX_DXERR_LIBRARY} 
					${DirectX_DXGUID_LIBRARY}
					${DirectX_COMPILER_LIBRARY}
					)
else()
	message(FATAL_ERROR "DIRECTX: NOT FOUND")
endif(DirectX_D3D11_FOUND)

# find zlib
set(ENV{ZLIB_DIR} ${ANT_DEPENDENCIES_DIR})
find_package(ZLIB)
if(ZLIB_FOUND)
	message("ZLIB: FOUND")
	include_directories(${ZLIB_INCLUDE_DIR})
	set(DEPENDECY_LIBS ${DEPENDECY_LIBS} ${ZLIB_LIBRARY})
else()
	message(FATAL_ERROR "ZLIB: NOT FOUND" ${ZLIB_INCLUDE_DIR} " :: " ${ZLIB_LIBRARY})
endif(ZLIB_FOUND)

# Do it for OGG
set(ENV{OGGDIR} ${ANT_DEPENDENCIES_DIR})
find_package(OGG)
if(OGG_FOUND)
	message("OGG: FOUND")
	include_directories(${OGGDIR_INCLUDE_DIR})
	set(DEPENDECY_LIBS ${DEPENDECY_LIBS} ${OGGDIR_LIBRARY})
else()
	message(FATAL_ERROR "OGG: NOT FOUND")
endif(OGG_FOUND)

# find bullet
set(BULLET_ROOT ${ANT_DEPENDENCIES_DIR})
find_package(BULLET COMPONENTS BulletDynamics BulletCollision LinearMath BulletMath REQUIRED)

if(BULLET_FOUND)
	message("BULLET: FOUND")
	include_directories(${BULLET_INCLUDE_DIR})
	set(DEPENDECY_LIBS ${DEPENDECY_LIBS} ${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "BULLET: NOT FOUND")
endif(BULLET_FOUND)

#find fastdelegate
set(ENV{FASTDELEGATE_DIR} ${ANT_DEPENDENCIES_DIR})
find_package(FASTDELEGATE)
if(FASTDELEGATE_FOUND)
	message("FASTDELEGATET: FOUND")
	include_directories(${FASTDELEGATE_INCLUDE_DIR})
else()
	message(FATAL_ERROR "FASTDELEGATE: NOT FOUND")
endif(FASTDELEGATE_FOUND)

# Add Box2d
ExternalProject_Add(
    box2d
    SVN_REPOSITORY http://box2d.googlecode.com/svn/trunk/Box2D/Box2D
    TIMEOUT 10  
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

SET_PROPERTY(TARGET box2d PROPERTY FOLDER "external")

# Get include directory of external project
ExternalProject_Get_Property(box2d source_dir)
INCLUDE_DIRECTORIES(${source_dir}/..)
MESSAGE(" fdjgid " ${source_dir}/..)

# Get binary directory of external project
ExternalProject_Get_Property(box2d binary_dir)
set(DEPENDECY_LIBS ${DEPENDECY_LIBS} ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}box2d${CMAKE_FIND_LIBRARY_SUFFIXES} ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}box2d${CMAKE_FIND_LIBRARY_SUFFIXES})

# add the ANT subdirectory
message("Configuring ANT...")

add_subdirectory(src)

# add the examples subdirectory
if(BUILD_EXAMPLES)
	message("Configuring examples...")
    add_subdirectory(examples)
endif()

# add the unittest subdirectory
if(BUILD_UNITTESTS)
	message("Configuring unittests...")
    add_subdirectory(unittests)
endif()