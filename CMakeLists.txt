cmake_minimum_required(VERSION 3.25)
project(arcade_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(arcade_game main.cpp src/entities/BaseEntity.hpp src/utils/Point.hpp src/render/Renderable.hpp src/render/Renderer.cpp src/render/Renderer.hpp src/render/Characters.hpp src/Tickable.hpp src/Scene.cpp src/Scene.hpp src/entities/BaseEntity.cpp src/utils/Cleanup.hpp src/Game.cpp src/Game.hpp src/Game.cpp)
