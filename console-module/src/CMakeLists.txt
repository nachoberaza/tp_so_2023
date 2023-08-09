add_subdirectory(utils)

set(EXECUTABLE_CONSOLE_NAME console-module)
set(EXECUTABLE_CONSOLE_SOURCE console-module.c)

add_executable(${EXECUTABLE_CONSOLE_NAME} ${EXECUTABLE_CONSOLE_SOURCE})

target_link_libraries(
        ${EXECUTABLE_CONSOLE_NAME}
        PUBLIC
        ${SHARED_LIBRARY_NAME}
)