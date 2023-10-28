add_executable(advancedSimulator main.cpp Classes.cpp Math.hpp Model.cpp Renderer.cpp Solver.cpp)
target_link_libraries(advancedSimulator PRIVATE engine)
