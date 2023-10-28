#include "Model.hpp"

void Model::addShape(Shape* shape) {
    m_shapes.push_back(shape);
}

Model::~Model() {
    for (auto &shape : m_shapes) {
        delete shape;
    }
}
