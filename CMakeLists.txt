cmake_minimum_required(VERSION 3.25)
project(terminal_game)

set(CMAKE_CXX_STANDARD 23)

# enable threads
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")

add_executable(terminal_game main.cpp src/entities/bases/BaseEntity.hpp src/utils/Point.hpp src/render/Renderable.hpp src/render/Renderer.cpp src/render/Renderer.hpp src/render/Character.hpp src/render/Character.cpp src/Tickable.hpp src/Scene.cpp src/Scene.hpp src/entities/bases/BaseEntity.cpp src/utils/Cleanup.hpp src/Game.cpp src/Game.hpp src/Game.cpp src/entities/Cube.cpp src/entities/Cube.hpp src/utils/Terminal.hpp src/utils/Terminal.cpp src/entities/definition/EntityFactory.hpp src/entities/definition/EntityDB.cpp src/entities/definition/EntityDB.hpp src/utils/Concepts.hpp src/entities/definition/EntityFactory.cpp src/utils/Typedefs.hpp src/render/Pixel.cpp src/render/Pixel.hpp src/entities/bases/BaseHealthEntity.cpp src/entities/bases/BaseHealthEntity.hpp src/input/InputSystem.cpp src/input/InputSystem.hpp src/input/Keyboard.hpp src/utils/Math.hpp src/input/InputTypes.hpp src/utils/Color.cpp src/utils/Color.hpp src/utils/ITerminalSequence.hpp src/render/RenderHelpers.cpp src/render/RenderHelpers.hpp src/render/IRenderHelper.hpp)
