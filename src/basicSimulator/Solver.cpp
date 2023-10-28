#include "Solver.hpp"
#include "utils.hpp"
#include "Math.hpp"

Ball::Ball() {}
Ball::Ball(sf::Vector2f position) : m_oldpos(position), m_curpos(position), m_radius(20.0f), m_mass(1.0f) {}
Ball::Ball(sf::Vector2f position, sf::Vector2f velocity)  : m_oldpos(position - velocity), m_curpos(position), m_radius(20.0f), m_mass(1.0f) {}

Solver::Solver() :
    m_gravity{ 20.0f, 100.0f },
    m_border{ 30.0f, 30.0f, 740.0f, 540.0f } {}

void Solver::addBall(const Ball& ball) {
    m_balls.push_back(ball);
}

void Solver::applyGravity(float dt) {
    const float t2 = dt * dt;
    for (auto& ball : m_balls) {
        auto temp = ball.m_curpos;
        ball.m_curpos = 2.0f * ball.m_curpos - ball.m_oldpos + m_gravity * t2;
        ball.m_oldpos = temp;
    }
}

void Solver::applyBorder() {
    for (auto& ball : m_balls) {
        if (ball.m_curpos.x - ball.m_radius < m_border.left) {
            float dx = ball.m_curpos.x - ball.m_oldpos.x;
            ball.m_curpos.x = m_border.left + ball.m_radius;
            ball.m_oldpos.x = ball.m_curpos.x + dx;
        }
        float right = m_border.left + m_border.width;
        if (ball.m_curpos.x + ball.m_radius > right) {
            float dx = ball.m_curpos.x - ball.m_oldpos.x;
            ball.m_curpos.x = right - ball.m_radius;
            ball.m_oldpos.x = ball.m_curpos.x + dx;
        }
        if (ball.m_curpos.y - ball.m_radius < m_border.top) {
            float dy = ball.m_curpos.y - ball.m_oldpos.y;
            ball.m_curpos.y = m_border.top + ball.m_radius;
            ball.m_oldpos.y = ball.m_curpos.y + dy;
        }
        float bottom = m_border.top + m_border.height;
        if (ball.m_curpos.y + ball.m_radius > bottom) {
            float dy = ball.m_curpos.y - ball.m_oldpos.y;
            ball.m_curpos.y = bottom - ball.m_radius;
            ball.m_oldpos.y = ball.m_curpos.y + dy;
        }
    }
}

void Solver::checkAndSolveCollision(Ball& a, Ball& b) {
    float dist2 = MathVec2::distance2(a.m_curpos, b.m_curpos);
    if (dist2 < (a.m_radius + b.m_radius) * (a.m_radius + b.m_radius)) {
        auto axis = b.m_curpos - a.m_curpos;
        axis /= MathVec2::length(axis);
        float dist = std::sqrt(dist2);
        float delta = (a.m_radius + b.m_radius - dist) * 0.5f;
        a.m_curpos -= axis * delta;
        b.m_curpos += axis * delta;
    }
}

void Solver::solveCollision() {
    const unsigned subSteps = 1;
    for (int i{ subSteps }; i--;) {
        for (int a = 0; a < m_balls.size(); ++a) {
            for (int b = a + 1; b < m_balls.size(); ++b) {
                checkAndSolveCollision(m_balls[a], m_balls[b]);
            }
        }
    }
}

void Solver::update(float dt) {
    // 最大间隔时间为0.1秒
    dt = std::min(dt, 0.1f);
    applyGravity(dt);
    applyBorder();
    solveCollision();
}

Renderer::Renderer(Solver& solver, RenderContext& context) :
    m_context(context), m_solver(solver), m_vertices(sf::PrimitiveType::Quads) {
    if (!m_texture.loadFromFile(Utils::fetch("ball.png"))) {
        throw std::runtime_error("Failed to load ball.png");
    }
    m_texture.generateMipmap();
    m_texture.setSmooth(true);
    m_textureSize = m_texture.getSize().x;
}

void Renderer::constructVA() {
    auto& balls = m_solver.m_balls;
    m_vertices.resize(balls.size() * 4);
    for (int i = 0; i < balls.size(); ++i) {
        auto& ball = balls[i];
        m_vertices[i * 4 + 0].position = ball.m_curpos + sf::Vector2f(-ball.m_radius, -ball.m_radius);
        m_vertices[i * 4 + 1].position = ball.m_curpos + sf::Vector2f(-ball.m_radius, ball.m_radius);
        m_vertices[i * 4 + 2].position = ball.m_curpos + sf::Vector2f(ball.m_radius, ball.m_radius);
        m_vertices[i * 4 + 3].position = ball.m_curpos + sf::Vector2f(ball.m_radius, -ball.m_radius);

        // m_vertices[i * 4 + 0].color = sf::Color::White;
        // m_vertices[i * 4 + 1].color = sf::Color::White;
        // m_vertices[i * 4 + 2].color = sf::Color::White;
        // m_vertices[i * 4 + 3].color = sf::Color::White;

        m_vertices[i * 4 + 0].texCoords = sf::Vector2f(0, 0);
        m_vertices[i * 4 + 1].texCoords = sf::Vector2f(0, m_textureSize);
        m_vertices[i * 4 + 2].texCoords = sf::Vector2f(m_textureSize, m_textureSize);
        m_vertices[i * 4 + 3].texCoords = sf::Vector2f(m_textureSize, 0);
    }
}

void Renderer::paintBorder() {
    sf::RectangleShape shape;
    shape.setPosition(sf::Vector2f(m_solver.m_border.left, m_solver.m_border.top));
    shape.setSize(sf::Vector2f(m_solver.m_border.width, m_solver.m_border.height));
    shape.setFillColor(sf::Color(128, 128, 128));
    m_context.draw(shape);
}

void Renderer::refresh() {
    m_context.clear();
    paintBorder();
    constructVA();
    m_context.draw(m_vertices, &m_texture);
    m_context.display();
}