//
// Created by efarhan on 2/22/21.
//

#pragma once

#include <cmath>

namespace maths
{
inline float Abs(float v)
{
    return std::abs(v);
}

inline bool Equal(float a, float b, float epsilon = 0.0000001f) {
    return Abs(a - b) < epsilon;
}
template<typename T>
inline float Mod(T a, T b)
{
    return a<b?a:a%b;
}
}