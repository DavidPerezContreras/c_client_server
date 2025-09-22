cmake_minimum_required(VERSION 3.10)

project(hello_world VERSION 0.1.0 LANGUAGES C)

include(CTest)
enable_testing()

# Server executable
add_executable(server server.c)

# Client executable
add_executable(client client.c)

# Enable sanitizers or runtime checks based on compiler
foreach(target IN ITEMS server client)
    if(CMAKE_C_COMPILER_ID MATCHES "Clang|GNU")
        message(STATUS "Clang/GCC detected — enabling AddressSanitizer for ${target}")
        target_compile_options(${target} PRIVATE -fsanitize=address -g)
        target_link_options(${target} PRIVATE -fsanitize=address)
    elseif(MSVC)
        message(STATUS "MSVC detected — enabling runtime checks for ${target}")
        target_compile_options(${target} PRIVATE /Zi /Od /RTC1)
        target_link_options(${target} PRIVATE /DEBUG)
    endif()
endforeach()
