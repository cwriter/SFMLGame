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

private:
	PE::PhysicsEngine m_physics_engine;

	//The Solar Systems in the Galaxy
	std::map<SSG_SolarSystem*, SFG::Pointer<SSG_SolarSystem>> m_systems;
	//The ships that aren't inside a Solar system, but still inside a galaxy
	std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

