#include <vector3.h>

#include <cmath>

namespace maths
{

Vec3f::Vec3f(const float *ptr)
{
    values[0] = ptr[0];
    values[1] = ptr[1];
    values[2] = ptr[2];
}
float Vec3f::Magnitude() const
{
    return std::sqrt(x*x+y*y+z*z);
}

Vec3f Vec3f::operator+(const Vec3f &v) const
{
    const Vec3f result(x+v.x, y+v.y, z+v.z);
    return result;
}

Vec3f Vec3f::operator-(const Vec3f &v) const
{
    const Vec3f result(x-v.x, y-v.y, z-v.z);
    return result;
}

Vec3f Vec3f::operator*(float f) const
{
    const Vec3f result(x*f, y*f, z*f);
    return result;
}

Vec3f Vec3f::operator/(float f) const
{
    const Vec3f result(x/f, y/f, z/f);
    return result;
}
Vec3f Vec3f::Normalized() const
{
    return *this/Magnitude();
}
float Vec3f::SqrMagnitude() const
{
    return Dot(*this, *this);
}

float Dot(const Vec3f &v1, const Vec3f &v2)
{
    return v1.x*v2.x+v1.y*v2.y+v1.z*v2.z;
}

#if defined(__SSE__)

#endif



}
