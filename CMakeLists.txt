cmake_minimum_required(VERSION 3.25)
project(arcade_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(arcade_game main.cpp src/entities/Entity.hpp src/utils/Point.hpp src/render/Renderable.hpp src/render/Renderer.cpp src/render/Renderer.hpp src/render/Characters.hpp src/entities/Tickable.hpp src/Scene.cpp src/Scene.hpp src/entities/Entity.cpp src/utils/Cleanup.hpp)
