set(LIBNAME "SPICE_ES_POCO")

# Sources 
find_package(Poco REQUIRED Net) 

file(GLOB_RECURSE SRCS "*.cpp")

add_library( "${LIBNAME}" ${LIB_MODE} ${SRCS} )
set_target_properties( "${LIBNAME}"
    PROPERTIES
    OUTPUT_NAME ${LIBNAME}
    COMPILE_FLAGS "-std=c++11"
    )
    
target_include_directories( "${LIBNAME}" PUBLIC ${CMAKE_SOURCE_DIR}/SPICE_BIG "${Poco_INCLUDE_DIRS}")

if (MINGW)
target_link_libraries( "${LIBNAME}" SPICE_BIG 
    debug ${Poco_Net_LIBRARY_DEBUG} ${Poco_Foundation_LIBRARY_DEBUG}
    optimized ${Poco_Net_LIBRARY} ${Poco_Foundation_LIBRARY})
else ()
target_link_libraries( "${LIBNAME}" SPICE_BIG ${Poco_Net_LIBRARY} ${Poco_Foundation_LIBRARY})
endif ()

install(TARGETS "${LIBNAME}" DESTINATION lib)
install(DIRECTORY ./ DESTINATION include/SPICE FILES_MATCHING PATTERN "*.h")

