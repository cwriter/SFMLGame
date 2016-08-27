#include "stdafx.h"
#include "SSG_PlanetSurface.h"

SSG_Biome_Hills::SSG_Biome_Hills(float radius)
{
	m_radius = radius;
	
	temp_min = 253.15f;
	temp_max = 313.15f;
		
	height_min = -500.f;
	height_max = 500.f;
		
	height_diff_min = 0.f;
	height_diff_max = 0.5f;
	
	width_min = 10000.f;
	width_max = 500000.f;
	
	base_clr = sf::Color::Green;
}

SFG::Pointer< SSG_Biome > SSG_Biome_Hills::next() const
{
	SFG::Pointer<SSG_Biome> ptr;
	std::default_random_engine gen;
	std::uniform_real_distribution<float> rndf(0.0f, 1.0f);
	
	float f = rndf(gen);
	
	if(f < 0.4f)
	{
		//Mountains
		ptr.reset(new SSG_Biome_Mountains(m_radius));
	}
	else if(f < 0.7f)
	{
		//Sea
		ptr.reset(new SSG_Biome_Sea(m_radius));
	}
	else if(f < 1.f)
	{
		//Desert
		ptr.reset(new SSG_Biome_Desert(m_radius));
	}
	
	return ptr;
}

SSG_Biome_Desert::SSG_Biome_Desert(float radius)
{
	temp_min = 230.f;
	temp_max = 503.15f;
	
	height_min = -500.f;
	height_max = 500.f;
	
	height_diff_min = 0.0f;
	height_diff_max = 0.1f;
	
	width_min = 100000.f;
	width_max = 1000000.f;
	
	base_clr = sf::Color::Yellow;
	
	m_radius = radius;
}

SFG::Pointer< SSG_Biome > SSG_Biome_Desert::next() const
{
	SFG::Pointer<SSG_Biome> ptr;
	std::default_random_engine gen;
	std::uniform_real_distribution<float> rndf(0.0f, 1.0f);
		
	float f = rndf(gen);
		
	if(f < 0.2f)
	{
		//Sea
		ptr.reset(new SSG_Biome_Sea(m_radius));
	}
	else if(f < 0.8f)
	{
		//Hills
		ptr.reset(new SSG_Biome_Hills(m_radius));
	}
	else if(f < 1.f)
	{
		//Desert
		ptr.reset(new SSG_Biome_Desert(m_radius));
	}
	
	return ptr;
}

SSG_Biome_Mountains::SSG_Biome_Mountains(float radius)
{
	temp_min = 213.15f;
	temp_max = 293.15f;
	
	height_min = 1500.f;
	height_max = 15000.f;
	
	height_diff_min = 0.0f;
	height_diff_max = 4.f;
	
	width_min = 100000;
	width_max = 1000000;
		
	base_clr = sf::Color(130, 130, 130);
	
	m_radius = radius;
}

SFG::Pointer< SSG_Biome > SSG_Biome_Mountains::next() const
{
	SFG::Pointer<SSG_Biome> ptr;
	std::default_random_engine gen;
	std::uniform_real_distribution<float> rndf(0.0f, 1.0f);
	
	float f = rndf(gen);
	
	if(f < 0.5f)
	{
		//Sea
		ptr.reset(new SSG_Biome_Sea(this->m_radius));
	}
	else if(f < 1.f)
	{
		//Mountains
		ptr.reset(new SSG_Biome_Mountains(this->m_radius));
	}
	
	return ptr;
}

SSG_Biome_Sea::SSG_Biome_Sea(float radius)
{
	m_radius = radius;
	
	temp_min = 277.15f;
	temp_max = 293.15f;
	
	height_min = -100000.f;
	height_max = 0.f;
	
	height_diff_min = 0.f;
	height_diff_max = 4.f;
	
	width_min = 100000;
	width_max = 10000000;
	
	base_clr = sf::Color::Blue;
}

SFG::Pointer< SSG_Biome > SSG_Biome_Sea::next() const
{
	SFG::Pointer<SSG_Biome> ptr;
	std::default_random_engine gen;
	std::uniform_real_distribution<float> rndf(0.0f, 1.0f);
		
	float f = rndf(gen);
	
	if(f < 0.6f)
	{
		//Hills
		ptr.reset(new SSG_Biome_Hills(this->m_radius));
	}
	else if (f < 0.8f)
	{
		//Desert
		ptr.reset(new SSG_Biome_Desert(this->m_radius));
	}
	else if(f < 1.f)
	{
		//Mountains
		ptr.reset(new SSG_Biome_Mountains(this->m_radius));
	}
	
	return ptr;
}

int SSG_PlanetSurface::load()
{
	surf.setPrimitiveType(sf::LineStrip);
	if(true){
	//TESTING
	SFG::Pointer<SSG_Biome_Hills> p(new SSG_Biome_Hills(m_radius));
	double endphi = 0.0;
	
	endphi = p->create(surf, 0., .25, 25, getRadius());
	
	auto ptr = p->next();
	
	while(endphi < 2.*PI)
	{
		auto n = ptr->next();
		//SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__, "Generation: At phi %f", endphi);
		endphi = n->create(surf, endphi, .25, 25., getRadius());
		ptr = n;
	}
	
	return 0;
	//!TESTING
	}
	//TODO: Add code for loading stuff
		
		
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

double SSG_Biome::create(sf::VertexArray& va, double startphi, double vcount_per_km, size_t vsmooth, double startheight) const
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
	
	double endphi = std::min(startphi + phidiff, 2.*PI);
	
	size_t vcount = size_t(vcount_per_km * float(width / 1000.f));
		
	float currdiff = 0.0;
	float grad = 0.f;
		
	for(size_t i = 0; i < vcount * vsmooth; i++)
	{
		float ang = startphi + phidiff*double(i)/double(vsmooth*vcount);
		if(i % vsmooth == 0)
		{
			/*if(radius + currdiff < height_min + m_radius) {
				radius = m_radius + height_min;
				std::cout << "Step " << i << ": Limiting height_min" << std::endl;
			}
			else if(radius + currdiff > height_max + m_radius) {
				radius = m_radius + height_max;
				std::cout << "Step " << i << ": Limiting height_max" << std::endl;
			}
			else*/
				radius += currdiff;
				
			currdiff = rndf(gen) * width / 1000. * vcount_per_km * ((rndi(gen) == -1) ? -1.f : 1.f);
				
			va.append(sf::Vertex(sf::Vector2f(
					cos(ang)*radius,
					sin(ang)*radius), 
				base_clr));
			
			grad = currdiff;
			//std::cout << "Grad is " << grad << std::endl;
			//std::this_thread::yield();
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
	
	//SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__, "Section of planet generated, at phi %f\n", endphi);
	return endphi;
}
