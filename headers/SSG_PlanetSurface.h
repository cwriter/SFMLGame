#pragma once
#include "stdafx.h"
#include "Object.h"
#include "PE_Vector.h"
#include "Physics.h"
#include <random>


class SSG_Biome_Mountains;



class SSG_Biome
{
public:
	
	virtual ~SSG_Biome()
	{
	}
	
	virtual SFG::Pointer<SSG_Biome> next() const = 0;
	
	
	///<summary>
	///
	///</summary>
	///<param name="vcount">
	///Count per 1000m
	///</param>
	void create(sf::VertexArray& va, double startphi, size_t vcount_per_km, size_t vsmooth, double startheight) const;
	
	bool matchesTemp(float temp)
	{
		return (temp_min <= temp && temp <= temp_max);
	}
	
	bool matchesHeight(float height)
	{
		return (temp_min <= height && height <= temp_max);
	}
	
protected:
	
	enum humidity {
		arid,
		dry,
		wet
	};
	
	sf::Color base_clr;
	
	float temp_max;
	float temp_min;
	
	float height_max;
	float height_min;
	
	float height_diff_max;
	float height_diff_min;
	
	float width_min;
	float width_max;
	
	float m_radius;
	
};

class SSG_Biome_Mountains 
	: public SSG_Biome
{
public:
	SSG_Biome_Mountains(float radius)
	{
		temp_min = 213.15f;
		temp_max = 293.15f;
		
		height_min = 1500.f;
		height_max = 15000.f;
		
		height_diff_min = 0.5f;
		height_diff_max = 4.f;
		
		width_min = 10000;
		width_max = 100000;
		
		base_clr = sf::Color(130, 130, 130);
		
		m_radius = radius;
	}
	
	virtual SFG::Pointer<SSG_Biome> next() const override
	{
		SFG::Pointer<SSG_Biome> ptr;
		std::default_random_engine gen;
		std::uniform_real_distribution<float> rndf(0.0f, 1.0f);
		
		float f = rndf(gen);
		
		if(f < 0.2f)
		{
			//Sea
		}
		else if(f < 0.8f)
		{
			//Hills
		}
		else if(f < 1.f)
		{
			//Desert
		}
		
		return ptr;
	}
private:
};


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
	
	//Biome traits:
	//========= DESERT =======
	// - Simple, dry, flat
	// - Possible neighbors: Mountains 0.5, Sea 0.5
	// - Temperature: > 30°
	// - Height over sea: [-500;500]m
	// - Height differences between points [-0.1;+0.1]dy/dx
	//========= HILLS ========
	// - Rich, wet, hills
	// - Possible neighbors: Mountains 0.2, Sea 0.2, Hills 0.6
	// - Temperature: [-20;30]°
	// - Height over sea: [-500;500]m
	// - Height differences between points [-0.5;+0.5]dy/dx
	//========= MOUNTAINS ====
	// - Simple, dry, mountainous
	// - Possible neighbors: Sea 0.2, Hills 0.6, Desert 0.2
	// - Temperature: [-60;20]°
	// - Height over Sea; [1500;10000]m
	// - Height difference between points [0.5;4]dy/dx
	//========= SEA ==========
	// - Simple, wet, flat
	// - Possible neighbors: Hills 0.6, Desert 0.2, Mountains 0.2
	// - Temperature [-30;30]°
	// - Height over sea: ]-inf;0]m
	// - Height difference between points [0.001; 4]dy/dx
	
	
	
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
	
	int load();
	
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