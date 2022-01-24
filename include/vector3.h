#pragma once
#include <array>
#include <cmath>
#include "instrinsics.h"
#include "vectorize.h"

namespace maths
{
union Vec3f
{
    struct
    {
        float x,y,z;
    };
    std::array<float, 3> values{};

    constexpr Vec3f() : x(0), y(0), z(0){}
    constexpr Vec3f(float x, float y, float z) : x(x), y(y), z(z){}
    Vec3f(const float* ptr);

    Vec3f operator+(const Vec3f& v) const;
    Vec3f operator-(const Vec3f& v) const;
    Vec3f operator*(float f) const;
    Vec3f operator/(float f) const;

    float& operator[](std::size_t idx) { return values[idx]; }
    const float& operator[](std::size_t idx) const { return values[idx]; }

    [[nodiscard]] float Magnitude() const;
    [[nodiscard]] float SqrMagnitude() const;
    [[nodiscard]] Vec3f Normalized() const;
    static constexpr Vec3f zero(){return {};}
    static constexpr Vec3f one(){return {1,1,1};}
};

float Dot(const Vec3f& v1, const Vec3f& v2);

template<std::size_t N>
class NVec3f
{
public:
    NVec3f() = default;
    NVec3f(const Vec3f *ptr);
    NVec3f(Vec3f v);
    NVec3f(const std::array<float, N>& xs, const std::array<float, N>& ys, const std::array<float, N>& zs) :
        xs(xs), ys(ys), zs(zs){}
    NVec3f(const FloatArray<N> & xs, const FloatArray<N> & ys, const FloatArray<N> & zs) :
        xs(xs.array()), ys(ys.array()), zs(zs.array()){}

    [[nodiscard]] FloatArray<N> Magnitude() const;
    [[nodiscard]] FloatArray<N> SqrMagnitude() const;
    static FloatArray<N> Dot(const NVec3f &v1, const NVec3f &v2);
    [[nodiscard]] NVec3f Normalized() const;

    [[nodiscard]] FloatArray<N> Xs() const {return xs;}
    [[nodiscard]] FloatArray<N> Ys() const {return ys;}
    [[nodiscard]] FloatArray<N> Zs() const {return zs;}

    [[nodiscard]] std::array<float, N>& Xs() {return xs;}
    [[nodiscard]] std::array<float, N>& Ys() {return ys;}
    [[nodiscard]] std::array<float, N>& Zs() {return zs;}

    NVec3f operator+(const NVec3f& v) const;
    NVec3f operator-(const NVec3f& v) const;
    NVec3f operator*(const FloatArray<N> & rhs) const;
    NVec3f operator*(float value) const;
    NVec3f operator/(const FloatArray<N> & rhs) const;

    [[nodiscard]] std::array<Vec3f, N> vectors() const;
private:
    std::array<float, N> xs{};
    std::array<float, N> ys{};
    std::array<float, N> zs{};
};

template<std::size_t N>
NVec3f<N>::NVec3f(const Vec3f *ptr)
{
    for(std::size_t i = 0; i < N; i++)
    {
        xs[i] = ptr[i].x;
        ys[i] = ptr[i].y;
        zs[i] = ptr[i].z;
    }
}
template<std::size_t N>
NVec3f<N>::NVec3f(Vec3f v)
{
    for(std::size_t i = 0; i < N; i++)
    {
        xs[i] = v.x;
        ys[i] = v.y;
        zs[i] = v.z;
    }
}
template<std::size_t N>
FloatArray<N> NVec3f<N>::Dot(const NVec3f<N> &v1, const NVec3f<N> &v2)
{
    FloatArray<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i] = v1.xs[i]*v2.xs[i]+v1.ys[i]*v2.ys[i]+v1.zs[i]*v2.zs[i];
    }
    return result;
}
template<std::size_t N>
NVec3f<N> NVec3f<N>::operator+(const NVec3f &v) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]+v.xs[i];
        result.ys[i] = ys[i]+v.ys[i];
        result.zs[i] = zs[i]+v.zs[i];
    }
    return result;
}
template<std::size_t N>
NVec3f<N> NVec3f<N>::operator-(const NVec3f &v) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]-v.xs[i];
        result.ys[i] = ys[i]-v.ys[i];
        result.zs[i] = zs[i]-v.zs[i];
    }
    return result;
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::operator*(const FloatArray<N> &rhs) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]*rhs[i];
        result.ys[i] = ys[i]*rhs[i];
        result.zs[i] = zs[i]*rhs[i];
    }
    return result;
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::operator*(float rhs) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]*rhs;
        result.ys[i] = ys[i]*rhs;
        result.zs[i] = zs[i]*rhs;
    }
    return result;
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::operator/(const FloatArray<N> &rhs) const
{
    NVec3f<N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result.xs[i] = xs[i]/rhs[i];
        result.ys[i] = ys[i]/rhs[i];
        result.zs[i] = zs[i]/rhs[i];
    }
    return result;
}
template<std::size_t N>
FloatArray<N> NVec3f<N>::SqrMagnitude() const
{
    return Dot(*this, *this);
}

