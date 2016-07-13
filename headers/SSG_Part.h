#pragma once
#include "stdafx.h"
#include "Object.h"
#include <list>
#include "Physics.h"


enum SSG_Material
{
	Steel,
	Plastic,
	Ceramic
	
};


///<summary>
///Represents a physically interactable, drawable part of a bigger object
///</summary>
class SSG_Part :
    public GObjectBase, public PE::PhysicObject
{
public:
	enum SSG_PartMessages
	{
		//Start at GObjectMessagesCount
		
	};
	
    SSG_Part();
    virtual ~SSG_Part();

    int notify(int msg) override
    {
        return 0;
    }

    virtual int load(XMLReader& data) override
    {
	
		return 0;
	}
    
    
    virtual mpf_class x() const override
    {
        return ((PE::PhysicObject*)this)->x();
    }

    virtual mpf_class y() const override
    {
        return ((PE::PhysicObject*)this)->y();
    }
    
    
private:

	int m_material;		//This holds the type of material the part consists of (if it is a simple type)
	
	std::list<SFG::Vector2f> m_points;	//The polygon points of the element (should not be many;
										//the count shall not exceed 100)
	
	

};

