#pragma once
#include "stdafx.h"
#include "internal_Rect.h"

namespace SFG
{
	template<typename T>
	class Vector2 : public SFG::iVector2<T>/*, public sf::Vector2<T>*/
	{
	public:
		Vector2<T>()
		{
			SFG::iVector2<T>::x = T(0);
			SFG::iVector2<T>::y = T(0);
		}

		Vector2<T>(const iVector2<T>& vec)
		{
			SFG::iVector2<T>::x = vec.x;
			SFG::iVector2<T>::y = vec.y;
		}

		Vector2<T>(float l, float t)
		{
			SFG::iVector2<T>::x = l;
			SFG::iVector2<T>::y = t;
		}
		Vector2<T>(const sf::Vector2<T>& vec)
		{
			SFG::iVector2<T>::x = vec.x;
			SFG::iVector2<T>::y = vec.y;
		}

		operator sf::Vector2<T>() const
		{
			return sf::Vector2f(SFG::iVector2<T>::x, SFG::iVector2<T>::y);
		}

		operator SFG::iVector2<T>() const
		{
			return SFG::iVector2<T>(SFG::iVector2<T>::x, SFG::iVector2<T>::y);
		}

		Vector2<T> operator=(const sf::Vector2<T>& in)
		{
			SFG::iVector2<T>::x = in.x;
			SFG::iVector2<T>::y = in.y;

			return *this;
		}

		T length() const {
			return sqrt(pow(SFG::iVector2<T>::x, 2.0f) + pow(SFG::iVector2<T>::y, 2.0f));
		}

		Vector2<T> trimmed(const T len)
		{
			return Vector2<T>(iVector2<T>::trimmed(len));
		}
		friend Vector2<T> operator+(const SFG::Vector2<T>& left, const SFG::Vector2<T>& right)
		{
			Vector2<T> ret;
			ret.x = left.x + right.x;
			ret.y = left.y + right.y;
			return ret;
		}

		/*friend Vector2<T> operator-(const SFG::Vector2<T>& left, const SFG::Vector2<T>& right)
		{
			Vector2<T> ret;
			ret.x = left.x - right.x;
			ret.y = left.y - right.y;
			return ret;
		}

		friend Vector2<T> operator+(const SFG::Vector2<T>& left, const SFG::iVector2<T>& right)
		{
			Vector2<T> ret;
			ret.x = left.x + right.x;
			ret.y = left.y + right.y;
			return ret;
		}*/

		//T x;
		//T y;
	};


	template<typename T>
	class Rect : public iRect<T>
	{
	public:
		Rect()
		{
			Rect(T(0), T(0), T(0), T(0));
		}
		Rect(T x, T y, T w, T h)
		{
			sf::Rect<T>(x, y, w, h);
		}

		Rect(const SFG::Vector2<T>& origin, const SFG::Vector2<T>& size)
		{
			this->left = origin.x;
			this->top = origin.y;
			this->width = size.x;
			this->height = size.y;
		}

		Rect(const sf::Rect<T>& r)
		{
			this->left = r.left;
			this->top = r.top;
			this->width = r.width;
			this->height = r.height;
		}

		operator sf::Rect<T>()
		{
			return sf::Rect<T>(left, top, width, height);
		}

		bool intersects(const sf::Rect<T>& r)
		{
			return sf::Rect<T>(left, top, width, height).intersects(r);
		}

		bool contains(const sf::Vector2<T>& v)
		{
			return sf::Rect<T>(left, top, width, height).contains(v);
		}

		inline T right() const { return left + width; }
		inline T bottom() const { return top + height; }
		inline SFG::Vector2<T> center() const { return sf::Vector2<T>(left + width / T(2), top + height / T(2)); }

		///<summary>
		///Create from diagonal. Position (x|y) is set to (0|0)
		///</summary>
		///<returns>The created Rect (being "this")</returns>
		Rect<T>* fromDiagonal(const SFG::Vector2<T>& diag);

		T distanceTo(T x, T y)
		{
			//distance is always rectangular, except at the vertices.
			if (left <= x && right() >= x)
			{
				//Vertically aligned. Get the correct distance by inspecting y values
				return std::max({
					top - y,
					y - bottom()
				});
			}
			if (top <= y && bottom() >= y)
			{
				//Horizontally aligned
				return std::max({
					left - x,
					x - right()
				});
			}

			//Now, handle the circles at the vertices (negative values don't exist here)
			return std::min({
				sqrt(pow(left - x, 2.f) + pow(top - y, 2.f)), //Q1
				sqrt(pow(x - right(), 2.f) + pow(top - y, 2.f)), //Q2
				sqrt(pow(x - right(), 2.f) + pow(y - bottom(), 2.f)), //Q3
				sqrt(pow(left - x, 2.f) + pow(y - bottom(), 2.f)) //Q4
			});
		}

		inline SFG::Vector2<T> topLeft() const { return sf::Vector2<T>(left, top); }

		T left;
		T top;
		T width;
		T height;
	};

	typedef Vector2<float> Vector2f;
	typedef Rect<float> FloatRect;
	typedef Rect<int> IntRect;
}
