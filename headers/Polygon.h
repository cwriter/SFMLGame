#pragma once
#include "stdafx.h"
#include "Rect.h"

namespace SFG
{


	///<summary>
	///Represents a polygon utility
	///</summary>
	class Polygon
	{
	public:
		Polygon();
		~Polygon();

		///<summary>
		///Gets the distance between 'this' and the argument point.
		///This is done by getting the two closest points, creating a line between those two and
		///taking the Minimum from the distances from Point1, Point2 and the line to the point.
		///</summary>
		SFG::Vector2f getVectorToPoint(const SFG::Vector2f& point);

	private:
		std::vector<SFG::Vector2f> m_points;	//The Points
	};

};