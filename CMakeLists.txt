cmake_minimum_required(VERSION 3.15)
project(tsk_project C)

# Set C standard and compile flags
set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Common compile flags
set(CMAKE_C_FLAGS_DEBUG "-g -DDEBUG -Wall -Wpedantic")
set(CMAKE_C_FLAGS_RELEASE "-O3 -g -Wall -Wpedantic")

# Define source folders
set(TSKSRC src/tsk)
set(UTILSRC src/utils)
set(MSCSRC src/tsk_misc)

set(TSKBLD ${CMAKE_BINARY_DIR}/build/tsk)
set(UTILBLD ${CMAKE_BINARY_DIR}/build/utils)
set(MSCBLD ${CMAKE_BINARY_DIR}/build/tsk_misc)

# Create build directories
file(MAKE_DIRECTORY ${TSKBLD} ${UTILBLD} ${MSCBLD})

# Source files
set(TSK_SOURCES
    ${TSKSRC}/instruction.c
    ${TSKSRC}/node.c
)

set(UTIL_SOURCES
    ${UTILSRC}/linkedlist.c
    ${UTILSRC}/strfun.c
)

set(MISC_SOURCES
    ${MSCSRC}/tsk_loader.c
)

set(MAIN_SOURCES
    src/tsk.c
    src/args.c
    src/constants.c
)

# Create object libraries for each group
add_library(tsk_objects OBJECT ${TSK_SOURCES})
add_library(util_objects OBJECT ${UTIL_SOURCES})
add_library(misc_objects OBJECT ${MISC_SOURCES})

# Main TSK binary
add_executable(tsk ${MAIN_SOURCES}
    $<TARGET_OBJECTS:tsk_objects>
    $<TARGET_OBJECTS:util_objects>
    $<TARGET_OBJECTS:misc_objects>
)

# Test runner
add_executable(test_runner test/runner.c
    $<TARGET_OBJECTS:tsk_objects>
    $<TARGET_OBJECTS:util_objects>
    $<TARGET_OBJECTS:misc_objects>
)
target_link_libraries(test_runner criterion)

# Clean target
add_custom_target(clean-all
    COMMAND ${CMAKE_COMMAND} -E rm -rf ${CMAKE_BINARY_DIR}/build
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/tsk ${CMAKE_BINARY_DIR}/test_runner
    COMMAND ${CMAKE_COMMAND} -E rm -f ${CMAKE_BINARY_DIR}/*.log
)
