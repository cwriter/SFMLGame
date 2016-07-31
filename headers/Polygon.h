#pragma once
#include "stdafx.h"
#include "Rect.h"
#include "Physics.h"
#include "PE_Vector.h"

namespace SFG
{
class Polygon;
class Line;

class Line
{
public:
	PE::Vector2df point;
	PE::Vector2df direction;
	
	///<summary>
	///Returns the components of the function y=mx+c
	///</summary>
	void createAnalyticFunction(mpf_class* m, mpf_class* c) const
	{
		assert(m != nullptr);
		assert(c != nullptr);
		//y = mx - m*x0 + y0
		*m = (direction.y / direction.x);
		*c = point.y - (*m) * point.x;
	}
	
	bool intersects(const Line& other, double* fac_this = nullptr, double* fac_other = nullptr) const
	{
		//Calculation:
		//1. Make analytic unlimited line
		//2. Solve equation
		//3. Find distance to the point
		
		//Step 1
		mpf_class m1, c1, m2, c2;
		this->createAnalyticFunction(&m1, &c1);
		other.createAnalyticFunction(&m2, &c2);
		
		//Step 2
		mpf_class x, y;
		mpf_class tmp = m1-m2;
		//m1 * x + c1 = m2 * x + c2
		//(m1-m2) * x = c2 - c1
		
		if(tmp == 0.f)
		{
			//Parallel lines. I'm lazy and I'm gonna say:
			//If the lines are the same, I don't care
			return false;
		}
		
		x = (c2 - c1) / (tmp);
		
		y = m1*x + c1;
		
		//Step 3
		PE::Vector2df point({x,y});
		
		PE::Vector2df diff1 = point - this->point;
		PE::Vector2df diff2 = point - other.point;
		auto factor1 = diff1.getFactor(this->direction);
		
		auto factor2 = diff1.getFactor(other.direction);
		
		if(fac_this != nullptr) *fac_this = factor1.get_d();
		if(fac_other != nullptr) *fac_other = factor2.get_d();
		
		if(factor1 < 0. || factor1 > 1.)
			return false;
		
		if(factor2 < 0. || factor2 > 1.)
			return false;
		
		return true;
	}
	
private:
};

///<summary>
///Represents a polygon utility
///</summary>
class Polygon
{
public:
    Polygon();
	
	Polygon(const std::vector<PE::Vector2df>& data)
	{
		std::copy(data.begin(), data.end(), m_points.begin());
	}
	
    ~Polygon();
	
	///<summary>
	///Finds out whether this polygon contains a specific point
	///</summary>
	bool contains(const PE::Vector2df& point) const;

    ///<summary>
    ///Gets the distance between 'this' and the argument point.
    ///This is done by getting the two closest points, creating a line between those two and
    ///taking the Minimum from the distances from Point1, Point2 and the line to the point.
    ///</summary>
    PE::Vector2df getVectorToPoint(const PE::Vector2df& point);
	
	///<summary>
	///Rotates and translates a Polygon
	///</summary>
	///<param name="rot">The rotation in degrees (counterclockwise)</param>
	///<param name="trans">The vector to shift the polygon with</param>
	SFG::Pointer<Polygon> transrotatedPolygon(double rot, const PE::Vector2df& trans) const;

	
	///<summary>
	///Draws polygon. Using this function 
	///</summary>
	void draw(sf::RenderTarget& target, const sf::Color& color, float linewidth = 1.f)
	{
		sf::RectangleShape shape;
		shape.setFillColor(color);
		shape.setOutlineColor(color);
		auto firstpoint = *m_points.begin();
		PE::Vector2df p1 = *m_points.begin();
		PE::Vector2df p2;
		
		//This loop connects all points
		for(auto it = m_points.begin(); it != m_points.end(); ++it)
		{
			++it;
			if(it == m_points.end())
				p2 = firstpoint;
			else
				p2 = *it;
			float length = PE::Vector2df(p2-p1).absLength().get_d();
			shape.setSize(sf::Vector2f(length, linewidth));
			shape.setOrigin(0.f, linewidth / 2.f);
			shape.setPosition(p1.x.get_d(), p1.y.get_d());
			//#TODO: Check if correct. I assume SFML uses clockwise rotation, but math uses counterclockwise
			shape.setRotation(-sin(mpf_class(p2.y-p1.y).get_d()));
			
			//Draw the shape now
			target.draw(shape);
			p1 = p2;
		}
	}
	
	


private:
    std::list<PE::Vector2df> m_points;	//The Points
};

};
