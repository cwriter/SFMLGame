#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"
#include "XML.h"
#include "Interactable.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Window.hpp>

#define SSG_PLANET_DEFAULT_RADIUS 1000.f
#define SSG_PLANET_DEFAULT_MASS 4.f/3.f*PI*10e10

class SSG_SolarSystem;

///<summary>
///Class that contains elements
///</summary>
template <class T>
class SSG_CelestialObjectContainer
	: public PE::PhysicObject, public GObjectBase, public SFG::Interactable
{
    static_assert(std::is_base_of<PE::PhysicObject, T>::value, "T must derive from PE::PhysicObject");

public:
    SSG_CelestialObjectContainer()
    {
        m_x = 0.;
        m_y = 0.;
		this->setClick([&](const sf::Vector2f& mpos, const sf::Mouse::Button& but){
			return -1;
		});
		this->setRightClick([&](const sf::Vector2f& mpos, const sf::Mouse::Button& but){
			return -1;
		});
		this->setHover([&](const sf::Vector2f& mpos){
			return -1;
		});
    }
    
    void draw(sf::RenderTarget& t)
	{
		for(size_t i = 0; i < m_CelestialObjects.size(); i++)
			m_CelestialObjects[i]->draw(t);
	}

    int update(float dt)
	{
		this->m_physicsEngine.applyMutualForces();
 		this->finishPhysicsCycle(dt);
		/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"Updating \"%s\" with %d objects", m_name.toAnsiString().c_str(),
							m_CelestialObjects.size()
		);*/
		#pragma omp parallel for
		for(size_t i = 0; i < m_CelestialObjects.size(); i++)
		{
			auto& g = m_CelestialObjects[i];
			//Check for mouse actions
			//for(auto& action : this->m_requests)
			for(size_t ii = 0; ii < m_requests.size(); ii++)
			{
				auto& action = m_requests[ii];
				if(action->isActive())
				{
					if(action->m_task_type == delayedActionTask::TaskType::mouseRequest)
					{
						auto req = static_cast<mouseRequest*>(action->m_request);
						//if(!this->getLogicBoundingRect().contains(req->pos)) continue;
						/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
							"%s: (%f|%f) %f:%f", g.first->m_name.toAnsiString().c_str(), g.first->getLogicBoundingRect().left, g.first->getLogicBoundingRect().top,
											g.first->getLogicBoundingRect().width, g.first->getLogicBoundingRect().height
						);*/
						if(g->getLogicBoundingRect().contains(req->pos))
						{
							if(g->onClick(req->pos, req->button) == 0)
							{
								//We're done
								/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
									"Event has been consumed"
								);*/
								#pragma omp critical
								{
									action->deactivate();
								}
							}
							else
							{
								//Handle more...
								/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
									"Relaying event to \"%s\"", g.first->m_name.toAnsiString().c_str()
								);*/
								//g.first->addDelayedActionTask(action);
							}
						}
						#pragma omp critical
						{
							g->addDelayedActionTask(action);
						}
					}
				}
			}
			g->update(dt);
		}
		return 0;
	}
	
	sf::FloatRect getLogicBoundingRect() const override
	{
		float l, r, t, b;
		l = std::numeric_limits<float>::max();
		r = std::numeric_limits<float>::min();
		t = std::numeric_limits<float>::max();
		b = std::numeric_limits<float>::min();
		
		//for(const auto& o : m_CelestialObjects)
		for(size_t i = 0; i < m_CelestialObjects.size(); i++)
		{
			auto& o = m_CelestialObjects[i];
			sf::FloatRect rect = o->getLogicBoundingRect();
			l = std::min(l, rect.left);
			r = std::max(r, rect.left + rect.width);
			t = std::min(t, rect.top);
			b = std::max(b, rect.top + rect.height);
		}
		
		return sf::FloatRect(l, r-l, t, b-t);
	}

    inline void removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
    {
        m_physicsEngine.removeObject(ptr);
        //Remove from the mass center
        m_x -= ptr->getMass().getScalar() * ptr->x();
        m_y -= ptr->getMass().getScalar() * ptr->y();

        this->setMass(getMass() - ptr->getMass());
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
    inline void addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
    {
        this->setMass(getMass() + ptr->getMass());
        m_physicsEngine.addObject(ptr);
        //We now need to add the object's mass to totalmass and
        //add the specific weights to the balanced position
        m_x += ptr->getMass().getScalar() * ptr->x();
        m_y += ptr->getMass().getScalar() * ptr->y();
    }

    
    PE::PhysicsEngine m_physicsEngine;
	std::vector<SFG::Pointer<T>> m_CelestialObjects;
    
	mpf_class m_x;
    mpf_class m_y;

};

///<summary>
///Represents a planet in a solar system (But suns, too. We aren't that strict)
///</summary>
class SSG_Planet :
    public GObjectBase, public PE::PhysicObject, public SFG::Interactable
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
    
    void setPosition(float x, float y) override;

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
    virtual SFG::FloatRect getBoundingRect() const override
    {
		return m_circle.getGlobalBounds();
	}
	
    sf::FloatRect getLogicBoundingRect() const override
    {
		return this->getBoundingRect();
	}
    
    int update_info(float dt);
	
	void draw_info(sf::RenderTarget& t);
    
    ///<summary>
	///Draws physics vectors of force and velocity to the specfied target, 
	///origining from the x() and y() positions
	///</summary>
	void drawPhysicsComponents(sf::RenderTarget* t, float vecscale) const;
    
    SSG_SolarSystem* m_parentSys;	//The parent system
    
    
	
	SFG::Pointer<SSG_Planet> m_parentPlanet;	//Only if available; The planet this moon is orbiting
protected:
	int internal_onclick(const sf::Vector2f& mpos, const sf::Mouse::Button& but);
private:
    sf::CircleShape m_circle;
	mpf_class m_x;
	mpf_class m_y;
	
	//================
	bool m_info_show;
	sfg::Window::Ptr m_info_window;
	sfg::Label::Ptr m_info_masstext;
	sfg::Label::Ptr m_info_postext;
	//================
	
};

