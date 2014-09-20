#ifndef DYB_POINT2D
#define DYB_POINT2D

#include<cmath>

namespace dyb
{

    template<class T>
    T abs(T num)
    {
        return num >= 0 ? num : -num;
    }

    template<class T>
    class point2d
    {
    public:
        T x, y;
        point2d(T _x, T _y) :x(_x), y(_y) {}
        point2d(){}
        double norm()const
        {
            return std::sqrt(x*x + y*y);
        }

        point2d & operator += (const point2d & rhs)
        {
            x += rhs.x;
            y += rhs.y;
            return *this;
        }

        point2d & operator -= (const point2d & rhs)
        {
            x -= rhs.x;
            y -= rhs.y;
            return *this;
        }

        point2d & operator *= (T rhs)
        {
            x *= rhs;
            y *= rhs;
            return *this;
        }

        point2d & operator /= (T rhs)
        {
            x /= rhs;
            y /= rhs;
            return *this;
        }

        point2d & operator = (const point2d & rhs)
        {
            x = rhs.x; y = rhs.y;
            return *this;
        }

        friend bool operator == (const point2d & lhs, const point2d & rhs)
        {
            return abs(lhs.x - rhs.x) < 0.000001
                && abs(lhs.y - rhs.y) < 0.000001;
        }
    };

    // hack
    template<class T>
    point2d<T> create_point(T x, T y)
    {
        return point2d<T>(x, y);
    }

    template<class LHS, class RHS>
    auto operator + (const point2d<LHS> &lhs, const point2d<RHS> &rhs)
        -> decltype(create_point(lhs.x + rhs.x, lhs.y + rhs.y))
    {
        return create_point(lhs.x + rhs.x, lhs.y + rhs.y);
    }

    template<class LHS, class RHS>
    auto operator - (const point2d<LHS> &lhs, const point2d<RHS> &rhs)
        -> decltype(create_point(lhs.x - rhs.x, lhs.y - rhs.y))
    {
        return create_point(lhs.x - rhs.x, lhs.y - rhs.y);
    }

    template<class LHS, class RHS>
    auto operator * (const point2d<LHS> &lhs, const point2d<RHS> &rhs)
        -> decltype(lhs.x*rhs.x + lhs.y*rhs.y)
    {
        return lhs.x*rhs.x + lhs.y*rhs.y;
    }

    template<class LHS, class RHS>
    auto operator * (const point2d<LHS> &lhs, RHS rhs)
        -> decltype(create_point(lhs.x*rhs, lhs.y*rhs))
    {
        return create_point(lhs.x*rhs, lhs.y*rhs);
    }

    template<class LHS, class RHS>
    auto operator * (LHS lhs, const point2d<RHS> &rhs)
        -> decltype(create_point(lhs*rhs.x, lhs*rhs.y))
    {
        return create_point(lhs*rhs.x, lhs*rhs.y);
    }

    template<class LHS, class RHS>
    auto operator / (const point2d<LHS> &lhs, RHS rhs)
        -> decltype(create_point(lhs.x / rhs, lhs.y / rhs))
    {
        return create_point(lhs.x / rhs, lhs.y / rhs);
    }

}

#endif