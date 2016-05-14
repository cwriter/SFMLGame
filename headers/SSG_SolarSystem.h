#pragma once
#include "stdafx.h"
#include "SSG_Planet.h"
#include "SSG_Ship.h"

///<summary>
///Abstracts a solar system containing a random amount of suns, planets, asteroids 
///and ships. 
///</summary>
class SSG_SolarSystem : public PE::PhysicObject
{
public:
	SSG_SolarSystem();
	virtual ~SSG_SolarSystem();
	
	int update()
	{

		return 0;
	}

	inline void addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
	{
		m_totalmass += ptr->getMass();
		m_physicsEngine.addObject(ptr);
	}
	
	inline void removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
	{
		m_physicsEngine;
	  
	}
	
	inline void addPlanetToSystem(SFG::Pointer<SSG_Planet>& ptr)
	{
		m_planets[ptr.getElement()] = ptr;
		addObjectToSystem(ptr.cast<PE::PhysicObject>());
	}

	///<summary>
	///Gets the center of all masses combined in the system
	///</summary>
	SFG::Vector2f getBalancePoint() 
	{
		SFG::Vector2f center(0.f, 0.f);
		
		double mass_sum = this->m_totalmass.getScalar();
		
		//Sum up all specific weights
		for(auto p : m_planets)
		{	
			//Add to sum up by weight
			center.x += p.first->x()*p.first->getMass().getScalar();
			center.y += p.first->y()*p.first->getMass().getScalar();
		}
		
		
		
		return center / mass_sum;
	}
	
	double x() const override {
		return m_x;
	}

	double y() const override {
		return m_y;
	}

private:
	double m_x;
	double m_y;
	
	PE::Mass m_totalmass;
	
	PE::PhysicsEngine m_physicsEngine;	//The local physiscs engine
	std::map<SSG_Planet*, SFG::Pointer<SSG_Planet>> m_planets;
	std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;

};