template<std::size_t N>
FloatArray<N> NVec3f<N>::Magnitude() const
{
    return FloatArray<N>::Sqrt(SqrMagnitude());
}

template<std::size_t N>
NVec3f<N> NVec3f<N>::Normalized() const
{
    return *this/Magnitude();
}

template<std::size_t N>
std::array<Vec3f, N> NVec3f<N>::vectors() const
{
    std::array<Vec3f, N> result;
    for(std::size_t i = 0; i < N; i++)
    {
        result[i].x = xs[i];
        result[i].y = ys[i];
        result[i].z = zs[i];
    }
    return result;
}

using FourVec3f = NVec3f<4>;
using EightVec3f = NVec3f<8>;

#if defined(__SSE__)

template<>
inline FourVec3f FourVec3f::operator+(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = _mm_loadu_ps(xs.data());
    auto y1 = _mm_loadu_ps(ys.data());
    auto z1 = _mm_loadu_ps(zs.data());

    const auto x2 = _mm_loadu_ps(v.xs.data());
    const auto y2 = _mm_loadu_ps(v.ys.data());
    const auto z2 = _mm_loadu_ps(v.zs.data());

    x1 = _mm_add_ps(x1, x2);
    y1 = _mm_add_ps(y1, y2);
    z1 = _mm_add_ps(z1, z2);

    _mm_storeu_ps(fv3f.xs.data(), x1);
    _mm_storeu_ps(fv3f.ys.data(), y1);
    _mm_storeu_ps(fv3f.zs.data(), z1);
    return fv3f;
}


template<>
inline FourVec3f FourVec3f::operator-(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = _mm_loadu_ps(xs.data());
    auto y1 = _mm_loadu_ps(ys.data());
    auto z1 = _mm_loadu_ps(zs.data());

    const auto x2 = _mm_loadu_ps(v.xs.data());
    const auto y2 = _mm_loadu_ps(v.ys.data());
    const auto z2 = _mm_loadu_ps(v.zs.data());

    x1 = _mm_sub_ps(x1, x2);
    y1 = _mm_sub_ps(y1, y2);
    z1 = _mm_sub_ps(z1, z2);

    _mm_storeu_ps(fv3f.xs.data(), x1);
    _mm_storeu_ps(fv3f.ys.data(), y1);
    _mm_storeu_ps(fv3f.zs.data(), z1);
    return fv3f;
}


template<>
inline FourVec3f FourVec3f::operator*(const FourFloat& rhs) const
{
    FourVec3f result;
    auto x = _mm_loadu_ps(xs.data());
    auto y = _mm_loadu_ps(ys.data());
    auto z = _mm_loadu_ps(zs.data());
    const auto v = _mm_loadu_ps(&rhs[0]);

    x = _mm_mul_ps(x, v);
    y = _mm_mul_ps(y, v);
    z = _mm_mul_ps(z, v);

    _mm_storeu_ps(result.Xs().data(), x);
    _mm_storeu_ps(result.Ys().data(), y);
    _mm_storeu_ps(result.Zs().data(), z);
    return result;

}

