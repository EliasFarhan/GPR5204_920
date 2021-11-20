#pragma once

#include "vector4.h"
namespace maths
{
class Mat4f
{
public:
	Mat4f() = default;
	~Mat4f() = default;
	Mat4f(const Mat4f& m) noexcept
	{
		values_ = m.values_;
	}

	Mat4f(const std::array<Vec4f, 4>& v)
	{
		values_ = v;
	}
	Vec4f& operator[](std::size_t idx) { return values_[idx]; }
	const Vec4f& operator[](std::size_t idx) const { return values_[idx]; }
	float operator()(size_t column, size_t row) const { return values_[column][row]; }
	float& operator()(size_t column, size_t row) { return values_[column][row]; }
	[[nodiscard]] Mat4f Transpose() const;
	[[nodiscard]] Mat4f operator+(const Mat4f& rhs) const;
	[[nodiscard]] Mat4f operator-(const Mat4f& rhs) const;
	[[nodiscard]] Vec4f operator*(const Vec4f& rhs) const;
	Mat4f operator*(float rhs) const noexcept;
	Mat4f operator/(float rhs) const noexcept;
	Mat4f operator*(const Mat4f& rhs) const noexcept;
	Mat4f MultIntrinsics(const Mat4f& rhs) const noexcept;
	[[nodiscard]] float Determinant() const;
	[[nodiscard]] Mat4f Inverse() const;
	[[nodiscard]] Mat4f InverseOpti() const;

	static float MatrixDiff(const Mat4f& m1, const Mat4f& m2);

	const static Mat4f Identity;
	const static Mat4f Zero;
private:
	std::array<Vec4f, 4> values_{};
};

const inline Mat4f Mat4f::Identity = Mat4f(
	std::array<Vec4f, 4>
{
	Vec4f(1, 0, 0, 0),
		Vec4f(0, 1, 0, 0),
		Vec4f(0, 0, 1, 0),
		Vec4f(0, 0, 0, 1)});


const inline Mat4f Mat4f::Zero = Mat4f(
	std::array<Vec4f, 4>
{
	Vec4f::zero(),
    Vec4f::zero(),
    Vec4f::zero(),
    Vec4f::zero()
});


}