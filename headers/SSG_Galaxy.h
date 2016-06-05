#pragma once
#include "stdafx.h"
#include "SSG_SolarSystem.h"
#include "SSG_Ship.h"
#include "XML.h"

///<summary>
///Class for a galaxy
///</summary>
class SSG_Galaxy : public PE::PhysicObject
{
public:
    SSG_Galaxy();
    virtual ~SSG_Galaxy();

    int load(const XMLReader& reader);
	
	int update(float dt)
	{
		this->m_physicsEngine.applyMutualForces();
		this->finishPhysicsCycle(dt);
		for(auto ss : m_systems)
		{
			ss.first->update(dt);
		}
		return 0;
	}
	
	void draw(sf::RenderTarget* t)
	{
			for(auto ss : m_systems)
			{
				ss.first->draw(t);
			}
	}
	
	SFG::Pointer<SSG_Planet> find(const sf::String& identifier) const
	{
		SFG::Pointer<SSG_Planet> ptr;
		
		for(auto ss : m_systems)
		{
			ptr.reset(ss.first->find(identifier));
			if(ptr.isValid())
				return ptr;
		}
		
		return ptr;
	}

    inline void addObjectToGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr);

    inline void removeObjectFromGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr);

    inline void addSolarSystem(SFG::Pointer<SSG_SolarSystem> slrsys);

    inline void addShipToGalaxy(const SFG::Pointer<SSG_Ship>& ptr);

    virtual double x() const override
    {
        return m_x / getMass().getScalar();
    }
    virtual double y() const override
    {
        return m_y / getMass().getScalar();
    }

private:
    PE::PhysicsEngine m_physicsEngine;
    //PE::Mass m_totalmass;

    sf::String m_galaxy_name;

    double m_x;
    double m_y;
    //The Solar Systems in the Galaxy
    std::map<SSG_SolarSystem*, SFG::Pointer<SSG_SolarSystem>> m_systems;
    //The ships that aren't inside a Solar system, but still inside a galaxy
    std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

