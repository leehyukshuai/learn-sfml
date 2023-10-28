#pragma once

#include "WindowContext.hpp"
#include <vector>
#include <cmath>
#include "utils.hpp"
extern const char* executable;

struct Ball {
    sf::Vector2f m_oldpos;
    sf::Vector2f m_curpos;
    float m_radius;
    float m_mass;
    Ball();
    Ball(sf::Vector2f position);
    Ball(sf::Vector2f position, sf::Vector2f velocity);
};

class Solver {
    friend class Renderer;
    std::vector<Ball> m_balls;
    const sf::Vector2f m_gravity;
    const sf::FloatRect m_border;
public:
    Solver();
    void addBall(const Ball &ball);
    void applyGravity(float dt);
    void applyBorder();
    void checkAndSolveCollision(Ball &a, Ball &b);
    void solveCollision();
    void update(float dt);
};

class Renderer {
    RenderContext& m_context;
    Solver& m_solver;
    sf::VertexArray m_vertices;
    sf::Texture m_texture;
    float m_textureSize;
    void constructVA();
    void paintBorder();
public:
    Renderer(Solver& solver, RenderContext& context);
    void refresh();
};

