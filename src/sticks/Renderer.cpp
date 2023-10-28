#include "Renderer.hpp"

Renderer::Renderer(Model& model, RenderContext& context) :
    m_model(model), m_context(context) {}

void Renderer::renderSticks() const {
    static sf::VertexArray m_array(sf::PrimitiveType::Lines);
    const auto& sticks = m_model.m_sticks;
    m_array.resize(2 * sticks.size());
    Vec2 prevPos = m_model.m_startPosition;
    for (int i = 0; i < sticks.size(); ++i) {
        const auto &stick = sticks[i];
        m_array[2 * i + 0].position = prevPos;
        m_array[2 * i + 1].position = stick.m_position;
        prevPos = stick.m_position;
        m_array[2 * i + 0].color = sf::Color::White;
        m_array[2 * i + 1].color = stick.m_color;
    }
    m_context.draw(m_array);
}

void Renderer::renderPath() const {
    static sf::VertexArray m_array(sf::PrimitiveType::LineStrip);
    for (const auto &path : m_model.m_paths) {
        if (path.first->m_draw) {
            int pointCount = path.second.size();
            m_array.resize(pointCount);

            for (int i = 0; i < path.second.size(); ++i) {
                m_array[i].position = path.second[i];
                m_array[i].color = path.first->m_color;
            }
            m_context.draw(m_array);
        }

    }
}

void Renderer::render() const {
    renderPath();
    if (m_showSticks) {
        renderSticks();
    }
}

bool Renderer::getShowSticks() const {
    return m_showSticks;
}

void Renderer::setShowSticks(bool showSticks) {
    m_showSticks = showSticks;
}
