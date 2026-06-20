#ifndef _VECTOR2_H_
#define _VECTOR2_H_

#include <cmath>

class Vector2
{
public:
    float x;
    float y;

public:
    Vector2() = default;
    ~Vector2() = default;

    Vector2(float x, float y) : x(x), y(y)
    {
    }

    Vector2 operator+(const Vector2 &position) const
    {
        return Vector2(x + position.x, y + position.y);
    }

    void operator+=(const Vector2 &position)
    {
        x += position.x, y += position.y;
    }

    Vector2 operator-(const Vector2 &position) const
    {
        return Vector2(x - position.x, y - position.y);
    }

    void operator-=(const Vector2 &position)
    {
        x -= position.x;
        y -= position.y;
    }

    Vector2 operator*(float val) const
    {
        return Vector2(x * val, y * val);
    }

    float operator*(const Vector2 &position) const
    {
        return x * position.x + y * position.y;
    }

    void operator*=(float val)
    {
        x *= val;
        y *= val;
    }

    float length() const
    {
        return std::sqrt(x * x + y * y);
    }

    Vector2 normalize() const
    {
        float len = length();

        if (len == 0)
        {
            return Vector2(0, 0);
        }

        return Vector2(x / len, y / len);
    }
};

#endif