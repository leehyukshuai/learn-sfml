#include "Solver.hpp"

int main(int argc, char** argv) {
    Utils::initialize(argv[0]);
    
    WindowContext window("physics", {800u, 600u});
    Solver solver;
    RenderContext &context = window.getRenderContext();
    Renderer renderer(solver, context);

    unsigned n = 0;

    sf::Clock clock;
    while (window.run())
    {
        if (n++ % 90 == 0) {
            solver.addBall(Ball{{100, 100}});
        }
        solver.update(clock.restart().asSeconds());
        renderer.refresh();
    }
}