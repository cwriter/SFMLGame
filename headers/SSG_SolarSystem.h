#pragma once
#include "stdafx.h"
#include "SSG_Planet.h"

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

	int addObjectToSystem(SFG::Pointer<PE::PhysicObject>& ptr)
	{
		m_physicsEngine.addObject(ptr);
		return 0;
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
	PE::PhysicsEngine m_physicsEngine;	//The local physiscs engine
	std::map<SSG_Planet*, SFG::Pointer<SSG_Planet>> m_planets;

};