template<>
inline FourVec3f FourVec3f::operator/(const FourFloat& rhs) const
{
    FourVec3f result;
    auto x = _mm_loadu_ps(xs.data());
    auto y = _mm_loadu_ps(ys.data());
    auto z = _mm_loadu_ps(zs.data());
    const auto v = _mm_loadu_ps(&rhs[0]);

    x = _mm_div_ps(x, v);
    y = _mm_div_ps(y, v);
    z = _mm_div_ps(z, v);

    _mm_storeu_ps(result.Xs().data(), x);
    _mm_storeu_ps(result.Ys().data(), y);
    _mm_storeu_ps(result.Zs().data(), z);
    return result;
}

template<>
inline FourVec3f FourVec3f::operator*(float value) const
{
    FourVec3f result;
    auto x = _mm_loadu_ps(xs.data());
    auto y = _mm_loadu_ps(ys.data());
    auto z = _mm_loadu_ps(zs.data());
    const auto v = _mm_load1_ps(&value);

    x = _mm_mul_ps(x, v);
    y = _mm_mul_ps(y, v);
    z = _mm_mul_ps(z, v);

    _mm_storeu_ps(result.Xs().data(), x);
    _mm_storeu_ps(result.Ys().data(), y);
    _mm_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
inline FourFloat FourVec3f::Dot(const FourVec3f& v1, const FourVec3f& v2)
{
    auto x1 = _mm_loadu_ps(v1.xs.data());
    auto y1 = _mm_loadu_ps(v1.ys.data());
    auto z1 = _mm_loadu_ps(v1.zs.data());

    auto x2 = _mm_loadu_ps(v2.xs.data());
    auto y2 = _mm_loadu_ps(v2.ys.data());
    auto z2 = _mm_loadu_ps(v2.zs.data());

    x1 = _mm_mul_ps(x1, x2);
    y1 = _mm_mul_ps(y1, y2);
    z1 = _mm_mul_ps(z1, z2);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);

    FourFloat result;
    _mm_storeu_ps(&result[0], x1);
    return result;
}
template<>
inline FourFloat FourVec3f::Magnitude() const
{
    auto x1 = _mm_loadu_ps(xs.data());
    auto y1 = _mm_loadu_ps(ys.data());
    auto z1 = _mm_loadu_ps(zs.data());

    x1 = _mm_mul_ps(x1, x1);
    y1 = _mm_mul_ps(y1, y1);
    z1 = _mm_mul_ps(z1, z1);

    x1 = _mm_add_ps(x1, y1);
    x1 = _mm_add_ps(x1, z1);
    x1 = _mm_sqrt_ps(x1);

    FourFloat result;
    _mm_storeu_ps(&result[0], x1);
    return result;
}
#endif


#if defined(__aarch64__)
template<>
inline FourVec3f FourVec3f::operator+(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(v.xs.data());
    const auto y2 = vld1q_f32(v.ys.data());
    const auto z2 = vld1q_f32(v.zs.data());

    x1 = vaddq_f32(x1, x2);
    y1 = vaddq_f32(y1, y2);
    z1 = vaddq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
inline FourVec3f FourVec3f::operator-(const FourVec3f& v) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(v.xs.data());
    const auto y2 = vld1q_f32(v.ys.data());
    const auto z2 = vld1q_f32(v.zs.data());

    x1 = vsubq_f32(x1, x2);
    y1 = vsubq_f32(y1, y2);
    z1 = vsubq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
inline FourVec3f FourVec3f::operator*(const FourFloat& values) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(&values[0]);
    const auto y2 = vld1q_f32(&values[0]);
    const auto z2 = vld1q_f32(&values[0]);

    x1 = vmulq_f32(x1, x2);
    y1 = vmulq_f32(y1, y2);
    z1 = vmulq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;

}

