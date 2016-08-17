#pragma once
#include "stdafx.h"
#include "PE_Matrix.h"
namespace PE
{
	template <size_t, typename> class Vector;
	

	inline double pow(mpf_class a, double b)
	{
		return std::pow(a.get_d(), b);
	}
	
	inline double atan2(mpf_class a, mpf_class b)
	{
		return std::atan2(a.get_d(), b.get_d());
	}
	
template <size_t dim, typename T>
class Vector
{
public:
    Vector()
    {
        for (size_t i = 0; i < dim; i++)
        {
            this->m_vec[i] = T(0);
        }
    }
    Vector(std::initializer_list<T> list) {
        size_t i = 0;
        for (const T* it = begin(list); it != end(list); ++it) {
            this->m_vec[i] = *it;
            i++;
        }
    }
    
    Vector(const std::vector<T>& vec)
	{
		std::copy(vec.begin(), vec.end(), m_vec);
	}
    ~Vector()
    {

    }
    
    ///<summary>
    ///Uses the euklidian distance (2-norm) to return the distance
    ///</summary>
    ///<param name=max>The maximum dims to use</param>
    T absLength(int max = -1) const
    {
        //return sqrt(pow(x, T(2)) + pow(y, T(2)));
		//return sqrt(pow(x.get_d(), 2.0) + pow(y.get_d(), 2.0));
		T sum(0);
		if(max == -1) max = dim;
		for(size_t i = 0; i < size_t(max); i++)
			sum += pow(m_vec[i], 2.0);
		return sqrt(sum);
    }
    
    T distanceTo(const PE::Vector<dim, T>& point) const
	{
		return Vector<dim, T>(point - *this).absLength();
	}

    Vector<dim + 1, T> addRow(T val = T(0)) {
        Vector<dim + 1, T> ret;
        for (size_t i = 0; i < dim; i++)
            ret.m_vec[i] = this->m_vec[i];

        ret.m_vec[dim] = val;
        return ret;
    }

    Vector<dim - 1, T> removeRow() {
        Vector<dim - 1, T> ret;
        for (size_t i = 0; i < dim - 1; i++)
            ret.m_vec[i] = this->m_vec[i];
        return ret;
    }

    Vector<dim, T> operator+(const Vector<dim, T>& v) const
    {
        Vector<dim, T> ret;
        for (size_t i = 0; i < dim; i++)
        {
            ret.m_vec[i] = this->m_vec[i] + v.m_vec[i];
        }
        return ret;
    }

    Vector<dim, T> operator-(const Vector<dim, T>& v) const
    {
        Vector<dim, T> ret;
        for (size_t i = 0; i < dim; i++)
        {
            ret.m_vec[i] = this->m_vec[i] - v.m_vec[i];
        }
        return ret;
    }

    Vector<dim, T> operator*(const double s) const
    {
        Vector<dim, T> ret;
        for (size_t i = 0; i < dim; i++)
        {
            ret.m_vec[i] = this->m_vec[i] * s;
        }
        return ret;
    }
    Vector<dim, T> operator*(const mpf_class s) const
    {
        Vector<dim, T> ret;
        for (size_t i = 0; i < dim; i++)
        {
            ret.m_vec[i] = this->m_vec[i] * s;
        }
        return ret;
    }
    
    Vector<dim, T> operator/(const double s) const
    {
        Vector<dim, T> ret;
        for (size_t i = 0; i < dim; i++)
        {
            ret.m_vec[i] = this->m_vec[i] / s;
        }
        return ret;
    }
    
    Vector<dim, T> operator/(const mpf_class s) const
    {
        Vector<dim, T> ret;
        for (size_t i = 0; i < dim; i++)
        {
            ret.m_vec[i] = this->m_vec[i] / s;
        }
        return ret;
    }

    Vector<dim, T> operator=(const Vector<dim, T>& v)
    {
        for (size_t i = 0; i < dim; i++)
        {
            this->m_vec[i] = v.m_vec[i];
        }
        return *this;
    }
    
    T operator[](size_t index) const
    {
		return m_vec[index];
	}

    T dotProduct(const Vector<dim, T>& v) const
    {
        T res = T(0);
        for (size_t i = 0; i < dim; i++)
        {
            res += this->m_vec[i] * v->m_vec[i];
        }
        return res;
    }
    
    ///<summary>
    ///Returns 0 if there exists no x | x * other = this, x otherwise
    ///</summary>
    T getFactor(const Vector<dim, T>& other) const
    {
		T lastFactor = m_vec[0] / other[0];
		//TODO: Check if this doesn't have issues with precision
		for(size_t i = 1; i < dim; i++)
		{
			T tmp = m_vec[i] / other[i];
			if(abs(mpf_class(lastFactor - tmp)) > 0.001)
				return T(0);
		}
		return lastFactor;
	}
    
    ///<summary>
    ///Returns a rotated version of this.
    ///<param name="angle">The angle in rad</param>
    ///</summary>
    Vector<dim, T> rotated2D(double angle) const
    {
		PE::Matrix<T> rotmat(2, 2);
		rotmat.setData({std::cos(angle), -std::sin(angle), std::sin(angle), std::cos(angle)});
		
		return PE::Vector<2, T>(PE::Matrix<T>(rotmat * this->toCol()).getColumn(0));
	}

	double getAngle2D() const
	{
		assert(dim >= 2);
		return atan2(this->y, this->x);
	}
	

    Vector<3, T> crossProduct(Vector<3, T> v) const
    {
        Vector<3, T> ret;
        ret.x = this->y*v.z - this->z*v.y;
        ret.y = this->z*v.x - this->x*v.z;
        ret.z = this->x*v.y - this->y*v.x;

        return ret;
    }
    
    Vector<dim, T> normalized() const
    {
		PE::Vector<dim, T> ret;
		
		ret = *this / this->absLength();
		
		return ret;
	}
    
    PE::Matrix<T> toRow() const
    {
		PE::Matrix<T> m(dim, 1);
		m.setRow(0, std::vector<T>(m_vec));
		
		
		return m;
	}
	
	 PE::Matrix<T> toCol() const
    {
		PE::Matrix<T> m(1, dim);
		for(size_t i = 0; i < dim; i++)
			m.set(0, i, m_vec[i]);
		
		
		return m;
	}

    T& x = m_vec[0];
    T& y = m_vec[1];
    T& z = m_vec[2];

    T m_vec[dim];



};

typedef Vector<2, float> Vector2f;
typedef Vector<2, mpf_class> Vector2df;
};