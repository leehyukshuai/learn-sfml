#include "Model.hpp"

Model::Model() : m_updating(true), m_speed(1.0f) {}

void Model::update(sf::Time dt) {
    if (m_updating) {
        Vec2 prevPos = m_startPosition;
        float prevAng = 0.0f;
        for (auto& stick : m_sticks) {
            stick.m_angle += stick.m_angularVelocity * dt.asSeconds() * m_speed;

            float nextAng = prevAng + stick.m_angle;
            Vec2 deltaPos = {stick.m_length * std::cos(nextAng), stick.m_length * std::sin(nextAng)};
            Vec2 nextPos = prevPos + deltaPos;

            stick.m_position = nextPos;

            if (stick.m_draw) {
                m_paths[&stick].push_back(nextPos);
            }

            prevAng = nextAng;
            prevPos = nextPos;
        }
    }
}

void Model::reset(const std::vector<Stick>& sticks) {
    m_sticks = sticks;
}

void Model::pause() {
    m_updating = !m_updating;
}

void Model::start() {
    // reset angles
    for (auto& stick : m_sticks) {
        stick.m_angle = 0;
    }
    // reset paths
    m_paths.clear();
    m_updating = true;
    update(sf::Time::Zero);
    m_updating = false;
}

void Model::setStartPosition(const Vec2& pos) {
    m_startPosition = pos;
}
