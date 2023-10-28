#include "Renderer.hpp"

void Renderer::constructVertexArray(){
    m_array.clear();
    
    for (auto &shape : m_model.m_shapes) {
        for (auto &link : shape->m_links) {
            sf::Vertex v1, v2;
            v1.position = link.m_p1.ptr->m_curr;
            v2.position = link.m_p2.ptr->m_curr;
            m_array.append(v1);
            m_array.append(v2);
        }
    }
}

Renderer::Renderer(Model& model, RenderContext& context) :
    m_model(model), m_context(context), m_array(sf::PrimitiveType::Lines) {}

void Renderer::refresh(){
    m_context.clear();
    constructVertexArray();
    m_context.draw(m_array);
    m_context.display();
}
