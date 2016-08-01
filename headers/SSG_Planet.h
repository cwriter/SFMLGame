#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"
#include "XML.h"

#define SSG_PLANET_DEFAULT_RADIUS 1000.f
#define SSG_PLANET_DEFAULT_MASS 4.f/3.f*PI*10e10

class SSG_SolarSystem;

class mouseRequest 
{
public:
	mouseRequest(){
		active = false;
	}
	bool active;
	sf::Vector2f pos;
	sf::Mouse::Button button;
};

class delayedActionTask
{
public:
	delayedActionTask(){
		active = true;
	}
	enum TaskType
	{
		mouseRequest = 0,
		takeoverRequest
		
	};
	
	bool isActive() const{
		return active;
	}
	
	void deactivate(){
		active = false;
	}
	
	TaskType m_task_type;
	
	void* m_request;
	
private:
	std::atomic<bool> active;
	
};

///<summary>
///Class that contains elements
///</summary>
template <class T>
class SSG_CelestialObjectContainer
	: public PE::PhysicObject, public GObjectBase
{
    static_assert(std::is_base_of<PE::PhysicObject, T>::value, "T must derive from PE::PhysicObject");

public:
    SSG_CelestialObjectContainer()
    {
        m_x = 0.;
        m_y = 0.;
    }

    int update(float dt)
	{
		this->m_physicsEngine.applyMutualForces();
 		this->finishPhysicsCycle(dt);
		for(auto g : m_CelestialObjects)
		{
			//Check for mouse actions
			for(auto& action : this->m_requests)
			{
				if(action->isActive())
				{
					if(action->m_task_type == delayedActionTask::TaskType::mouseRequest)
					{
						auto req = static_cast<mouseRequest*>(action->m_request);
						if(g.first->getBoundingRect().contains(req->pos))
						{
							
						}
				
					}
				}
			}
			g.first->update(dt);
		}
		return 0;
	}
	
	void draw(sf::RenderTarget* t)
	{
		for(auto g : m_CelestialObjects) 
		{
			g.first->draw(t);
		}
	}
	

    inline void removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
    {
        m_physicsEngine.removeObject(ptr);
        //Remove from the mass center
        m_x -= ptr->getMass().getScalar() * ptr->x();
        m_y -= ptr->getMass().getScalar() * ptr->y();

        this->setMass(getMass() - ptr->getMass());
    }

    inline void addSpecificToSystem(SFG::Pointer<T>& ptr)
    {
        m_CelestialObjects[ptr.getElement()] = ptr;
        addObjectToSystem(ptr);
    }

    mpf_class x() const override {
        return m_x / getMass().getScalar(); //Return the actual position
    }

    mpf_class y() const override {
        return m_y / getMass().getScalar(); //Return the actual position
    }

    void addDelayedActionTask(const SFG::Pointer<delayedActionTask>& task)
	{
		this->m_requests.push_back(task);
	}
    
protected:
    inline void addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
    {
        this->setMass(getMass() + ptr->getMass());
        m_physicsEngine.addObject(ptr);
        //We now need to add the object's mass to totalmass and
        //add the specific weights to the balanced position
        m_x += ptr->getMass().getScalar() * ptr->x();
        m_y += ptr->getMass().getScalar() * ptr->y();
    }

    std::vector<SFG::Pointer<delayedActionTask>> m_requests;
    
    PE::PhysicsEngine m_physicsEngine;
    std::map<T*, SFG::Pointer<T>> m_CelestialObjects;

    mpf_class m_x;
    mpf_class m_y;

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

    virtual mpf_class x() const override {
        //return double(SFG::FloatRect(m_circle.getGlobalBounds()).center().x);
		return m_x;
    }

    virtual mpf_class y() const override {
        //return double(SFG::FloatRect(m_circle.getGlobalBounds()).center().y);
		return m_y;
    }

    int notify(int msg) override
    {
        return 0;
    }


    sf::CircleShape& getShape() {
        return this->m_circle;
    }



    //===BELOW ARE SOME OVERRIDDEN FUNCTIONS TO MAKE THE SHAPE WORK THE SAME WAY AS THE SPRITE. DO NOT TOUCH!
    void setPosition(mpf_class x, mpf_class y) {
		m_x = x; 
		m_y = y;
        this->m_circle.setPosition(x.get_d(), y.get_d());
        GObjectBase::setPosition(x.get_d(), y.get_d());
    }
    
    void setPosition(float x, float y) override {
		SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__, 
				   "Function %s with low precision was called. Please call the function with\n"
				   "the higher precision instead", __FUNCTION__);
		std::string str;
		SFG::Util::getStackTrace(str);
		printf(str.c_str());
		m_x = x; 
		m_y = y;
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
    
    ///<summary>
	///Draws physics vectors of force and velocity to the specfied target, 
	///origining from the x() and y() positions
	///</summary>
	void drawPhysicsComponents(sf::RenderTarget* t, float vecscale) const;
    
    SSG_SolarSystem* m_parentSys;	//The parent system
	
	SFG::Pointer<SSG_Planet> m_parentPlanet;	//Only if available; The planet this moon is orbiting

private:
    sf::CircleShape m_circle;
	mpf_class m_x;
	mpf_class m_y;
	
};

