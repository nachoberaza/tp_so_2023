add_subdirectory(shared)

set(
        SHARED_LIBRARY_SOURCES
            shared/execution-context/execution-context.c
            shared/logs/log.c
            shared/memory/memory.c
            shared/serialization/serialization.c
            shared/sockets/server-socket.c
            shared/sockets/client-socket.c
)

set(
        SHARED_LIBRARY_HEADERS
        shared/execution-context/execution-context.h
        shared/logs/log.h
        shared/memory/memory.h
        shared/serialization/serialization.h
        shared/sockets/server-socket.h
        shared/sockets/client-socket.h
        shared/structs/execution-context.h
        shared/structs/memory.h
        shared/structs/socket.h
)

add_library(
        ${SHARED_LIBRARY_NAME}
        SHARED
        ${SHARED_LIBRARY_HEADERS}
        ${SHARED_LIBRARY_SOURCES}
)

target_include_directories(${SHARED_LIBRARY_NAME} PUBLIC "./")