# tests_common.cmake
# Common test infrastructure functions for all test types
# Provides functions for gathering test files and their dependencies

# djinterp_gather_test_files
#   function: gathers test files matching a pattern and prepends core dependencies
# 
# Parameters:
#   OUTPUT_VAR:     name of variable to store the resulting file list
#   PATTERN:        glob pattern for test files (e.g., "dfile_tests_sa*.c")
#   PREPEND_CORE:   whether to prepend djinterp.c (ON by default)
# 
# Example:
#   djinterp_gather_test_files(
#       OUTPUT_VAR MY_TEST_FILES
#       PATTERN "dfile_tests_sa*.c"
#       PREPEND_CORE ON
#   )
function(djinterp_gather_test_files)
    # parse arguments
    set(options "")
    set(oneValueArgs OUTPUT_VAR PATTERN PREPEND_CORE)
    set(multiValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # validate required arguments
    if(NOT DEFINED ARG_OUTPUT_VAR)
        message(FATAL_ERROR "djinterp_gather_test_files: OUTPUT_VAR is required")
    endif()
    if(NOT DEFINED ARG_PATTERN)
        message(FATAL_ERROR "djinterp_gather_test_files: PATTERN is required")
    endif()
    
    # default PREPEND_CORE to ON if not specified
    if(NOT DEFINED ARG_PREPEND_CORE)
        set(ARG_PREPEND_CORE ON)
    endif()
    
    # gather test files matching pattern
    file(GLOB TEST_FILES "${TEST_DIR}/${ARG_PATTERN}")
    
    # build result list
    set(RESULT_LIST "")
    
    # prepend djinterp.c if requested
    if(ARG_PREPEND_CORE)
        list(APPEND RESULT_LIST "${SOURCE_DIR}/djinterp.c")
    endif()
    
    # add the gathered test files
    list(APPEND RESULT_LIST ${TEST_FILES})
    
    # return via OUTPUT_VAR
    set(${ARG_OUTPUT_VAR} ${RESULT_LIST} PARENT_SCOPE)
endfunction()


# djinterp_add_module_dependencies
#   function: adds module source files to a list for specific modules
# 
# Parameters:
#   OUTPUT_VAR: name of variable to store/append the resulting file list
#   MODULES:    list of module names (e.g., dfile dmemory string_fn)
# 
# Example:
#   djinterp_add_module_dependencies(
#       OUTPUT_VAR MY_SOURCES
#       MODULES dfile dmemory string_fn
#   )
function(djinterp_add_module_dependencies)
    # parse arguments
    set(options "")
    set(oneValueArgs OUTPUT_VAR)
    set(multiValueArgs MODULES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # validate required arguments
    if(NOT DEFINED ARG_OUTPUT_VAR)
        message(FATAL_ERROR "djinterp_add_module_dependencies: OUTPUT_VAR is required")
    endif()
    if(NOT DEFINED ARG_MODULES)
        message(FATAL_ERROR "djinterp_add_module_dependencies: MODULES is required")
    endif()
    
    # start with existing value if variable is already defined
    if(DEFINED ${ARG_OUTPUT_VAR})
        set(RESULT_LIST ${${ARG_OUTPUT_VAR}})
    else()
        set(RESULT_LIST "")
    endif()
    
    # add each module's source file
    foreach(MODULE ${ARG_MODULES})
        list(APPEND RESULT_LIST "${SOURCE_DIR}/${MODULE}.c")
    endforeach()
    
    # return via OUTPUT_VAR
    set(${ARG_OUTPUT_VAR} ${RESULT_LIST} PARENT_SCOPE)
endfunction()


# djinterp_create_module_test_executable
#   function: creates a test executable with automatic dependency resolution
# 
# Parameters:
#   TARGET:         name of the test executable target
#   TEST_PATTERN:   glob pattern for test files (e.g., "dfile_tests_sa*.c")
#   MODULES:        list of module dependencies (e.g., dfile dmemory string_fn)
#   LINK_LIBRARIES: additional libraries to link against (optional)
#   PREPEND_CORE:   whether to prepend djinterp.c (ON by default)
# 
# Example:
#   djinterp_create_module_test_executable(
#       TARGET djinterp-c-dfile-tests-sa-${PLATFORM_ARCH}
#       TEST_PATTERN "dfile_tests_sa*.c"
#       MODULES dmemory string_fn
#       LINK_LIBRARIES dfile
#   )
function(djinterp_create_module_test_executable)
    # parse arguments
    set(options "")
    set(oneValueArgs TARGET TEST_PATTERN PREPEND_CORE)
    set(multiValueArgs MODULES LINK_LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # validate required arguments
    if(NOT DEFINED ARG_TARGET)
        message(FATAL_ERROR "djinterp_create_module_test_executable: TARGET is required")
    endif()
    if(NOT DEFINED ARG_TEST_PATTERN)
        message(FATAL_ERROR "djinterp_create_module_test_executable: TEST_PATTERN is required")
    endif()
    
    # default PREPEND_CORE to ON if not specified
    if(NOT DEFINED ARG_PREPEND_CORE)
        set(ARG_PREPEND_CORE ON)
    endif()
    
    # gather test files
    djinterp_gather_test_files(
        OUTPUT_VAR TEST_SOURCES
        PATTERN "${ARG_TEST_PATTERN}"
        PREPEND_CORE ${ARG_PREPEND_CORE}
    )
    
    # add module dependencies if specified
    if(DEFINED ARG_MODULES)
        djinterp_add_module_dependencies(
            OUTPUT_VAR TEST_SOURCES
            MODULES ${ARG_MODULES}
        )
    endif()
    
    # create the executable
    add_executable(${ARG_TARGET} ${TEST_SOURCES})
    
    # set include directories
    target_include_directories(${ARG_TARGET} 
        PRIVATE 
        ${INCLUDE_DIR} 
        ${TEST_DIR}
    )
    
    # link libraries if specified
    if(DEFINED ARG_LINK_LIBRARIES)
        target_link_libraries(${ARG_TARGET} PRIVATE ${ARG_LINK_LIBRARIES})
    endif()
endfunction()


# Module dependency chains
# These define which modules depend on which other modules

# djinterp_get_module_dependencies
#   function: returns the dependency chain for a given module
# 
# Parameters:
#   MODULE:     module name (e.g., dfile)
#   OUTPUT_VAR: name of variable to store the dependency list
# 
# The function returns modules in dependency order (dependencies first)
function(djinterp_get_module_dependencies MODULE OUTPUT_VAR)
    set(DEPS "")
    
    # define dependency chains
    if(MODULE STREQUAL "djinterp")
        # djinterp is the core, no dependencies
        set(DEPS "")
        
    elseif(MODULE STREQUAL "dmemory")
        # dmemory depends on djinterp
        set(DEPS "djinterp")
        
    elseif(MODULE STREQUAL "string_fn")
        # string_fn depends on dmemory (which depends on djinterp)
        set(DEPS "djinterp" "dmemory")
        
    elseif(MODULE STREQUAL "dfile")
        # dfile depends on djinterp, dmemory, and string_fn
        set(DEPS "djinterp" "dmemory" "string_fn")
        
    elseif(MODULE STREQUAL "dtime")
        # dtime depends on djinterp
        set(DEPS "djinterp")
        
    else()
        message(WARNING "Unknown module: ${MODULE}, assuming depends on djinterp only")
        set(DEPS "djinterp")
    endif()
    
    # add the module itself at the end
    list(APPEND DEPS ${MODULE})
    
    # return via OUTPUT_VAR
    set(${ARG_OUTPUT_VAR} ${DEPS} PARENT_SCOPE)
endfunction()