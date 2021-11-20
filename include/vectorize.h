//
// Created by efarhan on 23/02/2021.
//

#pragma once

#include <array>
#include <cstdlib>
#include <cmath>

#include "instrinsics.h"

namespace maths
{
template<std::size_t N>
class FloatArray
{
public:
    FloatArray() = default;
    FloatArray(const std::array<float, N>& arg): values_(arg){}

    float& operator[](std::size_t idx) { return values_[idx]; }
    const float& operator[](std::size_t idx) const { return values_[idx]; }

    FloatArray operator+(const FloatArray& rhs) const;
    FloatArray operator-(const FloatArray& rhs) const;
    FloatArray operator-() const;
    FloatArray operator*(const FloatArray& rhs) const;
    FloatArray operator*(float rhs) const;
    static FloatArray Sqrt(const FloatArray& rhs);
    static FloatArray Reciprocal(const FloatArray& rhs);

    [[nodiscard]] const std::array<float, N>& array() const {return values_;}
private:
    std::array<float, N> values_;
};

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator+(const FloatArray &rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]+rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator-(const FloatArray &rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]-rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator*(const FloatArray &rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]*rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator*(float rhs) const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = values_[i]*rhs;
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::Sqrt(const FloatArray &rhs)
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = std::sqrt(rhs[i]);
    }
    return result;
}
template<std::size_t N>
FloatArray<N> FloatArray<N>::Reciprocal(const FloatArray &rhs)
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = 1.0f / rhs[i];
    }
    return result;
}

template<std::size_t N>
FloatArray<N> FloatArray<N>::operator-() const
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = -values_[i];
    }
    return result;
}

using FourFloat = FloatArray<4>;
using EightFloat = FloatArray<8>;

#if defined(__SSE__)

template<>
FourFloat FourFloat::Sqrt(const FloatArray<4> &rhs);

template<>
FourFloat FourFloat::operator*(const FloatArray<4> &rhs) const;

template<>
FourFloat FourFloat::operator*(float rhs) const;

#endif

#if defined(__AVX2__)

template<>
EightFloat EightFloat::Sqrt(const EightFloat &rhs);

template<>
EightFloat EightFloat::operator*(const EightFloat &rhs) const;

template<>
EightFloat EightFloat::operator*(float rhs) const;

#endif

#if defined(__aarch64__)
template<>
FourFloat FourFloat::Sqrt(const FloatArray<4> &rhs);

template<>
FourFloat FourFloat::operator*(const FloatArray<4> &rhs) const;

template<>
FourFloat FourFloat::operator*(float rhs) const;

#endif
}