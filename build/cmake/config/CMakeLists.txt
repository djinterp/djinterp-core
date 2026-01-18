###############################################################################
# djinterp                                                       
# 
# This is the root-level CMakeList for CMake.
#
# author(s): Samuel 'teer' Neal-Blim                          date: 2023.03.01
###############################################################################
cmake_minimum_required(VERSION 3.20)

include(common.cmake)


# set C standard to C17
set(CMAKE_C_STANDARD 17 CACHE INTERNAL "") 
set(CMAKE_C_STANDARD_REQUIRED ON CACHE INTERNAL "")
# set C++ standard to C++20
set(CMAKE_CXX_STANDARD 23 CACHE INTERNAL "")
set(CMAKE_CXX_STANDARD_REQUIRED ON CACHE INTERNAL "")

# framework name
set(FRAMEWORK_NAME djinterp CACHE INTERNAL "")

# project name and version: this is DJINTERP
project(${FRAMEWORK_NAME} VERSION 0.1 LANGUAGES C CXX)

# get root directory
find_parent_directory_named(FRAMEWORK_ROOT_DIR ${FRAMEWORK_NAME})

#     Use ${FRAMEWORK_ROOT_DIR} to set subdirectory root path names
set(BINARY_DIR_ROOT 	${FRAMEWORK_ROOT_DIR}/bin 	CACHE INTERNAL "")
set(BUILD_DIR_ROOT 	${FRAMEWORK_ROOT_DIR}/build 	CACHE INTERNAL "")
set(INCLUDE_DIR_ROOT 	${FRAMEWORK_ROOT_DIR}/inc  	CACHE INTERNAL "")
set(RESOURCE_DIR_ROOT	${FRAMEWORK_ROOT_DIR}/res    	CACHE INTERNAL "")
set(SOURCE_DIR_ROOT 	${FRAMEWORK_ROOT_DIR}/src    	CACHE INTERNAL "")
set(TEST_DIR_ROOT   	${FRAMEWORK_ROOT_DIR}/test	CACHE INTERNAL "")

# set output directories to <root>/lib for libs, <root>/bin for executables
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${ROOT_DIR}/lib  CACHE INTERNAL "")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${ROOT_DIR}/lib  CACHE INTERNAL "")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${ROOT_DIR}/bin  CACHE INTERNAL "")

# Set Windows DLL includes
#     required to make Windows library files (*.lib/*.dll)
#set(DJINTERP_DLL_HEADERS
#    ${INC_DIR_ROOT}/build/dll/framework.h
#    ${INC_DIR_ROOT}/build/dll/pch.h
#CACHE INTERNAL "")
