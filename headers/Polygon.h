#pragma once
#include "stdafx.h"
#include "Rect.h"
#include "Physics.h"
#include "PE_Vector.h"

namespace SFG
{
class Polygon;

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
