#----------------------------------
# target_add_dependency
#----------------------------------

macro(set_null VAR)
    if (NOT ${VAR})
        set(${VAR} "")
    endif()
endmacro()

macro(target_add_dependency TARGET DEPENDENCY)
    # Dependencies
    get_target_property(TYPE ${DEPENDENCY} TYPE)
    if (NOT (TYPE STREQUAL "UTILITY"))
        target_link_libraries(${TARGET} PUBLIC ${DEPENDENCY})
    endif()
    get_target_property(DEP ${DEPENDENCY} INTERFACE_LINK_LIBRARIES)
    set_null(DEP)
    target_link_libraries(${TARGET} PUBLIC ${DEP})
        
    # Header Search Path
    get_target_property(HEADER_PATH ${DEPENDENCY} INTERFACE_INCLUDE_DIRECTORIES)
    set_null(HEADER_PATH)
    target_include_directories(${TARGET} PUBLIC ${HEADER_PATH})
    
    # Macros
    get_target_property(MACRO ${DEPENDENCY} COMPILE_DEFINITIONS)
    set_null(MACRO)
    target_compile_definitions(${TARGET} PUBLIC ${MACRO})
endmacro(target_add_dependency)