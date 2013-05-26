#
# Link libraires macro
#
MACRO(ANT_LINK_LIBRARIES TARGET)
	FOREACH(libname ${ARGN})
		TARGET_LINK_LIBRARIES(${TARGET} optimized "${libname}" debug "${libname}${CMAKE_DEBUG_POSTFIX}")
	ENDFOREACH(libname)
ENDMACRO(ANT_LINK_LIBRARIES TARGET)
