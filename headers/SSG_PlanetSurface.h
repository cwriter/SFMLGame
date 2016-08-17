#pragma once
#include "stdafx.h"
#include "Object.h"
#include "PE_Vector.h"


///<summary>
///Class that represents the surface. The coordinates shall be given in polar coordinates.
///For every object on (or below) the surface, the normalized radius vector of the 
///polar coordinates is the same as the normalized gravitational force vector.
///(0|0) is the center point of the planet.
///Because a planet would jitter quite a bit in the universe coordinate system, every
///planet has its own local coordinate system (origining at its center) and shall be
///drawn accordingly.
///</summary>
class SSG_PlanetSurface
{
public:
	
	SSG_PlanetSurface()
	{
		m_radius = 0;
	}
	
	SSG_PlanetSurface(double radius)
	{
		setRadius(radius);
		m_main_position.x = PI / 2.f;
		m_main_position.y = radius;
	}
	
	inline void setRadius(double radius)
	{
		m_radius = radius;
	}
	
	inline double getRadius() const
	{
		return m_radius;
	}
	
	inline const PE::Vector2df& getMainPosition() const
	{	
		return m_main_position;
	}
	
	void draw(sf::RenderTarget& t)
	{
		
	}
	
	int update(float dt)
	{
		return 0;
	}
	
private:
	PE::Vector2df m_main_position;
	double m_radius;
	
};