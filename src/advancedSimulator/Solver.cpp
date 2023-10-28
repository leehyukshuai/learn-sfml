#include "Solver.hpp"
#include "Math.hpp"
#include <algorithm>

Solver::Solver(Model& model) : m_model(model) {}

void Solver::update() {
    float dt = m_clock.restart().asSeconds();
    dt = std::min(dt, 0.1f);
    
    applyLinkForce();
    verletIntegrate(dt);

    constexpr int subSteps = 4;
    for (int t{ subSteps }; t--;) {
        solveCollision();
        applyConstLink();
    }
}

void Solver::applyLinkForce() {
    m_forces.clear();
    for (auto& shape : m_model.m_shapes) {
        for (auto& link : shape->m_links) {
            if (!link.m_constant && link.m_strength) {
                Vec2 p1_p2 = link.m_p2.ptr->m_curr - link.m_p1.ptr->m_curr;
                float dist = MathVec2::length(p1_p2);
                p1_p2 /= dist;
                float dx = dist - link.m_length;
                // positive -> pull, negative -> support
                float f = link.m_strength * dx;
                m_forces[link.m_p1.ptr] += f * p1_p2;
                m_forces[link.m_p2.ptr] -= f * p1_p2;
            }
        }
    }
}

void Solver::verletIntegrate(float dt) {
    for (auto& shape : m_model.m_shapes) {
        for (auto& point : shape->m_points) {
            if (!point.m_constant) {
                // calculate the amount force
                Vec2 force = m_constForce;
                auto it = m_forces.find(&point);
                if (it != m_forces.end()) {
                    force += it->second;
                }
                // we suppose accelerate is just force (a = f)
                auto temp = point.m_curr;
                point.m_curr = 2.0f * point.m_curr - point.m_prev + force * dt * dt;
                point.m_prev = temp;
            }
        }
    }
}

void Solver::applyConstLink() {
    for (auto& shape : m_model.m_shapes) {
        for (auto& link : shape->m_links) {
            if (link.m_constant) {
                Vec2 p1_p2 = link.m_p2.ptr->m_curr - link.m_p1.ptr->m_curr;
                float dist = MathVec2::length(p1_p2);
                p1_p2 /= dist;
                // we separate them equally (neglect the mass at present)
                float delta = (dist - link.m_length) * 0.5f;
                link.m_p1.ptr->m_curr += delta * p1_p2;
                link.m_p2.ptr->m_curr -= delta * p1_p2;
            }
        }
    }
}

void Solver::solveCollision() {
    const int shapeCount = m_model.m_shapes.size();
    for (int i = 0; i < shapeCount; ++i) {
        auto& s1 = m_model.m_shapes[i];
        for (int j = i + 1; j < shapeCount; ++j) {
            auto& s2 = m_model.m_shapes[j];

            Vec2 p1, p2, temp;        // the two intersection points
            for (auto& l1 : s1->m_links) {
                for (auto& l2 : s2->m_links) {
                    if (intersect(l1, l2, temp)) {
                        if (p1.x || p1.y) {
                            p2 = temp;
                            // find the nearest point to get collision direction
                            Vec2 points[]{ l1.m_p1.ptr->m_curr, l1.m_p2.ptr->m_curr, l2.m_p1.ptr->m_curr, l2.m_p2.ptr->m_curr };
                            unsigned index = std::min_element(points, points + 4, [&temp](const Vec2& v1, const Vec2& v2) {
                                return MathVec2::distance2(v1, temp) < MathVec2::distance2(v2, temp);
                                }) - points;

                            Vec2 norm = MathVec2::normalize(MathVec2::normal(p2 - p1));
                            Vec2 outstand = points[index];
                            Vec2 p_o = outstand - p2;

                            float delta = MathVec2::dot(norm, p_o);
                            norm *= delta * 0.51f;

                            if (index == 0) {
                                l1.m_p1.ptr->m_curr -= norm;
                                l2.m_p1.ptr->m_curr += norm;
                                l2.m_p2.ptr->m_curr += norm;
                            } else if (index == 1) {
                                l1.m_p2.ptr->m_curr -= norm;
                                l2.m_p1.ptr->m_curr += norm;
                                l2.m_p2.ptr->m_curr += norm;
                            } else if (index == 2) {
                                l2.m_p1.ptr->m_curr -= norm;
                                l1.m_p1.ptr->m_curr += norm;
                                l1.m_p2.ptr->m_curr += norm;
                            } else {
                                l2.m_p2.ptr->m_curr -= norm;
                                l1.m_p1.ptr->m_curr += norm;
                                l1.m_p2.ptr->m_curr += norm;
                            }

                        } else {
                            p1 = temp;
                        }
                    }
                }
            }

        }
    }
}

bool Solver::intersect(const Link& l1, const Link& l2, Vec2& intersection) {
    float l1dx = l1.m_p2.ptr->m_curr.x - l1.m_p1.ptr->m_curr.x;
    float l1dy = l1.m_p2.ptr->m_curr.y - l1.m_p1.ptr->m_curr.y;
    float l1x0 = l1.m_p1.ptr->m_curr.x;
    float l1y0 = l1.m_p1.ptr->m_curr.y;
    float l2dx = l2.m_p2.ptr->m_curr.x - l2.m_p1.ptr->m_curr.x;
    float l2dy = l2.m_p2.ptr->m_curr.y - l2.m_p1.ptr->m_curr.y;
    float l2x0 = l2.m_p1.ptr->m_curr.x;
    float l2y0 = l2.m_p1.ptr->m_curr.y;

    float b1 = l1y0 * l1dx - l1x0 * l1dy;
    float b2 = l2y0 * l2dx - l2x0 * l2dy;

    float A = -l1dy * l2dx + l1dx * l2dy;

    constexpr float eps = 1.0f;

    if (std::abs(A) > eps) {
        float B1 = b1 * l2dx - b2 * l1dx;
        float B2 = -l1dy * b2 + l2dy * b1;

        float x = B1 / A;
        float y = B2 / A;

        float l1x1 = l1.m_p2.ptr->m_curr.x;
        float l1y1 = l1.m_p2.ptr->m_curr.y;
        float l2x1 = l2.m_p2.ptr->m_curr.x;
        float l2y1 = l2.m_p2.ptr->m_curr.y;

        if (Math::lieIn(x, l1x0, l1x1, eps) && Math::lieIn(x, l2x0, l2x1, eps) &&
            Math::lieIn(y, l1y0, l1y1, eps) && Math::lieIn(y, l2y0, l2y1, eps)) {
            intersection.x = x;
            intersection.y = y;
            return true;
        }
    }

    return false;
}


void Solver::setConstForce(Vec2 force) {
    m_constForce = force;
}
