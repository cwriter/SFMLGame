#pragma once

namespace PE
{

template <typename T>
class Matrix
{
public:
    Matrix(size_t width, size_t height) : m_width(width), m_height(height)
    {
        m_data.resize(width*height, T(0));
    }
    
    Matrix<T> operator+(const Matrix<T>& other)
	{
		assert(m_width == other.m_width);
		assert(m_height == other.m_height);
		
        Matrix<T> ret(m_width, m_height);
		
		//Add index for index
		for(size_t i = 0; i < m_width * m_height; i++)
		{
			ret.m_data[i] = this->m_data[i] + other.m_data[i];
		}
		
		return ret;
	}

    Matrix<T> operator*(const Matrix<T>& other)
    {
        assert(m_width == other.m_height);
        Matrix<T> ret(m_width, other.m_height);

        for (size_t row_i = 0; row_i < m_height; row_i++)
        {
            for (size_t col_i = 0; col_i < other.m_width; col_i++)
            {
                //Sum up
                T sum(0);
                for (size_t c = 0; c < m_width; c++)
                {
                    sum += T(this->get(c, row_i) * other.get(col_i, c));
                }

                //Do the calculations
                ret.set(col_i, row_i, sum);
            }
        }

        return ret;
    }
    
    void setRow(size_t index, const std::vector<T>& row)
	{
		assert(row.size() == m_width);
		//overwrite the elements
		std::copy(row.begin(), row.end(), m_data.begin() + index * m_width);
	}
	
	void setData(const std::vector<T>& data)
	{
		assert(data.size() == m_data.size());
		std::copy(data.begin(), data.end(), m_data.begin());
	}

    ///<summary>
    ///Compares if two matrices are equal.
    ///</summary>
    bool operator==(const Matrix<T>& other) const
    {
        if (other.m_width != m_width || other.m_height != m_height) return false;
        //Compare bytes
        if (memcmp(m_data.data(), other.m_data.data(), m_height*m_width*sizeof(T)) == 0)
            return true; // return true if no differences have been found
        else
            return false;// otherwise just return false
    }

    ///<summary>
    ///Sets the entry at (x,y) to value
    ///</summary>
    void set(size_t x, size_t y, T value)
    {
        assert(x < m_width);
        assert(y < m_height);

        m_data[y*m_width + x] = value;
    }

    ///<summary>
    ///Returns the entry at (x,y).
    ///</summary>
    T get(size_t x, size_t y) const
    {
        assert(x < m_width);
        assert(y < m_height);

        return m_data[y*m_width + x];
    }

    ///<summary>
    ///Very time-consuming operation (compared to getRow())
    ///</summary>
    std::vector<T> getColumn(size_t index)
    {
        assert(index < m_width);
        std::vector<T> ret(m_height);
        for (size_t i = 0; i < m_height; i++)
        {
            ret[i] = m_data[index + m_width*i];
        }

        return ret;
    }
    ///<summary>
    ///Gets a row
    ///</summary>
    std::vector<T> getRow(size_t index)
    {
        assert(index < m_height);
        std::vector<T> ret(m_data.begin() + index*m_width, m_data.begin() + index*m_width + m_width);

        return ret;
    }

    size_t m_width;
    size_t m_height;
    std::vector<T> m_data; //Formatted as lines pinned together (line1line2...)
};
};