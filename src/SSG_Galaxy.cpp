#include "stdafx.h"
#include "SSG_Galaxy.h"



SSG_Galaxy::SSG_Galaxy()
{
	this->m_x = 0.f;
	this->m_y = 0.f;
}


SSG_Galaxy::~SSG_Galaxy()
{
}

void SSG_Galaxy::addObjectToGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr)
{
	m_totalmass += ptr->getMass();
	m_physicsEngine.addObject(ptr);
	//We now need to add the object's mass to totalmass and
	//add the specific weights to the balanced position
	m_x += ptr->getMass().getScalar() * ptr->x();
	m_y += ptr->getMass().getScalar() * ptr->y();
}

void SSG_Galaxy::addSolarSystem(SFG::Pointer<SSG_SolarSystem> slrsys)
{
      this->addObjectToGalaxy(slrsys);
      this->m_systems[slrsys.getElement()] = slrsys;
}

void SSG_Galaxy::removeObjectFromGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr)
{
	m_physicsEngine.removeObject(ptr);
	//Remove from the mass center
	m_x -= ptr->getMass().getScalar() * ptr->x();
	m_y -= ptr->getMass().getScalar() * ptr->y();

	m_totalmass -= ptr->getMass();
}

void SSG_Galaxy::addShipToGalaxy(const SFG::Pointer<SSG_Ship>& ptr)
{
	this->m_ships[ptr.getElement()] = ptr;
	this->addObjectToGalaxy(ptr);
}