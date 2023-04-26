cmake_minimum_required(VERSION 3.25)
project(arcade_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(arcade_game main.cpp src/entities/bases/BaseEntity.cpp src/utils/Point.cpp src/render/Renderable.hpp src/render/Renderer.cpp src/render/Renderer.hpp src/render/Characters.hpp src/Tickable.hpp src/Scene.cpp src/Scene.hpp src/utils/Cleanup.cpp src/Game.cpp src/Game.hpp src/Game.cpp src/entities/PlayerEntity.cpp src/render/TerminalSequences.hpp src/render/TerminalSequences.cpp src/entities/EntityFactory.cpp src/entities/EntityDB.cpp src/utils/Concepts.cpp src/utils/Typedefs.cpp src/render/Pixel.cpp src/render/Pixel.hpp src/entities/bases/BaseHealthEntity.cpp)
