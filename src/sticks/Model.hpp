#pragma once

#include <SFML/Graphics.hpp>
#include "Classes.hpp"
#include "Math.hpp"
#include <vector>
#include <unordered_map>

class Model {
    friend class Renderer;
    friend class Controler;
private:
    std::vector<Stick> m_sticks;
    std::unordered_map<const Stick *, std::vector<Vec2>> m_paths;
    bool m_updating;
    Vec2 m_startPosition;

    float m_speed;

public:
    Model();
    void update(sf::Time dt);
    void reset(const std::vector<Stick> &sticks);
    void pause();
    void start();
    void setStartPosition(const Vec2 &pos);
};
