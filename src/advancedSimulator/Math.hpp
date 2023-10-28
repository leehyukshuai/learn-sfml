#pragma once

#include <SFML/Graphics.hpp>
#include <cmath>

using Vec2 = sf::Vector2f;
using CVec2 = const sf::Vector2f&;

struct Math {
    // if x lies in [a-eps, b+eps] or [b-eps, a+eps]?
    static bool lieIn(float x, float a, float b, float eps = 1e-6) {
        if (a > b) {
            std::swap(a, b);
        }
        return (a - eps <= x) && (x <= b + eps);
    }
};

struct MathVec2
{
    template<typename TVec2>
    static float length2(TVec2 v)
    {
        return v.x * v.x + v.y * v.y;
    }

    template<typename TVec2>
    static float length(TVec2 v)
    {
        return std::sqrt(length2(v));
    }

    template<typename TVec2>
    static float distance2(TVec2 v1, TVec2 v2)
    {
        return (v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y);
    }

    template<typename TVec2>
    static float distance(TVec2 v1, TVec2 v2)
    {
        return std::sqrt((v1.x - v2.x) * (v1.x - v2.x) + (v1.y - v2.y) * (v1.y - v2.y));
    }

    template<typename TVec2>
    static float dot(TVec2 v1, TVec2 v2)
    {
        return v1.x * v2.x + v1.y * v2.y;
    }

    template<typename TVec2>
    static float cross(TVec2 v1, TVec2 v2)
    {
        return v1.x * v2.y - v1.y * v2.x;
    }
    
    template<typename Vec2Type>
    static Vec2Type normal(const Vec2Type& v)
    {
        return {-v.y, v.x};
    }

    template<typename Vec2Type>
    static Vec2Type normalize(const Vec2Type& v)
    {
        return v / length(v);
    }

    template<template<typename> class Vec2Type, typename T>
    static Vec2Type<T> reflect(const Vec2Type<T>& v, const Vec2Type<T>& n)
    {
        return v - n * (MathVec2::dot(v, n) * 2.0f);
    }
};

