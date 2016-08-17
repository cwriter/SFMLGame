#pragma once
#include "stdafx.h"
#include "Interactable.h"

///<summary>
///Class that contains elements
///</summary>
template <class T>
class SSG_CelestialObjectContainer
	: public PE::PhysicObject, public GObjectBase, public SFG::Interactable
{
    static_assert(std::is_base_of<PE::PhysicObject, T>::value, "T must derive from PE::PhysicObject");

public:
    SSG_CelestialObjectContainer();
    
    void draw(sf::RenderTarget& t)
	{
		for(size_t i = 0; i < m_CelestialObjects.size(); i++)
			m_CelestialObjects[i]->draw(t);
	}

    int update(float dt);
	
	sf::FloatRect getLogicBoundingRect() const override;

    void removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr);
	
	SFG::Pointer<T> findClosestTo(const PE::Vector2f& pos) const
	{
		SFG::Pointer<T> min;
		float dist(std::numeric_limits<float>::max());
		
		for(const auto& g : m_CelestialObjects)
		{
			float d = pos.distanceTo(PE::Vector2f({
				(float)g->x().get_d(),
				(float)g->y().get_d()}));
			
			dist = std::min(d, dist);
			if(d == dist)
				min.reset(g);
		}
		return min;
	}

    inline void addSpecificToSystem(const SFG::Pointer<T>& ptr)
    {
        //m_CelestialObjects[ptr.getElement()] = ptr;
		m_CelestialObjects.push_back(ptr);
        addObjectToSystem(ptr);
    }

    mpf_class x() const override {
        return m_x / getMass().getScalar(); //Return the actual position
    }

    mpf_class y() const override {
        return m_y / getMass().getScalar(); //Return the actual position
    }
    
	virtual void clearDelayedActionTasks() override {
		m_requests.clear();
		for(auto& o : m_CelestialObjects)
			o->clearDelayedActionTasks();
	}

protected:
    void addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr);
    
    PE::PhysicsEngine m_physicsEngine;
	std::vector<SFG::Pointer<T>> m_CelestialObjects;
    
	mpf_class m_x;
    mpf_class m_y;

};

#include "SSG_CelestialObjectContainer.cpp"