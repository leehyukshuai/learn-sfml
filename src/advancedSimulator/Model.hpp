#pragma once
#include "Classes.hpp"

class Model {
    friend class Renderer;
    friend class Solver;
private:
    std::vector<Shape *> m_shapes;
public:
    // the pointer will be maintained by Model, so you 
    // shouldn't use the passed-in pointer anymore
    void addShape(Shape *shape);

    ~Model();
};
