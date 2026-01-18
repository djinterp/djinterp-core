# tests_standalone.cmake
# Standalone test framework specific infrastructure
# Provides functions for creating standalone test executables

# include the common test infrastructure
# Use CMAKE_CURRENT_LIST_DIR to get the directory where this file is located
get_filename_component(TESTS_STANDALONE_DIR "${CMAKE_CURRENT_LIST_FILE}" PATH)
include(${TESTS_STANDALONE_DIR}/tests_common.cmake)


# djinterp_add_standalone_framework
#   function: adds the standalone test framework files to a source list
# 
# Parameters:
#   OUTPUT_VAR:            name of variable to store/append the framework files
#   TEST_FRAMEWORK_DIR:    optional directory where test framework files are located
#                          (defaults to TEST_DIR if not specified)
# 
# Adds:
#   - test_standalone.c
#   - test_common.c
# 
# Example:
#   djinterp_add_standalone_framework(OUTPUT_VAR MY_SOURCES)
#   djinterp_add_standalone_framework(
#       OUTPUT_VAR MY_SOURCES 
#       TEST_FRAMEWORK_DIR "${SOURCE_DIR}/test"
#   )
function(djinterp_add_standalone_framework)
    # parse arguments
    set(options "")
    set(oneValueArgs OUTPUT_VAR TEST_FRAMEWORK_DIR)
    set(multiValueArgs "")
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # validate required arguments
    if(NOT DEFINED ARG_OUTPUT_VAR)
        message(FATAL_ERROR "djinterp_add_standalone_framework: OUTPUT_VAR is required")
    endif()
    
    # use custom framework dir if provided, otherwise use TEST_DIR
    if(DEFINED ARG_TEST_FRAMEWORK_DIR)
        set(FRAMEWORK_DIR "${ARG_TEST_FRAMEWORK_DIR}")
    else()
        set(FRAMEWORK_DIR "${TEST_DIR}")
    endif()
    
    # start with existing value if variable is already defined
    if(DEFINED ${ARG_OUTPUT_VAR})
        set(RESULT_LIST ${${ARG_OUTPUT_VAR}})
    else()
        set(RESULT_LIST "")
    endif()
    
    # add framework files
    list(APPEND RESULT_LIST 
        "${FRAMEWORK_DIR}/test_standalone.c"
        "${FRAMEWORK_DIR}/test_common.c"
    )
    
    # return via OUTPUT_VAR
    set(${ARG_OUTPUT_VAR} ${RESULT_LIST} PARENT_SCOPE)
endfunction()


# djinterp_get_standalone_dependencies
#   function: returns all dependencies needed by the standalone test framework
# 
# Parameters:
#   OUTPUT_VAR: name of variable to store the dependency module list
# 
# The standalone test framework requires:
#   - djinterp.h (core)
#   - dmemory.h (for d_memcpy, d_memset)
#   - dfile.h (for file I/O in test output)
#   - string_fn.h (for string utilities)
# 
# Returns modules in dependency order
function(djinterp_get_standalone_dependencies OUTPUT_VAR)
    # the standalone framework needs:
    # - djinterp (core)
    # - dmemory (memory functions)
    # - string_fn (string functions, which also includes dmemory)
    # - dfile (file operations)
    
    set(DEPS 
        "djinterp"
        "dmemory"
        "string_fn"
    )
    
    # return via OUTPUT_VAR
    set(${OUTPUT_VAR} ${DEPS} PARENT_SCOPE)
endfunction()


