
# This file handles the detection of dependecies
# This assumes that the dependencies are in the depedencies directory

SET(DEPENDENCIES_ROOT ${PROJECT_BINARY_DIR})

# Convert it to the absolute path to not use relative
GET_FILENAME_COMPONENT(DEPENDENCIES_ROOT ${DEPENDENCIES_ROOT} ABSOLUTE)

SET( FULL_DEPENDENCIES_DIR ${PROJECT_BINARY_DIR}/dependecies)

# Add to native file path. Whatevah
FILE(TO_NATIVE_PATH ${FULL_DEPENDENCIES_DIR} FULL_DEPENDENCIES_DIR) 