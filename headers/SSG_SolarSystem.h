#pragma once
#include "stdafx.h"
#include "SSG_Planet.h"
#include "SSG_Ship.h"
#include "XML.h"

class SSG_Planet;
///<summary>
///Abstracts a solar system containing a random amount of suns, planets, asteroids
///and ships.
///</summary>
class SSG_SolarSystem : 
	public SSG_CelestialObjectContainer<SSG_Planet>
{
public:
    SSG_SolarSystem();
    virtual ~SSG_SolarSystem();

    int load(const XMLReader& reader);


    int update(float dt)
    {
        this->m_physicsEngine.applyMutualForces();
        this->finishPhysicsCycle(dt);
        for(auto p : m_CelestialObjects)
        {
            p.first->update(dt);
        }
        return 0;
    }

    void draw(sf::RenderTarget* t)
    {
        for(auto p : m_CelestialObjects)
        {
            p.first->draw(*t);
        }
    }
    
    SFG::Pointer<SSG_Planet> find(const sf::String& identifier) const
	{
		SFG::Pointer<SSG_Planet> ptr(nullptr);
		for(auto p : m_CelestialObjects)
		{
			if(p.first->getName() == identifier)
			{
				ptr.reset(p.second);
				return ptr;
			}
		}
		
		return ptr;
	}


    inline void addShipToSystem(SFG::Pointer<SSG_Ship>& ptr)
    {
        this->m_ships[ptr.getElement()] = ptr;
        this->m_physicsEngine.addObject(ptr);
    }

    ///<summary>
    ///Gets the center of all masses combined in the system
    ///</summary>
    SFG::Vector2<mpf_class> getBalancePoint();

private:

    std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;

};

