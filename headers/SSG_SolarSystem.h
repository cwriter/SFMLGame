#pragma once
#include "stdafx.h"
#include "SSG_Planet.h"
#include "SSG_Ship.h"
#include "XML.h"

///<summary>
///Abstracts a solar system containing a random amount of suns, planets, asteroids
///and ships.
///</summary>
class SSG_SolarSystem : public PE::PhysicObject
{
public:
    SSG_SolarSystem();
    virtual ~SSG_SolarSystem();

    int load(const XMLReader& reader);


    int update(float dt)
    {
        this->m_physicsEngine.applyMutualForces();
        this->finishPhysicsCycle(dt);
        for(auto p : m_planets)
        {
            p.first->update(dt);
        }
        return 0;
    }

    void draw(sf::RenderTarget* t)
    {
        for(auto p : m_planets)
        {
            p.first->draw(*t);
        }
    }
    
    SFG::Pointer<SSG_Planet> find(const sf::String& identifier) const
	{
		SFG::Pointer<SSG_Planet> ptr(nullptr);
		for(auto p : m_planets)
		{
			if(p.first->getName() == identifier)
			{
				ptr.reset(p.second);
				return ptr;
			}
		}
		
		return ptr;
	}

    inline void addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr);

    inline void removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr);

    inline void addPlanetToSystem(SFG::Pointer<SSG_Planet>& ptr);

    inline void addShipToSystem(SFG::Pointer<SSG_Ship>& ptr)
    {
        this->m_ships[ptr.getElement()] = ptr;
        this->m_physicsEngine.addObject(ptr);
    }

    ///<summary>
    ///Gets the center of all masses combined in the system
    ///</summary>
    SFG::Vector2f getBalancePoint();

    double x() const override {
        return m_x / getMass().getScalar(); //Return the actual position
    }

    double y() const override {
        return m_y / getMass().getScalar(); //Return the actual position
    }
private:
    //The balanced center (as specific weight, units m*kg)
    double m_x;
    double m_y;

    //PE::Mass m_totalmass;

    PE::PhysicsEngine m_physicsEngine;	//The local physics engine
    std::map<SSG_Planet*, SFG::Pointer<SSG_Planet>> m_planets;
    std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;

};

