#include "stdafx.h"
#include "SSG_PlanetSurface.h"

int SSG_PlanetSurface::load()
{
	//TODO: Add code for loading stuff
		
		
	surf.setPrimitiveType(sf::LineStrip);
	//TESTING: We'll just add random vertices for now.
		
	std::default_random_engine gen;
	std::uniform_int_distribution<int> rnd(0,255);
	std::uniform_real_distribution<float> rndf(-1000.f, 1000.f);
	rnd(gen);
	const size_t def_count = 10000;
	const size_t defining_steps = 25;
	const size_t vert_count = def_count * defining_steps;
		
	float currdiff = rndf(gen);
	float nextdiff = rndf(gen);
	float grad = 0.f;
		
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
			
			grad = float(nextdiff - currdiff);
		}
		else
		{
			//Smooth vertices out 
			
			float fac = 0.0f;
			//fac = sin((PI/2.0) / float(defining_steps) * float(i % defining_steps));
			fac = (1.0 + sin(-(PI/2.0) + PI / float(defining_steps) * float(i % defining_steps))) / 2.0;
			
			double tmp = grad * fac;
			
			surf.append(sf::Vertex(sf::Vector2f(
					cos(ang)*(m_radius+currdiff + tmp),
					sin(ang)*(m_radius+currdiff + tmp)), 
				sf::Color::White));
		}
	}
	//!TESTING
	
	return 0;
}