#pragma once
#include "Math.hpp"
#include <memory>

struct Point {
    Vec2 m_curr;        // current position
    Vec2 m_prev;        // previous position
    bool m_constant;    // can this point be moved?

    Point();

    // it create a point at a position with initial velocity
    Point(Vec2 position, Vec2 velocity);

    // it create a still point at position
    Point(Vec2 position);

    // it create a still point at position
    Point(Vec2 position, bool constant);

};

struct Link {
    // use either pointers or index to present its two ends
    union {
        Point *ptr;
        unsigned idx;
    } m_p1, m_p2;
    bool m_constant;    // length is constant or not?
    bool m_solid;       // can other shape intersect with this link?
    // if m_length == NAN: use the distance as initial length
    float m_length;     // the initial length
    float m_strength;   // the k in Hooke's law (F=kx)
};

// 硬杆模型
struct Stick : public Link {
    // create a stick with its length equals to the distance between p1 and p2
    Stick(unsigned id1, unsigned id2, bool solid = false);
};

// 弹簧模型
struct Spring : public Link {
    // create a spring with its length equals to the distance between p1 and p2,
    Spring(unsigned id1, unsigned id2, float strength = 10.0f, bool solid = false);
    // create a spring with its length equals to the distance between p1 and p2,
    Spring(unsigned id1, unsigned id2, bool solid);
    // create a spring with its length specified
    Spring(unsigned id1, unsigned id2, float strength, float length, bool solid);
};

struct Shape {
    std::vector<Point> m_points;
    std::vector<Link> m_links;

    Shape();
    Shape(const std::vector<Point>&points);
    Shape(const std::vector<Point>&points , const std::vector<Link>&links);

    void pinPoint(int index);
    void addPoint(const Point &point);
    void addLink(const Link&l);
};

struct RigidBody : public Shape {
    // if simplify flag is true, the support sticks frame will be simplified.
    // make sure the passed-in points be arranged by (counter)clockwise!
    RigidBody(const std::vector<Vec2> &positions, bool simplify = false);
};

struct SoftBody : public Shape {
    // if simplify flag is true, the support sticks frame will be simplified.
    // make sure the passed-in points be arranged by (counter)clockwise!
    SoftBody(const std::vector<Vec2> &positions);
};

struct Border : public Shape {
    // all shapes will lie in this border
    // make sure the passed-in points be arranged by (counter)clockwise!
    Border(const std::vector<Vec2> &positions);
};