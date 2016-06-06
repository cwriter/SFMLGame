#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"
#include "XML.h"

#define SSG_PLANET_DEFAULT_RADIUS 1000.f
#define SSG_PLANET_DEFAULT_MASS 4.f/3.f*PI*10e10

///<summary>
///Class that contains elements
///</summary>
template <class T>
class SSG_CelestialObjectContainer : public PE::PhysicObject
{
	static_assert(std::is_base_of<PE::PhysicObject, T>::value, "T must derive from PE::PhysicObject");

public:
	SSG_ObjectContainer<T>()
	{
		m_x = 0.;
		m_y = 0.;
	}
	
	inline void addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
	{
		this->setMass(getMass() + ptr->getMass());
		m_physicsEngine.addObject(ptr);
		//We now need to add the object's mass to totalmass and
		//add the specific weights to the balanced position
		m_x += ptr->getMass().getScalar() * ptr->x();
		m_y += ptr->getMass().getScalar() * ptr->y();
	}
	
	inline void removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
	{
		m_physicsEngine.removeObject(ptr);
		//Remove from the mass center
		m_x -= ptr->getMass().getScalar() * ptr->x();
		m_y -= ptr->getMass().getScalar() * ptr->y();

		this->setMass(getMass() - ptr->getMass());
	}

    inline void addPlanetToSystem(SFG::Pointer<SSG_Planet>& ptr)
	{
		m_CelestialObjects[ptr.getElement()] = ptr;
		addObjectToSystem(ptr);
	}
	
	double x() const override {
        return m_x / getMass().getScalar(); //Return the actual position
    }

    double y() const override {
        return m_y / getMass().getScalar(); //Return the actual position
    }
	
protected:
	PE::PhysicsEngine m_physicsEngine;
	std::map<T*, SFG::Pointer<T>> m_CelestialObjects;
	
	double m_x;
    double m_y;
	
};

///<summary>
///Represents a planet in a solar system (But suns, too. We aren't that strict)
///</summary>
class SSG_Planet :
    public GObjectBase, public PE::PhysicObject
{
public:
    SSG_Planet();
    virtual ~SSG_Planet();


    int load(const XMLReader& reader);
	
    void draw(sf::RenderTarget& t) override;

	inline const sf::String& getName() const
	{
		return this->m_name;
	}
	
    int update(float dt) override;

    double x() const override {
        return double(SFG::FloatRect(m_circle.getGlobalBounds()).center().x);
    }

    double y() const override {
        return double(SFG::FloatRect(m_circle.getGlobalBounds()).center().y);
    }

    int notify(int msg) override
    {
        return 0;
    }


    sf::CircleShape& getShape() {
        return this->m_circle;
    }



    //===BELOW ARE SOME OVERRIDDEN FUNCTIONS TO MAKE THE SHAPE WORK THE SAME WAY AS THE SPRITE. DO NOT TOUCH!
    void setPosition(float x, float y) override {
        this->m_circle.setPosition(x, y);
        GObjectBase::setPosition(x, y);
    }

    void setPosition(const sf::Vector2f& position) override {
        this->m_circle.setPosition(position);
        GObjectBase::setPosition(position);
    }

    void setScale(float factorX, float factorY) override {
        this->m_circle.setScale(factorX, factorY);
        GObjectBase::setScale(factorX, factorY);
    }

    void setScale(const sf::Vector2f& factors) override {
        this->m_circle.setScale(factors);
        GObjectBase::setScale(factors);
    }

    void setRotation(float angle) override {
        this->m_circle.setRotation(angle);
        GObjectBase::setRotation(angle);
    }

private:
    sf::CircleShape m_circle;
};

