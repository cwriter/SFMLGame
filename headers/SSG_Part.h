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

class CollisionDetail
{
public:
	CollisionDetail(bool collides, double eta,
					const PE::PhysicObject& obj1, const PE::PhysicObject& obj2,
					const PE::Velocity& newvel1, const PE::Velocity& newvel2)
		: m_collides(collides), m_eta(eta), m_o1(obj1), m_o2(obj2), m_nv1(newvel1), m_nv2(newvel2)
	{
			
	}
private:	
	bool m_collides;
	double m_eta; 	//Time until contact (in s)
	PE::PhysicObject m_o1;
	PE::PhysicObject m_o2;
	PE::Velocity m_nv1;
	PE::Velocity m_nv2;
};

///<summary>
///Represents a physically interactable, drawable part of a bigger object
///</summary>
class SSG_Part :
    public GObjectBase, public PE::PhysicObject
{
public:
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
    
    ///<summary>
    ///Gets a collision report from the provided parameters.
    ///</summary>
    ///<param name="inc">The incoming object</param>
    SFG::Pointer<CollisionDetail> collide(const PE::PhysicObject* inc) const
	{
		//#TODO: CONSIDER MOVING TO THE PHYSICOBJECT CLASS DIRECTLY
		
		//Safety first
		assert(inc != nullptr);
		
		//Get quick links
		const auto& vel = inc->getVelocity();
		const auto& mass = inc->getMass();
		const PE::Vector2df pos({inc->x(), inc->y()});
		
		//Get local quick links
		const auto& localvel = this->getVelocity();
		const auto& localmass = this->getMass();
		const PE::Vector2df localpos({this->x(), this->y()});
		
		//Now we have a nice little equation to solve:
		//localpos + localvel*t = pos + vel*t
		//-> localpos - pos = vel*t - localvel*t
		//-> (localpos - pos) / (vel - localvel) = t
		mpf_class tmp = (localpos.x - pos.x) / (vel.getVector().x- localvel.getVector().x);
		double eta = tmp.get_d();
		//Get a (really) rough estimate whether the objects will collide
		bool b_collides = (abs((localpos.y - pos.y) / (vel.getVector().y- localvel.getVector().y) - eta) < 5);
			
		
		
		//Also, get the (elastic) impulse:
		//v_1' = 2*(m_1*v_1 + m_2*v_2)/(m_1 + m_2) - v_1
		//(dito for v_2')
		PE::Vector2df reusable = 	(vel.getVector() * mass.getScalar() 
									+ localvel.getVector() * localmass.getScalar())
									/ (mass.getScalar() + localmass.getScalar());
		PE::Vector2df newvel = reusable*2.f - vel.getVector();
		PE::Vector2df newlocalvel = reusable*2.f - localvel.getVector();
		
		
		//Create the collision object
		SFG::Pointer<CollisionDetail> ptr(new CollisionDetail(b_collides, eta,
															  *this, *inc,
															  PE::Velocity(newlocalvel),  PE::Velocity(newvel)));
		
		
		return ptr;
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

