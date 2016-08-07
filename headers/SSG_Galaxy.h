#pragma once
#include "stdafx.h"
#include "SSG_SolarSystem.h"
#include "SSG_Ship.h"
#include "XML.h"

///<summary>
///Class for a galaxy
///</summary>
class SSG_Galaxy : 
	public SSG_CelestialObjectContainer<SSG_SolarSystem>
{
public:
    SSG_Galaxy();
    virtual ~SSG_Galaxy();

    int load(const XMLReader& reader);
	
	SFG::Pointer<SSG_Planet> find(const sf::String& identifier) const
	{
		SFG::Pointer<SSG_Planet> ptr;
		
		for(auto ss : m_CelestialObjects)
		{
			ptr.reset(ss->find(identifier));
			if(ptr.isValid())
				return ptr;
		}
		
		return ptr;
	}

    inline void addShipToGalaxy(const SFG::Pointer<SSG_Ship>& ptr);

    virtual mpf_class x() const override
    {
        return m_x / getMass().getScalar();
    }
    virtual mpf_class y() const override
    {
        return m_y / getMass().getScalar();
    }

private:
    sf::String m_galaxy_name;

    //The ships that aren't inside a Solar system, but still inside a galaxy
    std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

