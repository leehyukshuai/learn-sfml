#pragma once
#include "Model.hpp"
#include <unordered_map>

class Solver {
private:
    Model &m_model;
    Vec2 m_constForce;
    sf::Clock m_clock;
    std::unordered_map<Point *, Vec2> m_forces;

    void applyLinkForce();
    void verletIntegrate(float dt);
    void applyConstLink();
    void solveCollision();

    bool intersect(const Link& l1, const Link& l2, Vec2& intersection);

public:
    Solver(Model &model);

    void update();
    void setConstForce(Vec2 force);
};