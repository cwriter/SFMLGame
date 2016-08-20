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

void SSG_Biome::create(sf::VertexArray& va, double startphi, size_t vcount_per_km, size_t vsmooth, double startheight) const
{
	std::default_random_engine gen;
	std::uniform_real_distribution<float> rndf(height_diff_min, height_diff_max);
	std::uniform_int_distribution<int> rndi(-1, 0);
		
	
	double radius = startheight;
	
	std::uniform_real_distribution<float> rndfLength(width_min, width_max);
	//Phi*r = Circumference
	//Circumference / r = Phi
	
	float width = rndfLength(gen);
	
	float phidiff = width / m_radius;
	
	double endphi = startphi + phidiff;
	
	size_t vcount = vcount_per_km * float(width / 1000.f);
		
	float currdiff = 0.f;
	float nextdiff = rndf(gen) * rndi(gen);
	float grad = 0.f;
		
	for(size_t i = 0; i < vcount * vsmooth; i++)
	{
		float ang = startphi + phidiff*double(i)/double(vsmooth*vcount);
		if(i % vsmooth == 0)
		{
			radius += currdiff;
			if(radius + currdiff < height_min) {
				radius = height_min;
			}
			else if(radius + currdiff > height_max) {
				radius = height_max;
			}
			else
				radius += currdiff;
				
			//Get defining vecs
			currdiff = nextdiff;
			nextdiff = rndf(gen) * rndi(gen);
				
			va.append(sf::Vertex(sf::Vector2f(
					cos(ang)*radius,
					sin(ang)*radius), 
				base_clr));
				
			grad = nextdiff - currdiff;
		}
		else
		{
			//Smooth out
			float fac = (1.0 + sin(-(PI/2.0) + PI / float(vsmooth) * float(i % vsmooth))) / 2.0;
			double tmp = grad * fac;
			
			va.append(sf::Vertex(sf::Vector2f(
					cos(ang)*(radius + tmp),
					sin(ang)*(radius + tmp)), 
				base_clr));
		}
	}
	
	auto n = next();
	n->create(va, endphi, vcount_per_km, vsmooth, radius);
	
}