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
	bool inside = false;
	getVectorToPoint(point, &inside);
	
	return inside;
}

void Polygon::draw(sf::RenderTarget& target, const sf::Color& color, float linewidth)
{	
	sf::VertexArray va(sf::LinesStrip);
	
	//This loop connects all points
	for(auto it = m_points.begin(); it != m_points.end(); ++it)
	{
		va.append(sf::Vertex(sf::Vector2f(it->x.get_d(), it->y.get_d()), color));
	}
	
	target.draw(va);
}


PE::Vector2df Polygon::getVectorToPoint(const PE::Vector2df& point, bool* inside) const
{
	//We assume a clockwise polygon (contents are on the right side of each line)
	
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
		
		//This scope is for point to point (rather than point to line) dists
		{
			//Create the next line
			Line nextline;
			nextline.point = vec[i % vec.size()];
			nextline.direction = vec[(i+1) % vec.size()] - vec[i % vec.size()];
			
			//so it's line-point-nextline. Now, get the line connecting point
			//with the current polygon vertex
			Line connection;
			connection.point = vec[i % vec.size()];
			connection.direction = point - vec[i % vec.size()];
			 
			//Now, compare angles.
			double a1 = nextline.direction.getAngle2D() + 2.0 * PI;
			double a2 = connection.direction.getAngle2D() + 2.0 * PI;
			double a3 = PE::Vector2df(PE::Vector2df({0,0}) - line.direction).getAngle2D() + 2.0 * PI;
			
			//Now, it must be that a3 < a2 < a1
			if(a3 < a2 && a2 < a1)
			{	
				#pragma omp critical
				{
					if(connection.direction.absLength() < ret.absLength())
					{
						ret = connection.direction;
						if(inside != nullptr)
							*inside = true;
					}
				}
			}
			else
			{
				#pragma omp critical
				{
					if(connection.direction.absLength() < ret.absLength())
					{
						ret = connection.direction;
						if(inside != nullptr)
							*inside = false;
					}
				}
			}
		}
		
		//Rotate by 90 deg = Pi/2 clockwise
		auto rightSide = line.direction.rotated2D(-PI/2.f);
		
		//Create a line from the point to be checked to the polygon line
		Line tmp = line;
		tmp.point = point;
		tmp.direction = PE::Vector2df({0,0}) - rightSide;
		
		//Check if it actually intersects
		double factor = 0.f;
		tmp.intersects(line, &factor);
		if(factor <= 0.f)
		{
			//Wrong side or no intersection (the latter is REALLY rare)
			#pragma omp critical
			{
				if(rightSide.absLength() < ret.absLength())
				{
					if(inside != nullptr)
						*inside = false;
				}
			}
		}
		else
		{
			//YAAAAAY, we have an intersection!
			//Replace if shorter
			#pragma omp critical
			{
				if(rightSide.absLength() < ret.absLength())
				{
					ret = rightSide;
					if(inside != nullptr)
						*inside = true;
				}
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
