#pragma once
#include "stdafx.h"
#include "SSG_Galaxy.h"
#include "SSG_Ship.h"

///<summary>
///Class that contains all galaxies
///</summary>
class SSG_Universe : public PE::PhysicObject
{
public:
    SSG_Universe();
    virtual ~SSG_Universe();


    int load(const XMLReader& reader);


	
	int update(float dt)
	{
		this->m_physics_engine.applyMutualForces();
 		this->finishPhysicsCycle(dt);
		for(auto g : m_galaxies)
		{
			g.first->update(dt);
		}
		
	}

	void draw(sf::RenderTarget* t)
	{
		for(auto g : m_galaxies) 
		{
			g.first->draw(t);
		}
	}
	
	
	SFG::Pointer<SSG_Planet> findPlanet(const sf::String& identifier) const
	{
		SFG::Pointer<SSG_Planet> ptr;
		for(auto g : m_galaxies)
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

    ///<summary>
    ///Hands a Galaxy to the physics engine of this universe
    ///</summary>
    ///<param name="ptr">
    ///The pointer to the ship that should be added.
    ///</param>
    void addGalaxy(SFG::Pointer<SSG_Galaxy>& ptr);

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
	
    //The physics engine used
    PE::PhysicsEngine m_physics_engine;

    //PE::Mass m_totalmass;

    double m_x;
    double m_y;

    //Map that contains galaxies
    std::map<SSG_Galaxy*, SFG::Pointer<SSG_Galaxy>> m_galaxies;

    //Map that contains ships
    std::map<SSG_Ship*, SFG::Pointer<SSG_Ship>> m_ships;
};

