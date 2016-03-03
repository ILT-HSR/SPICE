set(LIBNAME "SPICE_BIG")

# Sources
file(GLOB_RECURSE SRCS "*.cpp")

add_library( "${LIBNAME}" ${LIB_MODE} ${SRCS} )
set_target_properties( "${LIBNAME}"
    PROPERTIES
    OUTPUT_NAME ${LIBNAME}
    COMPILE_FLAGS "-std=c++11"
    )