template<>
inline FourVec3f FourVec3f::operator/(const FourFloat& values) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto x2 = vld1q_f32(&values[0]);
    const auto y2 = vld1q_f32(&values[0]);
    const auto z2 = vld1q_f32(&values[0]);

    x1 = vdivq_f32(x1, x2);
    y1 = vdivq_f32(y1, y2);
    z1 = vdivq_f32(z1, z2);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
inline FourVec3f FourVec3f::operator*(float value) const
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    const auto v = vld1q_dup_f32(&value);

    x1 = vmulq_f32(x1, v);
    y1 = vmulq_f32(y1, v);
    z1 = vmulq_f32(z1, v);

    vst1q_f32(fv3f.xs.data(), x1);
    vst1q_f32(fv3f.ys.data(), y1);
    vst1q_f32(fv3f.zs.data(), z1);
    return fv3f;
}

template<>
inline FourFloat FourVec3f::Dot(const FourVec3f& v1, const FourVec3f& v2)
{
    FourVec3f fv3f;
    auto x1 = vld1q_f32(v1.xs.data());
    auto y1 = vld1q_f32(v1.ys.data());
    auto z1 = vld1q_f32(v1.zs.data());

    const auto x2 = vld1q_f32(v2.xs.data());
    const auto y2 = vld1q_f32(v2.ys.data());
    const auto z2 = vld1q_f32(v2.zs.data());


    x1 = vmulq_f32(x1, x2);
    y1 = vmulq_f32(y1, y2);
    z1 = vmulq_f32(z1, z2);

    x1 = vaddq_f32(x1, y1);
    x1 = vaddq_f32(x1, z1);

    FourFloat result;
    vst1q_f32(&result[0], x1);
    return result;
}
template<>
inline FourFloat FourVec3f::Magnitude() const
{
    auto x1 = vld1q_f32(xs.data());
    auto y1 = vld1q_f32(ys.data());
    auto z1 = vld1q_f32(zs.data());

    x1 = vmulq_f32(x1, x1);
    y1 = vmulq_f32(y1, y1);
    z1 = vmulq_f32(z1, z1);

    x1 = vaddq_f32(x1, y1);
    x1 = vaddq_f32(x1, z1);
    x1 = vsqrtq_f32(x1);

    FourFloat result;
    vst1q_f32(&result[0], x1);
    return result;
}

#endif


