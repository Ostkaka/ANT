project(example_sfmlapp)

set(SRCROOT ${PROJECT_SOURCE_DIR}/src)

# all source files
FILE( GLOB SOURCE ${SRCROOT}/*.cpp ${SRCROOT}/*.hpp)

# add include paths of external libraries
include_directories(${SFML_INCLUDE_DIR})

# define the demo target
add_executable(example_sfmlapp ${SOURCE})

# link dependecies
target_link_libraries(example_sfmlapp ${ANT_LIBS})

# properties
SET_PROPERTY(TARGET example_sfmlapp PROPERTY FOLDER "examples")

# install target
install(TARGETS example_sfmlapp RUNTIME DESTINATION examples/example_sfmlapp COMPONENT examples)

