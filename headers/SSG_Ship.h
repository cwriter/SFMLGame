#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"
#include "SSG_Part.h"

///<summary>
///Provides a abstraction class for ships, bases and vehicles.
///</summary>
class SSG_Ship :
    public virtual GObjectBase, public virtual PE::PhysicObject
{
public:

    SSG_Ship();
    virtual ~SSG_Ship();

    int notify(int msg) override {
        return 0;
    }

    ///<summary>
    ///Loads a ship
    ///</summary>
    ///<param name="reader">
    ///The reader to load the data from
    ///</param>
    int load(const XMLReader& reader) override;
	
    virtual int update(float dt) override
    {
	
		return 0;
	}
	
    virtual mpf_class x() const override
    {
		return m_x;
	}
	
    virtual mpf_class y() const override
    {
		return m_y;
	}

private:
	
	mpf_class m_x;
	mpf_class m_y;
	
	
    std::vector<SFG::Pointer<SSG_Part>> m_parts;	//All parts associated with the ship
    PE::PhysicsEngine m_local_physics;				//All objects that enter this ship shall enter a local system
};

