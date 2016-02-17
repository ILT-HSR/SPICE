set(LIBNAME "SPICE_Core")

# Sources - recurse to also add internal subdirectory
file(GLOB_RECURSE SRCS "*.cpp")

add_library( "${LIBNAME}" ${LIB_MODE} ${SRCS} )
set_target_properties( "${LIBNAME}"
    PROPERTIES
    OUTPUT_NAME ${LIBNAME}
    COMPILE_FLAGS "-std=c++11"
    )
    
target_include_directories( "${LIBNAME}" PUBLIC ${CMAKE_SOURCE_DIR}/SPICE_BIG)
target_link_libraries( "${LIBNAME}" SPICE_BIG)
