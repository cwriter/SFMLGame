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

	SFG::Vector2f Polygon::getVectorToPoint(const SFG::Vector2f& point)
	{
		if (this->m_points.size() == 0) return SFG::Vector2f(0.f, 0.f);
		//#TODO: This is pretty slow. Check for different, faster approaches
		SFG::Vector2f ret;
		//Get the points with the minimum distance
		std::vector<SFG::Vector2f> veccpy(m_points.size());
		//Copy the array to allow sorting
		std::copy(m_points.begin(), m_points.end(), std::back_inserter(veccpy));

		std::sort(veccpy.begin(), veccpy.end(), [=](const SFG::Vector2f& l, const SFG::Vector2f& r)
		{
			return l.distanceTo(point) < r.distanceTo(point);
		});

		//Temporary 2 smallest values
		SFG::Vector2f vecs[2];
		memset(vecs, 0, sizeof(SFG::Vector2f) * 2);

		//Sorted, now get the 2 smallest values
		for (size_t i = 0; i < veccpy.size() && i < 2; i++)
		{
			vecs[i] = veccpy[i];
		}

		//Now, create a line between those points
		SFG::Vector2f dvec = vecs[1] - vecs[0];

		//Get the gradient
		float m = dvec.y / dvec.x;
		
		//By that gradient, create the analytic equation y = mx + c
		float c = vecs[0].y - vecs[0].x*m;

		//Get the second line 90° on the old gradient.
		float m2 = -1.f / m;

		//This second line goes through 'point'
		float c2 = point.y - point.x*m2;

		//So we have equations m*x+c=m2*x+c2, so we have an equation (m-m2)*x=c2-c -> x=(c2-c)/(m-m2)
		float x = (c2 - c) / (m - m2);
		float y = m*x + c;

		//Now, get the distance
		float dist = sqrt(std::pow(x - point.x, 2.f) + std::pow(y - point.y, 2.f));
		float min = std::min(dist, SFG::Vector2f(point - vecs[0]).length()); 
		if (m_points.size() > 1)
		{
			min = std::min(min, SFG::Vector2f(point - vecs[1]).length());
		}
		
		//'min' contains the minimum distance
		if (min == dist)
		{
			//Line distance
			ret.x = x - point.x;
			ret.y = y - point.y;
		}
		else if (min == SFG::Vector2f(point - vecs[0]).length())
		{
			//Closest point
			ret.x = vecs[0].x - point.x;
			ret.y = vecs[0].y - point.y;
		}
		else
		{
			//2nd-closest point
			ret.x = vecs[1].x - point.x;
			ret.y = vecs[1].y - point.y;
		}

		return ret;
	}
};
