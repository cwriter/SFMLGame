#pragma once
#include "stdafx.h"
#include "SSG_SolarSystem.h"
#include "SSG_Ship.h"

///<summary>
///Class for a galaxy
///</summary>
class SSG_Galaxy : public PE::PhysicObject
{
public:
	SSG_Galaxy();
	virtual ~SSG_Galaxy();
	
	inline void addObjectToGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr);
	
	inline void removeObjectFromGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr);
	
	inline void addSolarSystem(SFG::Pointer<SSG_SolarSystem> slrsys);
	
	inline void addShipToGalaxy(const SFG::Pointer<SSG_Ship>& ptr);
	
	virtual double x() const override 
	{
	      return m_x / m_totalmass.getScalar();
	}
	virtual double y() const override
	{
	      return m_y / m_totalmass.getScalar();	  
	}

private:
	PE::PhysicsEngine m_physicsEngine;
	PE::Mass m_totalmass;
	
	double m_x;
	double m_y;
	//The Solar Systems in the Galaxy
	std::map<SSG_SolarSystem*, SFG::Pointer<SSG_SolarSystem>> m_systems;
	//The ships that aren't inside a Solar system, but still inside a galaxy
	std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

