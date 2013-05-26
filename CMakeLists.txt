# CMake master file for ANT
#

# Make sure they are using a recent version of CMake
cmake_minimum_required(VERSION 2.8)

# Project name
project(ant)

# Include the macros file for this project
include(${PROJECT_SOURCE_DIR}/cmake/antMacros.cmake)
include(${PROJECT_SOURCE_DIR}/cmake/HandleDependencies.cmake)

# define the path of our additional CMake modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake/Modules/)

# Build options
option(BUILD_EXAMPLES "If example applications should be build" ON)
option(BUILD_UNITTESTS "If unit tests should be buildt" ON)

# setup version numbers - THis is not really needed
set(ANT_VERSION_MAJOR 0)
set(ANT_VERSION_MINOR 22)
set(ANT_VERSION_PATCH 4)
set(ANT_VERSION "${ANT_VERSION_MAJOR}.${ANT_VERSION_MINOR}.${ANT_VERSION_PATCH}")

# Set dependencies dir
SET( ENV{ANT_DEPENDENCIES_DIR} ${FULL_DEPENDENCIES_DIR})  # Set by handle HandleDependencies
SET( ANT_DEPENDENCIES_DIR $ENV{ANT_DEPENDENCIES_DIR})

set( ANT_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
set( ANT_SOURCE_DIR ${PROJECT_SOURCE_DIR}/src)

# Set project targets
SET( ANTCORE_TARGET_NAME "antCore")
SET( ANTTILE_TARGET_NAME "antTile")

include_directories(
  "${PROJECT_SOURCE_DIR}/include"
)
# Visual studio specific
if (MSVC)
	add_definitions(-DUNICODE -D_UNICODE)
endif(MSVC)

# add include paths of external SFML libraries
MESSAGE("Using dependecies dir: " ${ANT_DEPENDENCIES_DIR})

# find external SFML libraries
find_package(SFML REQUIRED audio graphics window system)

include_directories(${SFML_INCLUDE_DIR})

set(DEPENDECY_LIBS ${SFML_MAIN_LIBRARY})

# find SFML
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
include_directories(${ZLIB_INCLUDE_DIR})

# find TinyXml
set(ENV{TINYXML_DIR} ${ANT_DEPENDENCIES_DIR})
find_package(TINYXML)
if(TINYXML_FOUND)
	message("TinyXml: FOUND")
	include_directories(${TINYXML_INCLUDE_DIR})
	set(DEPENDECY_LIBS ${DEPENDECY_LIBS} ${TINYXML_LIBRARY})
else()
	message(FATAL_ERROR "TinyXml: NOT FOUND")
endif(TINYXML_FOUND)

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

#find bullet
set(BULLET_ROOT ${ANT_DEPENDENCIES_DIR})
find_package(BULLET COMPONENTS BulletDynamics BulletCollision LinearMath BulletMath REQUIRED)

if(BULLET_FOUND)
	message("BULLET: FOUND")
	include_directories(${BULLET_INCLUDE_DIR})
	set(DEPENDECY_LIBS ${DEPENDECY_LIBS} ${BULLET_LIBRARIES})
else()
	message(FATAL_ERROR "BULLET: NOT FOUND")
endif(BULLET_FOUND)

#find lua
#set(ENV{LUA_DIR} ${ANT_DEPENDENCIES_DIR})
#find_package(Lua51)
#if(LUA_FOUND)
#	message("LUA: FOUND")
#else()
#	message(FATAL_ERROR "LUA: NOT FOUND")
#endif(LUA_FOUND)

#find fastdelegate
set(ENV{FASTDELEGATE_DIR} ${ANT_DEPENDENCIES_DIR})
find_package(FASTDELEGATE)
if(FASTDELEGATE_FOUND)
	message("FASTDELEGATET: FOUND")
	include_directories(${FASTDELEGATE_INCLUDE_DIR})
else()
	message(FATAL_ERROR "FASTDELEGATE: NOT FOUND")
endif(FASTDELEGATE_FOUND)

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