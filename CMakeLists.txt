cmake_minimum_required(VERSION 3.25)
project(arcade_game)

set(CMAKE_CXX_STANDARD 23)

add_executable(arcade_game main.cppm src/entities/bases/BaseEntity.cppm src/utils/Point.cppm src/render/Renderable.cppm src/render/Renderer.cppm src/render/Characters.cppm src/Tickable.cppm src/Scene.cppm src/utils/Cleanup.cppm src/Game.cppm src/entities/PlayerEntity.cppm src/render/TerminalSequences.cppm src/entities/EntityFactory.cppm src/entities/EntityDB.cppm src/utils/Concepts.cppm src/utils/Typedefs.cppm src/render/Pixel.cppm src/entities/bases/BaseHealthEntity.cppm)
