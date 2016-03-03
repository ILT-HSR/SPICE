set(LIBNAME "SPICE_XML_POCO")

# Sources 
find_package(Poco REQUIRED XML) 

file(GLOB_RECURSE SRCS "*.cpp")

add_library( "${LIBNAME}" ${LIB_MODE} ${SRCS} )
set_target_properties( "${LIBNAME}"
    PROPERTIES
    OUTPUT_NAME ${LIBNAME}
    COMPILE_FLAGS "-std=c++11"
    )
    
target_include_directories( "${LIBNAME}" PUBLIC ${CMAKE_SOURCE_DIR}/SPICE_BIG "${Poco_INCLUDE_DIRS}")

target_link_libraries( "${LIBNAME}" SPICE_BIG 
    debug ${Poco_XML_LIBRARY_DEBUG} ${Poco_Foundation_LIBRARY_DEBUG}
    optimized ${Poco_XML_LIBRARY} ${Poco_Foundation_LIBRARY})
