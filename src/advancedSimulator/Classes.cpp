#include "Classes.hpp"

Point::Point() :
    m_curr{}, m_prev{}, m_constant{} {}

Point::Point(Vec2 position, Vec2 velocity) :
    m_curr(position), m_prev(position - velocity), m_constant(false) {}

Point::Point(Vec2 position) :
    m_curr(position), m_prev(position), m_constant(false) {}

Point::Point(Vec2 position, bool constant) :
    m_curr(position), m_prev(position), m_constant(constant) {}

Stick::Stick(unsigned id1, unsigned id2, bool solid) {
    m_p1.idx = id1;
    m_p2.idx = id2;
    m_constant = true;
    // later, m_length will be substituded by the distance between p1 and p2
    m_length = std::nanf("");
    m_solid = solid;
    m_strength = 0.0f;
}

Spring::Spring(unsigned id1, unsigned id2, float strength, bool solid) {
    m_p1.idx = id1;
    m_p2.idx = id2;
    m_constant = false;
    // later, m_length will be substituded by the distance between p1 and p2
    m_length = std::nanf("");
    m_solid = solid;
    m_strength = strength;
}

Spring::Spring(unsigned id1, unsigned id2, bool solid) {
    m_p1.idx = id1;
    m_p2.idx = id2;
    m_constant = false;
    m_length = std::nanf("");
    m_solid = solid;
    m_strength = 10.0f;
}

Spring::Spring(unsigned id1, unsigned id2, float strength, float length, bool solid) {
    m_p1.idx = id1;
    m_p2.idx = id2;
    m_constant = false;
    m_length = length;
    m_solid = solid;
    m_strength = strength;
}

Shape::Shape() {}

Shape::Shape(const std::vector<Point>& points) :
    m_points(points) {}

Shape::Shape(const std::vector<Point>& points, const std::vector<Link>& links) :
    m_points(points), m_links(links) {
    for (auto& link : m_links) {
        link.m_p1.ptr = &m_points[link.m_p1.idx];
        link.m_p2.ptr = &m_points[link.m_p2.idx];
        if (link.m_length != link.m_length) {
            link.m_length = MathVec2::distance(link.m_p1.ptr->m_curr, link.m_p2.ptr->m_curr);
        }
    }
}

void Shape::pinPoint(int index) {
    m_points[index].m_constant = true;
}

void Shape::addPoint(const Point& point) {
    m_points.push_back(point);
}

void Shape::addLink(const Link& l) {
    m_links.push_back(l);
    Link& link = m_links.back();
    link.m_p1.ptr = &m_points[link.m_p1.idx];
    link.m_p2.ptr = &m_points[link.m_p2.idx];
    if (link.m_length != link.m_length) {
        link.m_length = MathVec2::distance(link.m_p1.ptr->m_curr, link.m_p2.ptr->m_curr);
    }
}

RigidBody::RigidBody(const std::vector<Vec2>& positions, bool simplify) {

    for (auto& pos : positions) {
        addPoint(Point(pos, false));
    }

    int pointCount = positions.size();

    // sticks on the border are solid
    for (int i = 0; i < pointCount - 1; ++i) {
        addLink(Stick(i, i + 1, true));
    }
    addLink(Stick(0, pointCount - 1, true));

    // add supporting internal sticks
    if (simplify) {
        for (int i = 2; i < pointCount - 1; ++i) {
            addLink(Stick(0, i, true));
        }
    } else {
        for (int i = 0; i < pointCount; ++i) {
            for (int j = i + 2; j < std::min(i + pointCount - 1, pointCount); ++j) {
                addLink(Stick(i, j));
            }
        }
    }
}

Border::Border(const std::vector<Vec2>& positions) {

    for (auto& pos : positions) {
        addPoint(Point(pos, true));
    }

    int pointCount = positions.size();

    for (int i = 0; i < pointCount - 1; ++i) {
        addLink(Stick(i, i + 1, true));
    }
    addLink(Stick(0, pointCount - 1, true));
}

SoftBody::SoftBody(const std::vector<Vec2>& positions) {
    for (auto& pos : positions) {
        addPoint(Point(pos, false));
    }

    int pointCount = positions.size();

    // springs on the border are solid
    for (int i = 0; i < pointCount - 1; ++i) {
        addLink(Spring(i, i + 1, 100.0f));
    }
    addLink(Spring(0, pointCount - 1, 100.0f));

    // add supporting internal springs
    for (int i = 0; i < pointCount; ++i) {
        for (int j = i + 2; j < std::min(i + pointCount - 1, pointCount); ++j) {
            addLink(Spring(i, j, 100.0f));
        }
    }
}
