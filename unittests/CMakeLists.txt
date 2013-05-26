# For make-based builds, defines make target named test.
# For Visual Studio builds, defines Visual Studio project named RUN_TESTS.
project(unittests)

set(SRCROOT ${PROJECT_SOURCE_DIR})

# all source files
FILE(GLOB SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp)

# find external SFML libraries
if(WINDOWS)
  find_package(SFML REQUIRED main)
endif(WINDOWS)

# Enable ExternalProject CMake module
INCLUDE(ExternalProject)

# Set default ExternalProject root directory
SET_DIRECTORY_PROPERTIES(PROPERTIES EP_PREFIX ${CMAKE_BINARY_DIR}/dependecies)

# Add gtest
ExternalProject_Add(
    googletest
    SVN_REPOSITORY http://googletest.googlecode.com/svn/trunk/
    TIMEOUT 10
    # Force separate output paths for debug and release builds to allow easy
    # identification of correct lib in subsequent TARGET_LINK_LIBRARIES commands
    CMAKE_ARGS -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG:PATH=DebugLibs
               -DCMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE:PATH=ReleaseLibs
               -Dgtest_force_shared_crt=ON
    # Disable install step
    INSTALL_COMMAND ""
    # Wrap download, configure and build steps in a script to log output
    LOG_DOWNLOAD ON
    LOG_CONFIGURE ON
    LOG_BUILD ON)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# Specify include dir
ExternalProject_Get_Property(googletest source_dir)
INCLUDE_DIRECTORIES(${source_dir}/include)


# define the demo target
add_executable(unittest_Process ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/unittests.hpp)

# Create dependency of MainTest on googletest
ADD_DEPENDENCIES(unittest_Process googletest)

# Specify MainTest's link libraries
ExternalProject_Get_Property(googletest binary_dir)
TARGET_LINK_LIBRARIES(unittest_Process
                      debug ${binary_dir}/DebugLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES}
                      optimized ${binary_dir}/ReleaseLibs/${CMAKE_FIND_LIBRARY_PREFIXES}gtest${CMAKE_FIND_LIBRARY_SUFFIXES})


# Link dependencies
ANT_LINK_LIBRARIES(unittest_Process ${DEPENDECY_LIBS})

# This is internal linking so no need to hassle here
target_link_libraries(unittest_Process ${ANT_CORE_TARGET})
target_link_libraries(unittest_Process ${ANT_TILE_TARGET})


add_test( runUnitTests unittest_Process)