# djinterp_create_standalone_test_executable
#   function: creates a standalone test executable with all required dependencies
# 
# Parameters:
#   TARGET:         name of the test executable target
#   TEST_PATTERN:   glob pattern for test files (e.g., "dfile_tests_sa*.c")
#   EXTRA_MODULES:  additional module dependencies beyond framework defaults (optional)
#   LINK_LIBRARIES: additional libraries to link against (optional)
# 
# Automatically includes:
#   - Test framework files (test_standalone.c, test_common.c)
#   - Core dependencies (djinterp, dmemory, string_fn)
#   - Test files matching the pattern
#   - Any extra modules specified
# 
# Example:
#   djinterp_create_standalone_test_executable(
#       TARGET djinterp-c-dfile-tests-sa-${PLATFORM_ARCH}
#       TEST_PATTERN "dfile_tests_sa*.c"
#       LINK_LIBRARIES dfile
#   )
#
#   djinterp_create_standalone_test_executable(
#       TARGET djinterp-c-dtime-tests-sa-${PLATFORM_ARCH}
#       TEST_PATTERN "dtime_tests_sa*.c"
#       EXTRA_MODULES dtime
#   )
function(djinterp_create_standalone_test_executable)
    # parse arguments
    set(options "")
    set(oneValueArgs TARGET TEST_PATTERN)
    set(multiValueArgs EXTRA_MODULES LINK_LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # validate required arguments
    if(NOT DEFINED ARG_TARGET)
        message(FATAL_ERROR "djinterp_create_standalone_test_executable: TARGET is required")
    endif()
    if(NOT DEFINED ARG_TEST_PATTERN)
        message(FATAL_ERROR "djinterp_create_standalone_test_executable: TEST_PATTERN is required")
    endif()
    
    # gather test files (with djinterp.c prepended)
    djinterp_gather_test_files(
        OUTPUT_VAR TEST_SOURCES
        PATTERN "${ARG_TEST_PATTERN}"
        PREPEND_CORE ON
    )
    
    # add standalone test framework files
    djinterp_add_standalone_framework(OUTPUT_VAR TEST_SOURCES)
    
    # get framework dependencies
    djinterp_get_standalone_dependencies(FRAMEWORK_DEPS)
    
    # add framework dependency modules
    djinterp_add_module_dependencies(
        OUTPUT_VAR TEST_SOURCES
        MODULES ${FRAMEWORK_DEPS}
    )
    
    # add extra module dependencies if specified
    if(DEFINED ARG_EXTRA_MODULES)
        djinterp_add_module_dependencies(
            OUTPUT_VAR TEST_SOURCES
            MODULES ${ARG_EXTRA_MODULES}
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


# djinterp_create_standalone_test_library_approach
#   function: creates a reusable standalone test support library and links it to an executable
# 
# This is an alternative approach that compiles the framework and dependencies once
# and links them to multiple test executables. More efficient for multiple test suites.
# 
# Parameters:
#   TARGET:         name of the test executable target
#   TEST_PATTERN:   glob pattern for test files (e.g., "dfile_tests_sa*.c")
#   SUPPORT_LIB:    name of the shared support library (e.g., "test-standalone-support")
#   LINK_LIBRARIES: additional libraries to link against (optional)
#   CREATE_SUPPORT: whether to create the support library (only for first call)
# 
# Example:
#   # First test suite - create the support library
#   djinterp_create_standalone_test_library_approach(
#       TARGET djinterp-c-dfile-tests-sa-${PLATFORM_ARCH}
#       TEST_PATTERN "dfile_tests_sa*.c"
#       SUPPORT_LIB test-standalone-support
#       CREATE_SUPPORT ON
#       LINK_LIBRARIES dfile
#   )
#   
#   # Subsequent test suites - reuse the support library
#   djinterp_create_standalone_test_library_approach(
#       TARGET djinterp-c-dtime-tests-sa-${PLATFORM_ARCH}
#       TEST_PATTERN "dtime_tests_sa*.c"
#       SUPPORT_LIB test-standalone-support
#       CREATE_SUPPORT OFF
#       LINK_LIBRARIES dtime
#   )
function(djinterp_create_standalone_test_library_approach)
    # parse arguments
    set(options "")
    set(oneValueArgs TARGET TEST_PATTERN SUPPORT_LIB CREATE_SUPPORT)
    set(multiValueArgs LINK_LIBRARIES)
    cmake_parse_arguments(ARG "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})
    
    # validate required arguments
    if(NOT DEFINED ARG_TARGET)
        message(FATAL_ERROR "djinterp_create_standalone_test_library_approach: TARGET is required")
    endif()
    if(NOT DEFINED ARG_TEST_PATTERN)
        message(FATAL_ERROR "djinterp_create_standalone_test_library_approach: TEST_PATTERN is required")
    endif()
    if(NOT DEFINED ARG_SUPPORT_LIB)
        message(FATAL_ERROR "djinterp_create_standalone_test_library_approach: SUPPORT_LIB is required")
    endif()
    
    # default CREATE_SUPPORT to ON if not specified
    if(NOT DEFINED ARG_CREATE_SUPPORT)
        set(ARG_CREATE_SUPPORT ON)
    endif()
    
    # create support library if requested
    if(ARG_CREATE_SUPPORT)
        set(SUPPORT_SOURCES "")
        
        # add djinterp core
        list(APPEND SUPPORT_SOURCES "${SOURCE_DIR}/djinterp.c")
        
        # add framework files
        djinterp_add_standalone_framework(OUTPUT_VAR SUPPORT_SOURCES)
        
        # get and add framework dependencies
        djinterp_get_standalone_dependencies(FRAMEWORK_DEPS)
        djinterp_add_module_dependencies(
            OUTPUT_VAR SUPPORT_SOURCES
            MODULES ${FRAMEWORK_DEPS}
        )
        
        # create the support library
        add_library(${ARG_SUPPORT_LIB} STATIC ${SUPPORT_SOURCES})
        target_include_directories(${ARG_SUPPORT_LIB} PUBLIC ${INCLUDE_DIR} ${TEST_DIR})
    endif()
    
    # gather test files (without djinterp.c since it's in the support library)
    file(GLOB TEST_FILES "${TEST_DIR}/${ARG_TEST_PATTERN}")
    
    # create the test executable
    add_executable(${ARG_TARGET} ${TEST_FILES})
    
    # set include directories
    target_include_directories(${ARG_TARGET} 
        PRIVATE 
        ${INCLUDE_DIR} 
        ${TEST_DIR}
    )
    
    # link the support library
    target_link_libraries(${ARG_TARGET} PRIVATE ${ARG_SUPPORT_LIB})
    
    # link additional libraries if specified
    if(DEFINED ARG_LINK_LIBRARIES)
        target_link_libraries(${ARG_TARGET} PRIVATE ${ARG_LINK_LIBRARIES})
    endif()
endfunction()