#pragma once
#include "stdafx.h"
#include "SSG_Galaxy.h"
#include "SSG_Ship.h"

///<summary>
///Class that contains all galaxies
///</summary>
class SSG_Universe : public SSG_CelestialObjectContainer<SSG_Galaxy>
{
public:
    SSG_Universe();
    virtual ~SSG_Universe();


    int load(const XMLReader& reader);
	
	SFG::Pointer<SSG_Planet> findPlanet(const sf::String& identifier) const
	{
		SFG::Pointer<SSG_Planet> ptr;
		for(auto g : m_CelestialObjects)
		{
			ptr.reset(g.first->find(identifier));
			if(ptr.isValid())
				return ptr;
		}
		return ptr;
	}
	
    ///<summary>
    ///Hands a ship to the physics engine of this universe
    ///</summary>
    ///<param name="ptr">
    ///The pointer to the ship that should be added.
    ///</param>
    void addShip(SFG::Pointer<SSG_Ship>& ptr);


    double x() const override
    {
        return m_x / getMass().getScalar();
    }

    double y() const override
    {
        return m_y / getMass().getScalar();
    }



    int object_message_handler_ships(int msg, const SFG::Pointer<MsgPackage>& pkg);

    int object_message_handler_galaxies(int msg, const SFG::Pointer<MsgPackage>& pkg);

private:
	
	sf::String m_name;


    //Map that contains ships
    std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

