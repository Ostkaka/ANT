project(example_basictest)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
FILE( GLOB SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# define the demo target
add_executable(example_basictest ${SOURCE})

# link dependecies
target_link_libraries(example_basictest ${ANT_LIBS})

# properties
SET_PROPERTY(TARGET example_basictest PROPERTY FOLDER "examples")

# install target
install(TARGETS example_basictest RUNTIME DESTINATION examples/basictest COMPONENT examples)

