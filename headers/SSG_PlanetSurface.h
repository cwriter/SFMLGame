#pragma once
#include "stdafx.h"
#include "Object.h"
#include "PE_Vector.h"
#include <random>

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
		surf.setPrimitiveType(sf::Lines);
	}
	
	SSG_PlanetSurface(double radius)
	{
		setRadius(radius);
		m_main_position.x = PI / 2.f;
		m_main_position.y = radius;
	}
	
	int load()
	{
		//TODO: Add code for loading stuff
		
		
		surf.setPrimitiveType(sf::LineStrip);
		//TESTING: We'll just add random vertices for now.
		
		std::default_random_engine gen;
		std::uniform_int_distribution<int> rnd(0,255);
		std::uniform_real_distribution<float> rndf(-1000.f, 1000.f);
		rnd(gen);
		const size_t vert_count = 100000;
		const size_t defining_steps = 10;
		
		float currdiff = rndf(gen);
		float nextdiff = rndf(gen);
		
		for(size_t i = 0; i < vert_count; i++)
		{
			float ang = 2.0*PI*double(i)/double(vert_count);
			if(i % defining_steps == 0)
			{
				currdiff = nextdiff;
				nextdiff = rndf(gen);
				surf.append(sf::Vertex(sf::Vector2f(
						cos(ang)*(m_radius+currdiff),
						sin(ang)*(m_radius+currdiff)), 
					sf::Color::Red));
			}
			else
			{
				//Smooth vertices out 
				double delta = nextdiff - currdiff;
				double tmp = delta * double((i % defining_steps)) / double(defining_steps); 
				
				surf.append(sf::Vertex(sf::Vector2f(
						cos(ang)*(m_radius+currdiff + tmp),
						sin(ang)*(m_radius+currdiff + tmp)), 
					sf::Color::White));
			}
		}
		//!TESTING
		
		return 0;
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
		sf::VertexArray va(sf::Triangles);
		float dist = 100.f;
		double phi = m_main_position.x.get_d();
		double r = m_main_position.y.get_d();
		va.append(sf::Vertex(sf::Vector2f(
			cos(phi)*r - dist,
			sin(phi)*r - dist)
		, sf::Color::Red));
		
		va.append(sf::Vertex(sf::Vector2f(
			cos(phi)*r,
			sin(phi)*r + dist)
		, sf::Color::Blue));
		
		va.append(sf::Vertex(sf::Vector2f(
			cos(phi)*r + dist,
			sin(phi)*r - dist)
		, sf::Color::Yellow));
		
		t.draw(va);
		
		t.draw(surf);
		
	}
	
	int update(float dt)
	{
		return 0;
	}
	
private:
	PE::Vector2df m_main_position;
	sf::VertexArray surf;
	double m_radius;
	
};