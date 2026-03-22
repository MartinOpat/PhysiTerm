cmake_minimum_required(VERSION 3.10)
project(PhysiTerm VERSION 1.0.0 LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

# Compiler warnings
if(MSVC)
  add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /W4")
else()
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -Wall -Wextra -Wpedantic")
endif()

# Create executable
add_executable(physiterm
    src/main.c
    src/physics.c
    src/particles.c
    src/logs.c
    src/terminal.c
)

# Link libraries
if(UNIX)
  target_link_libraries(physiterm m ncurses)
endif()

# Include directories
target_include_directories(physiterm PRIVATE include)

# Set output directory
set_target_properties(physiterm PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin
)

