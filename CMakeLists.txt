cmake_minimum_required(VERSION 3.25)
project(arcade_game)

set(CMAKE_CXX_STANDARD 23)

# enable threads
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(arcade_game main.cpp src/entities/bases/BaseEntity.hpp src/utils/Point.hpp src/render/Renderable.hpp src/render/Renderer.cpp src/render/Renderer.hpp src/render/Characters.hpp src/Tickable.hpp src/Scene.cpp src/Scene.hpp src/entities/bases/BaseEntity.cpp src/utils/Cleanup.hpp src/Game.cpp src/Game.hpp src/Game.cpp src/entities/PlayerEntity.cpp src/entities/PlayerEntity.hpp src/render/TerminalSequences.hpp src/render/TerminalSequences.cpp src/entities/definition/EntityFactory.hpp src/entities/definition/EntityDB.cpp src/entities/definition/EntityDB.hpp src/utils/Concepts.hpp src/entities/definition/EntityFactory.cpp src/utils/Typedefs.hpp src/render/Pixel.cpp src/render/Pixel.hpp src/entities/bases/BaseHealthEntity.cpp src/entities/bases/BaseHealthEntity.hpp src/input/InputSystem.cpp src/input/InputSystem.hpp src/input/Keyboard.hpp src/utils/Math.hpp)
