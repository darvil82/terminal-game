cmake_minimum_required(VERSION 3.25)
project(arcade_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(arcade_game main.cpp src/entities/bases/BaseEntity.cpp src/utils/Point.cpp src/render/Renderable.cpp src/render/Renderer.cpp src/render/Characters.cpp src/Tickable.cpp src/Scene.cpp src/utils/Cleanup.cpp src/Game.cpp src/entities/PlayerEntity.cpp src/render/TerminalSequences.cpp src/entities/EntityFactory.cpp src/entities/EntityDB.cpp src/utils/Concepts.cpp src/utils/Typedefs.cpp src/render/Pixel.cpp src/entities/bases/BaseHealthEntity.cpp)
