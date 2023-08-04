# Setting cmake minimum version required
cmake_minimum_required(VERSION 3.25)

project(
        tp-2023-1c-Compu-mundo-hiper-mega-red
        VERSION
        1.0.0
        LANGUAGES
        C
)

set(SHARED_LIBRARY_NAME shared_library)
set(SO_COMMONS_LIBRARY so_commons_library)

add_subdirectory(console-module)
add_subdirectory(cpu-module)
add_subdirectory(file-system-module)
add_subdirectory(kernel-module)
add_subdirectory(memory-module)
add_subdirectory(shared-library)