add_subdirectory(shared)


add_library(
        ${LIBRARY_NAME}
        SHARED
        ${LIBRARY_HEADERS}
        ${LIBRARY_SOURCES}
)

target_include_directories(${LIBRARY_NAME} PUBLIC "./shared")