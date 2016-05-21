#include "stdafx.h"
#include "SSG_SolarSystem.h"



SSG_SolarSystem::SSG_SolarSystem()
{
	m_x = 0.;
	m_y = 0.;
}


SSG_SolarSystem::~SSG_SolarSystem()
{
}

void SSG_SolarSystem::addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
{
	m_totalmass += ptr->getMass();
	m_physicsEngine.addObject(ptr);
	//We now need to add the object's mass to totalmass and
	//add the specific weights to the balanced position
	m_x += ptr->getMass().getScalar() * ptr->x();
	m_y += ptr->getMass().getScalar() * ptr->y();
}

void SSG_SolarSystem::removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
{
	m_physicsEngine.removeObject(ptr);
	//Remove from the mass center
	m_x -= ptr->getMass().getScalar() * ptr->x();
	m_y -= ptr->getMass().getScalar() * ptr->y();
	
	m_totalmass -= ptr->getMass();
}

void SSG_SolarSystem::addPlanetToSystem(SFG::Pointer<SSG_Planet>& ptr)
{
	m_planets[ptr.getElement()] = ptr;
	addObjectToSystem(ptr.cast<PE::PhysicObject>());
}

SFG::Vector2f SSG_SolarSystem::getBalancePoint() 
{
	SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "This function should not be called in Release Mode!");
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