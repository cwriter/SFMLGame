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

    SFG::Pointer<SSG_Planet> find(const sf::String& identifier) const
	{
		for(auto& p : m_CelestialObjects)
		{
			if(p->getName() == identifier)
			{
				return p;
			}
		}
		
		return SFG::Pointer<SSG_Planet>(nullptr);
	}
	
	///<summary>
	///Calculates (simple) lighting for all objects in this solar system.
	///</summary>
	int illuminate(int steps = 2);


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

