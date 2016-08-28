#pragma once
#include "stdafx.h"
#include "Object.h"
#include "PE_Vector.h"
#include "Physics.h"
#include <random>
#include <chrono>

class SSG_Biome_Mountains;
class SSG_Biome_Desert;
class SSG_Biome_Hills;
class SSG_Biome_Sea;


class SSG_Biome
{
public:
	
	SSG_Biome()
	{
		gen.seed(std::chrono::system_clock::now().time_since_epoch().count());
	}
	
	virtual ~SSG_Biome()
	{
	}
	
	virtual SFG::Pointer<SSG_Biome> next() const = 0;
	
	
	///<summary>
	///Creates a biome
	///</summary>
	///<param name="vcount">
	///Count per 1000m
	///</param>
	double create(sf::VertexArray& va, double startphi, double vcount_per_km, size_t vsmooth, double startheight);
	
	bool matchesTemp(float temp)
	{
		return (temp_min <= temp && temp <= temp_max);
	}
	
	bool matchesHeight(float height)
	{
		return (temp_min <= height && height <= temp_max);
	}
	
//protected:
	
	enum humidity {
		arid,
		dry,
		wet
	};
	
	mutable std::default_random_engine gen;
	
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
	float m_planet_radius;
};

class SSG_Biome_Hills : public SSG_Biome
{
public:
	SSG_Biome_Hills(float radius);
	
	virtual SFG::Pointer<SSG_Biome> next() const override;
	
private:
	
};

class SSG_Biome_Desert 
	: public SSG_Biome
{
public:
	SSG_Biome_Desert(float radius);
	
	virtual SFG::Pointer<SSG_Biome> next() const override;
private:
};

class SSG_Biome_Mountains 
	: public SSG_Biome
{
public:
	SSG_Biome_Mountains(float radius);
	
	virtual SFG::Pointer<SSG_Biome> next() const override;
private:
};

class SSG_Biome_Sea : public SSG_Biome
{
public:
	SSG_Biome_Sea(float radius);
	
	virtual SFG::Pointer<SSG_Biome> next() const override;
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
	// - Possible neighbors: Mountains 0.4, Sea 0.3, Desert 0.3
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