#include "stdafx.h"
#include "Polygon.h"



namespace SFG
{
Polygon::Polygon()
{
}


Polygon::~Polygon()
{
}

bool Polygon::contains(const PE::Vector2df& point) const
{
	//We assume the polygon's contents are always on the right side
	//of the connection lines (clockwise polygon)
	
	
	
	return false;
}


PE::Vector2df Polygon::getVectorToPoint(const PE::Vector2df& point)
{
	//We assume a clockwise polygon (contents are on the right side of each line
	
	//We're lazy as fuck and will copy the lines first
	//(this will make openmp usage easier / even possible)
	std::vector<PE::Vector2df> vec(m_points.size());
	std::copy(m_points.begin(), m_points.end(), vec.begin());
	
	//Set the maximal values first
    PE::Vector2df ret({std::numeric_limits<mpf_class>::max(),std::numeric_limits<mpf_class>::max()});
    
	//Create a vector each time
	#pragma omp parallel for
	for(size_t i = 1; i <= vec.size(); i++)
	{
		//Create the line
		Line line;
		line.point = vec[i-1];
		line.direction = vec[i % vec.size()] - vec[i-1];
		
		//Rotate by 90 deg = Pi/2 clockwise
		auto rightSide = line.direction.rotated2D(-PI/2.f);
		
		//Check if the "normal" vector can catch up to the point
		Line tmp = line;
		tmp.direction = -rightSide;
		//TODO: Finish implementation
		
		//Replace if shorter
		#pragma omp critical
		{
			if(rightSide.absLength() < ret.absLength())
			{
				ret = rightSide;
			}
		}
		
	}
	
	return ret;
}

SFG::Pointer<Polygon> Polygon::transrotatedPolygon(double rot, const PE::Vector2df& trans) const
{
	PE::Matrix<mpf_class> rotmat(2, 2);
		
	rotmat.setData({ cos(rot), sin(rot), -sin(rot), cos(rot) });
	
	std::vector<PE::Vector2df> lst(m_points.size());
	
	//Copy the list first
	std::copy(m_points.begin(), m_points.end(), lst.begin());
		
	//rotate and translate (Use openmp if we can to speed it up)
	//(it could take quite a while to go through > 100 points and transrotate them)

#pragma omp parallel for simd
	for(size_t i = 0; i < lst.size(); i++)
	{
		auto& b = lst[i];
		b = PE::Vector2df((rotmat * b.toCol() + trans.toCol()).getColumn(0));
	}
	
	return SFG::Pointer<Polygon>(new Polygon(lst));
}

};
