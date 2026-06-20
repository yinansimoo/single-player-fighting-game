cmake_minimum_required(VERSION 3.5.0)
project(KDQS VERSION 0.1.0 LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(-Wall -Wpedantic)
endif()

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_mixer REQUIRED)
find_package(SDL2_ttf REQUIRED)

set(COMMON_SOURCES
    src/game.cpp
    src/timer.cpp 
    src/camera.cpp
    src/atlas.cpp
    src/animation.cpp 
    src/collision_box.cpp 
    src/manager/resource_manager.cpp
    src/manager/collision_manager.cpp
    src/manager/state_machine.cpp
    src/manager/character_manager.cpp
    src/manager/bullet_time_manager.cpp
    src/character.cpp
    src/player/player.cpp
    src/enemy/enemy.cpp
    src/player/player_state_node.cpp
    src/enemy/sword.cpp
    src/enemy/barb.cpp
    src/enemy/enemy_state_node.cpp
)

add_executable(game src/main.cpp ${COMMON_SOURCES})

set_target_properties(game PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}/bin"
)

set_target_properties(game PROPERTIES OUTPUT_NAME "game-${CMAKE_SYSTEM_NAME}")

# 定义路径
include_directories(
    ${PROJECT_SOURCE_DIR}/include
    ${PROJECT_SOURCE_DIR}/include/manager
    ${PROJECT_SOURCE_DIR}/include/player
    ${PROJECT_SOURCE_DIR}/include/enemy
    ${PROJECT_SOURCE_DIR}/thirdparty
)

# 添加路径
target_include_directories(game PRIVATE ${INCLUDE_DIRS})

target_link_libraries(game PRIVATE 
    SDL2::SDL2
    SDL2_image::SDL2_image
    SDL2_mixer::SDL2_mixer
    SDL2_ttf::SDL2_ttf
)




