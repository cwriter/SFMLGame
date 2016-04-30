#pragma once
#include "stdafx.h"
#include "SSG_Galaxy.h"
#include "SSG_Ship.h"

///<summary>
///Class that contains all galaxies
///</summary>
class SSG_Universe : public MsgObject
{
public:
	SSG_Universe();
	virtual ~SSG_Universe();

	void addShip(SFG::Pointer<SSG_Ship>& ptr);

	void addGalaxy(SFG::Pointer<SSG_Galaxy>& ptr); 

	int object_message_handler_ships(int msg, const SFG::Pointer<MsgPackage>& pkg);

	int object_message_handler_galaxies(int msg, const SFG::Pointer<MsgPackage>& pkg);

private:
	PE::PhysicsEngine m_physics_engine;

	//Map that contains galaxies
	std::map<SSG_Galaxy*, SFG::Pointer<SSG_Galaxy>> m_galaxies;

	//Map that contains ships
	std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

