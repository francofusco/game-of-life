# Project configuration
cmake_minimum_required(VERSION 3.12)
project(game_of_life)
set(CMAKE_CXX_STANDARD 11)

# copile options
option(DEBUG "Enable debug. If 'ON', it will define 'DEBUG_ON' during the compilation." OFF)

# Compile piksel
add_subdirectory(piksel)

# Name of the main executable. Feel free to change it.
set(GAME_OF_LIFE_EXE ${PROJECT_NAME})

# Compile the Game of Life
add_executable(${GAME_OF_LIFE_EXE}
  src/game_of_life.cpp
  src/main.cpp
)

# Make headers visible
target_include_directories(${GAME_OF_LIFE_EXE}
    PUBLIC src
    PUBLIC piksel/src
)

# Link piksel to the executable
target_link_libraries(${GAME_OF_LIFE_EXE}
    piksel
)

if(DEBUG)
  # Enable the use of the debug macro
  target_compile_definitions(${GAME_OF_LIFE_EXE} PRIVATE DEBUG_ON)
endif(DEBUG)
