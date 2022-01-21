#pragma once

#define _USE_MATH_DEFINES
#include <cmath>


class Shape
{
public:
    virtual ~Shape() = default;
    [[nodiscard]] virtual float Area() const = 0;
    [[nodiscard]] virtual float Perimeter() const = 0;

};

class Circle : public Shape
{
public:
    Circle(float radius) : radius_(radius)
    {
    }
    [[nodiscard]] float Area() const override
    {
        static constexpr float pi = M_PI;
        return radius_ * radius_ * pi;
    }
    [[nodiscard]] float Perimeter() const override
    {
        static constexpr float pi = M_PI;
        return 2.0f * radius_ * pi;
    }
private:
    float radius_ = 0.0f;
};

class Rect : public Shape
{
public:
    Rect(float width, float height) : width_(width), height_(height)
    {

    }
    [[nodiscard]] float Area() const override
    {
        return width_ * height_;
    }
    [[nodiscard]] float Perimeter() const override
    {
        return 2.0f * (width_ + height_);
    }
private:
    float width_ = 0.0f;
    float height_ = 0.0f;
};