#if defined(__AVX2__)
template<>
inline EightVec3f EightVec3f::operator+(const EightVec3f& v) const
{
    EightVec3f result;

    auto x1 = _mm256_loadu_ps(xs.data());
    auto y1 = _mm256_loadu_ps(xs.data());
    auto z1 = _mm256_loadu_ps(xs.data());

    const auto x2 = _mm256_loadu_ps(v.xs.data());
    const auto y2 = _mm256_loadu_ps(v.xs.data());
    const auto z2 = _mm256_loadu_ps(v.xs.data());

    x1 = _mm256_add_ps(x1, x2);
    y1 = _mm256_add_ps(y1, y2);
    z1 = _mm256_add_ps(z1, z2);

    _mm256_storeu_ps(result.xs.data(), x1);
    _mm256_storeu_ps(result.ys.data(), y1);
    _mm256_storeu_ps(result.zs.data(), z1);

    return result;
}
template<>
inline EightVec3f EightVec3f::operator-(const EightVec3f& v) const
{
    EightVec3f result;

    auto x1 = _mm256_loadu_ps(xs.data());
    auto y1 = _mm256_loadu_ps(xs.data());
    auto z1 = _mm256_loadu_ps(xs.data());

    const auto x2 = _mm256_loadu_ps(v.xs.data());
    const auto y2 = _mm256_loadu_ps(v.xs.data());
    const auto z2 = _mm256_loadu_ps(v.xs.data());

    x1 = _mm256_sub_ps(x1, x2);
    y1 = _mm256_sub_ps(y1, y2);
    z1 = _mm256_sub_ps(z1, z2);

    _mm256_storeu_ps(result.xs.data(), x1);
    _mm256_storeu_ps(result.ys.data(), y1);
    _mm256_storeu_ps(result.zs.data(), z1);

    return result;
}
template<>
inline EightVec3f EightVec3f::operator*(const EightFloat& rhs) const
{
    EightVec3f result;
    auto x = _mm256_loadu_ps(xs.data());
    auto y = _mm256_loadu_ps(ys.data());
    auto z = _mm256_loadu_ps(zs.data());
    const auto v = _mm256_loadu_ps(&rhs[0]);

    x = _mm256_mul_ps(x, v);
    y = _mm256_mul_ps(y, v);
    z = _mm256_mul_ps(z, v);

    _mm256_storeu_ps(result.Xs().data(), x);
    _mm256_storeu_ps(result.Ys().data(), y);
    _mm256_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
inline EightVec3f EightVec3f::operator*(float value) const
{
    EightVec3f result;
    auto x = _mm256_loadu_ps(xs.data());
    auto y = _mm256_loadu_ps(ys.data());
    auto z = _mm256_loadu_ps(zs.data());
    const auto v = _mm256_broadcast_ss(&value);

    x = _mm256_mul_ps(x, v);
    y = _mm256_mul_ps(y, v);
    z = _mm256_mul_ps(z, v);

    _mm256_storeu_ps(result.Xs().data(), x);
    _mm256_storeu_ps(result.Ys().data(), y);
    _mm256_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
inline EightVec3f EightVec3f::operator/(const EightFloat& values) const
{
    EightVec3f result;
    auto x = _mm256_loadu_ps(xs.data());
    auto y = _mm256_loadu_ps(ys.data());
    auto z = _mm256_loadu_ps(zs.data());
    const auto v = _mm256_loadu_ps(&values[0]);

    x = _mm256_div_ps(x, v);
    y = _mm256_div_ps(y, v);
    z = _mm256_div_ps(z, v);

    _mm256_storeu_ps(result.Xs().data(), x);
    _mm256_storeu_ps(result.Ys().data(), y);
    _mm256_storeu_ps(result.Zs().data(), z);
    return result;
}
template<>
inline EightFloat EightVec3f::Dot(const EightVec3f& v1, const EightVec3f& v2)
{
    auto x1 = _mm256_loadu_ps(v1.xs.data());
    auto y1 = _mm256_loadu_ps(v1.ys.data());
    auto z1 = _mm256_loadu_ps(v1.zs.data());

    auto x2 = _mm256_loadu_ps(v2.xs.data());
    auto y2 = _mm256_loadu_ps(v2.ys.data());
    auto z2 = _mm256_loadu_ps(v2.zs.data());

    x1 = _mm256_mul_ps(x1, x2);
    y1 = _mm256_mul_ps(y1, y2);
    z1 = _mm256_mul_ps(z1, z2);

    x1 = _mm256_add_ps(x1, y1);
    x1 = _mm256_add_ps(x1, z1);

    EightFloat result;
    _mm256_storeu_ps(&result[0], x1);
    return result;
}
template<>
inline EightFloat EightVec3f::Magnitude() const
{
    auto x1 = _mm256_loadu_ps(xs.data());
    auto y1 = _mm256_loadu_ps(ys.data());
    auto z1 = _mm256_loadu_ps(zs.data());

    x1 = _mm256_mul_ps(x1, x1);
    y1 = _mm256_mul_ps(y1, y1);
    z1 = _mm256_mul_ps(z1, z1);

    x1 = _mm256_add_ps(x1, y1);
    x1 = _mm256_add_ps(x1, z1);
    x1 = _mm256_sqrt_ps(x1);

    EightFloat result;
    _mm256_storeu_ps(&result[0], x1);
    return result;
}


#endif
}
