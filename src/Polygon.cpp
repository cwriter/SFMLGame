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

PE::Vector2df Polygon::getVectorToPoint(const PE::Vector2df& point)
{
    if (this->m_points.size() == 0) return PE::Vector2df();
    //#TODO: This is pretty slow. Check for different, faster approaches
    PE::Vector2df ret;
    //Get the points with the minimum distance
    std::vector<PE::Vector2df> veccpy(m_points.size());
    //Copy the array to allow sorting
    std::copy(m_points.begin(), m_points.end(), veccpy.begin());

    std::sort(veccpy.begin(), veccpy.end(), [&](const PE::Vector2df& l, const PE::Vector2df& r)
    {
        return l.distanceTo(point) < r.distanceTo(point);
    });

    //Temporary 2 smallest values
    PE::Vector2df vecs[2];
    memset(vecs, 0, sizeof(SFG::Vector2f) * 2);

    //Sorted, now get the 2 smallest values
    for (size_t i = 0; i < veccpy.size() && i < 2; i++)
    {
        vecs[i] = veccpy[i];
    }

    //Now, create a line between those points
    PE::Vector2df dvec = vecs[1] - vecs[0];

    //Get the gradient
    auto m = dvec.y / dvec.x;

    //By that gradient, create the analytic equation y = mx + c
    auto c = vecs[0].y - vecs[0].x*m;

    //Get the second line 90� on the old gradient.
    auto m2 = -1.f / m;

    //This second line goes through 'point'
    auto c2 = point.y - point.x*m2;

    //So we have equations m*x+c=m2*x+c2, so we have an equation (m-m2)*x=c2-c -> x=(c2-c)/(m-m2)
    auto x = (c2 - c) / (m - m2);
    auto y = m*x + c;

    //Now, get the distance
    auto dist = std::sqrt(std::pow(mpf_class(x - point.x).get_d(), 2.) + std::pow(mpf_class(y - point.y).get_d(), 2.));
    auto min = std::min(dist, PE::Vector2df(point - vecs[0]).absLength().get_d());
    if (m_points.size() > 1)
    {
        min = std::min(min, PE::Vector2df(point - vecs[1]).absLength().get_d());
    }

    //'min' contains the minimum distance
    if (min == dist)
    {
        //Line distance
        ret.x = x - point.x;
        ret.y = y - point.y;
    }
    else if (min == PE::Vector2df(point - vecs[0]).absLength().get_d())
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
