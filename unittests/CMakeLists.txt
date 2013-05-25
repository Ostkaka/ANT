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

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR})

# define the demo target
add_executable(unittest_Process ${PROJECT_SOURCE_DIR}/main.cpp ${PROJECT_SOURCE_DIR}/unittests.hpp)
target_link_libraries(unittest_Process ${DEPENDECY_LIBS})
target_link_libraries(unittest_Process antCore)
target_link_libraries(unittest_Process antTile)


add_test( runUnitTests unittest_Process)