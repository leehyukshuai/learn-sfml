#include "WindowContext.hpp"
#include "Solver.hpp"
#include "Renderer.hpp"
#include "Model.hpp"

Shape *createCloth();
int main() {
    WindowContext window("Simulator", { 800u, 600u });

    Model model;
    Solver solver(model);
    Renderer renderer(model, window.getRenderContext());

    Shape *shape = new Shape;
    shape->addPoint(Point({400, 100}, true));
    shape->addPoint(Point({600, 200}));
    shape->addPoint(Point({200, 190}));
    shape->addPoint(Point({700, 300}));
    shape->addPoint(Point({100, 300}));

    shape->addLink(Spring(0, 1));
    shape->addLink(Spring(0, 2));
    shape->addLink(Stick(1, 2));
    shape->addLink(Stick(1, 3));
    shape->addLink(Stick(2, 4));

    model.addShape(shape);
    solver.setConstForce({ 0.0f, 100.0f });

    while (window.run()) {
        solver.update();
        renderer.refresh();
    }
}
