#pragma once
#include "Model.hpp"
#include "RenderContext.hpp"

class Renderer {
private:
    Model &m_model;
    RenderContext &m_context;
    sf::VertexArray m_array;
    
    void constructVertexArray();
public:
    Renderer(Model &model, RenderContext &context);
    void refresh();
};