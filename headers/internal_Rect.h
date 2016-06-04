#pragma once
#include <math.h>
#include <algorithm>
#ifndef PI
#define PI 3.1415926535897932384626433832795
#endif

namespace SFG
{

///<summary>
///The internal class (used for testing)
///</summary>
template<typename T>
class iVector2
{
public:
    iVector2()
    {
        x = T(0);
        y = T(0);
    }
    iVector2(T x, T y) : x(x), y(y)
    {
    }

    T length() const {
        return sqrt(pow(x, 2.0f) + pow(y, 2.0f));
    }

    iVector2<T> operator+(const SFG::iVector2<T>& right) const
    {
        iVector2<T> ret;
        ret.x = this->x + right.x;
        ret.y = this->y + right.y;
        return ret;
    }

    iVector2<T> operator-(const SFG::iVector2<T>& right) const
    {
        iVector2<T> ret;
        ret.x = this->x - right.x;
        ret.y = this->y - right.y;
        return ret;
    }

    iVector2<T> operator-(SFG::iVector2<T> right)
    {
        iVector2<T> ret;
        ret.x = this->x - right.x;
        ret.y = this->y - right.y;
        return ret;
    }

    ///<summary>
    ///Returns the vector rotated by 'degrees' degrees. The old vector remains unchanged
    ///</summary>
    ///<param name="degrees">The amount of degrees to rotate clockwise</param>
    iVector2<T> rotated(double degrees) const
    {
        iVector2<T> red; //r(otat)ed

        double cangle = atan(this->y / this->x); //Get the current angle

        double cdangle = cangle*180. / PI; //Current angle in degrees

        double ndangle = cdangle - degrees; //- is for the clockwise rotation. Counterclockwise, + would be fine

        double nangle = ndangle*PI / 180.;

        red.x = T(cos(nangle) * length()); //Get the rotated values
        red.y = T(sin(nangle) * length());

        return red;
    }

    ///<summary>
    ///Calculates the distance to a different point
    ///</summary>
    T distanceTo(const iVector2<T>& point) const
    {
        iVector2<T> tmp = point - *this;
        return tmp.length();
    }

    iVector2<T> operator*(const T factor) const
    {
        iVector2<T> ret = *this;

        ret.x *= factor;
        ret.y *= factor;

        return ret;
    }

    iVector2<T> operator/(const T divisor) const
    {
        iVector2<T> ret = *this;

        ret.x /= divisor;
        ret.y /= divisor;

        return ret;
    }

    ///<summary>
    ///Trims 'this' to a vector of the given length without altering the direction.
    ///</summary>
    iVector2<T> trimmed(const T len) const
    {
        //If the 'this'.length() is 0, any scaling will stay 0
        if (length() == T(0)) return *this;

        iVector2<T> ret = *this;

        ret = ret * (len/length());

        assert(abs(ret.length() - len) <= T(1));

        return ret;
    }


//#ifdef SFG_TESTING
    T x;
    T y;
//#endif
};

template<typename T>
class iRect
{
public:
    iRect()
    {
        this->top = T(0);
        this->left = T(0);
        this->width = T(0);
        this->height = T(0);
    }
    iRect(T x, T y, T w, T h)
        : left(x), top(y), width(w), height(h)
    {

    }

    inline T right() const {
        return left + width;
    }
    inline T bottom() const {
        return top + height;
    }
    inline SFG::iVector2<T> center() const {
        return SFG::iVector2<T>(left + width / T(2), top + height / T(2));
    }

    ///<summary>
    ///Create from diagonal. Position (x|y) is set to (0|0)
    ///</summary>
    ///<returns>The created Rect (being "this")</returns>
    iRect<T>* fromDiagonal(const SFG::iVector2<T>& diag)
    {
        this->width = diag.x;
        this->height = diag.y;

        return this;
    }

    ///<summary>
    ///Returns the area of the rect.
    ///</summary>
    T area() const
    {
        return std::abs(this->width*this->height);
    }

    T distanceTo(T x, T y) const
    {
        //distance is always rectangular, except at the vertices.
        if (left <= x && right() >= x)
        {
            //Vertically aligned. Get the correct distance by inspecting y values
            return std::max( {
                top - y,
                y - bottom()
            });
        }
        if (top <= y && bottom() >= y)
        {
            //Horizontally aligned
            return std::max( {
                left - x,
                x - right()
            });
        }

        //Now, handle the circles at the vertices (negative values don't exist here)
        return std::min( {
            sqrt(pow(left - x, 2.f) + pow(top - y, 2.f)), //Q1
            sqrt(pow(x - right(), 2.f) + pow(top - y, 2.f)), //Q2
            sqrt(pow(x - right(), 2.f) + pow(y - bottom(), 2.f)), //Q3
            sqrt(pow(left - x, 2.f) + pow(y - bottom(), 2.f)) //Q4
        });
    }

    inline SFG::iVector2<T> topLeft() const {
        return SFG::iVector2<T>(left, top);
    }

    T left;
    T top;
    T width;
    T height;
};
};
