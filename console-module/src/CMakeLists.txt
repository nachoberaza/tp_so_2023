add_subdirectory(utils)

set(EXECUTABLE_CONSOLE_NAME console-module)
set(EXECUTABLE_CONSOLE_SOURCE console-module.c)
set(CONSOLE_UTILS_LIBRARY console_utils_library)

set(
        CONSOLE_LIBRARY_HEADERS
            utils/config-utils.h
            utils/file-utils.h
            utils/logger-utils.h
            utils/socket-utils.h
)

set(
        CONSOLE_LIBRARY_SOURCES
            utils/config-utils.c
            utils/file-utils.c
            utils/logger-utils.c
            utils/socket-utils.c
)


add_executable(${EXECUTABLE_CONSOLE_NAME} ${EXECUTABLE_CONSOLE_SOURCE})

# Adding console utils library
add_library(
        ${CONSOLE_UTILS_LIBRARY}
        STATIC
        ${CONSOLE_LIBRARY_HEADERS}
        ${CONSOLE_LIBRARY_SOURCES}
)

# Linking libraries
target_link_libraries(
        ${CONSOLE_UTILS_LIBRARY}
        PUBLIC
        ${SHARED_LIBRARY_NAME}
        ${COMMONS_LIBRARY}
)

target_link_libraries(
        ${EXECUTABLE_CONSOLE_NAME}
        PUBLIC
        ${SHARED_LIBRARY_NAME}
        ${CONSOLE_UTILS_LIBRARY}
)