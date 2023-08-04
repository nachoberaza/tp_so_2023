add_subdirectory(utils)

set(EXECUTABLE_NAME console-module)
set(EXECUTABLE_SOURCE console-module.c)

add_executable(${EXECUTABLE_NAME} ${EXECUTABLE_SOURCE})