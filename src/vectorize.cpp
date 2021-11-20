//
// Created by efarhan on 23/02/2021.
//

#include "vectorize.h"

namespace maths
{
#if defined(__SSE__)
template<>
FourFloat FourFloat::Sqrt(const FloatArray<4> &rhs)
{
    auto vs = _mm_loadu_ps(&rhs[0]);
    vs = _mm_sqrt_ps(vs);

    FourFloat result;
    _mm_storeu_ps(&result[0], vs);
    return result;
}

template<>
FourFloat FourFloat::operator*(const FloatArray<4> &rhs) const
{
    auto v1s = _mm_loadu_ps(values_.data());
    auto v2s = _mm_loadu_ps(rhs.values_.data());
    v1s = _mm_mul_ps(v1s, v2s);

    FourFloat result;
    _mm_storeu_ps(result.values_.data(), v1s);
    return result;
}

template<>
FourFloat FourFloat::operator*(float rhs) const
{
    auto v1s = _mm_loadu_ps(values_.data());
    auto v2 = _mm_load1_ps(&rhs);
    v1s = _mm_mul_ps(v1s, v2);

    FourFloat result;
    _mm_storeu_ps(result.values_.data(), v1s);
    return result;
}
#endif

#if defined(__AVX2__)
template<>
EightFloat EightFloat::Sqrt(const EightFloat &rhs)
{
    auto vs = _mm256_loadu_ps(rhs.values_.data());
    vs = _mm256_sqrt_ps(vs);

    EightFloat result;
    _mm256_storeu_ps(result.values_.data(), vs);
    return result;
}

template<>
EightFloat EightFloat::operator*(const EightFloat &rhs) const
{
    auto v1s = _mm256_loadu_ps(values_.data());
    auto v2s = _mm256_loadu_ps(rhs.values_.data());
    v1s = _mm256_mul_ps(v1s, v2s);

    EightFloat result;
    _mm256_storeu_ps(result.values_.data(), v1s);
    return result;
}

template<>
EightFloat EightFloat::operator*(float rhs) const
{
    auto v1s = _mm256_loadu_ps(values_.data());
    auto v2 = _mm256_broadcast_ss(&rhs);
    v1s = _mm256_mul_ps(v1s, v2);

    EightFloat result;
    _mm256_storeu_ps(result.values_.data(), v1s);
    return result;
}
#endif

#if defined(__aarch64__)
template<>
FourFloat FourFloat::Sqrt(const FloatArray<4> &rhs)
{
    auto vs = vld1q_f32(rhs.values_.data());
    vs = vsqrtq_f32(vs);

    FourFloat result;
    vst1q_f32(&result[0], vs);
    return result;
}

template<>
FourFloat FourFloat::operator*(const FloatArray<4> &rhs) const
{
    auto v1s = vld1q_f32(values_.data());
    auto v2s = vld1q_f32(rhs.values_.data());
    v1s = vmulq_f32(v1s, v2s);

    FourFloat result;
    vst1q_f32(&result[0], v1s);
    return result;
}

template<>
FourFloat FourFloat::operator*(float rhs) const
{
    auto v1s = vld1q_f32(values_.data());
    auto v2 = vld1q_dup_f32(&rhs);
    v1s = vmulq_f32(v1s, v2);

    FourFloat result;
    vst1q_f32(&result[0], v1s);
    return result;
}
#endif
}