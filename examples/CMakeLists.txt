# Examples will be importing from GQE so we need to remove this definition
remove_definitions(-DANT_EXPORTS)

add_subdirectory(basictest)
add_subdirectory(example_sfmlapp)