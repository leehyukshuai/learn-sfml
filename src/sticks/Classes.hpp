#pragma once
#include <SFML/Graphics.hpp>
#include <optional>

struct Stick {
    int m_length;               // unit: pixels
    float m_angularVelocity;    // unit: rad/s
    float m_angle;              // unit: rad
    sf::Color m_color;          // the color of stick and its pencil
    bool m_draw;                // does it has a pencil attached?

    sf::Vector2f m_position;            // the position of end point

    Stick(int length, int avelocity, bool draw = false, sf::Color color = sf::Color::White) :
        m_length(length), m_angularVelocity(avelocity), m_angle(0.0f), m_color(color), m_draw(draw) {}
};
