#pragma once
#include "stdafx.h"
#include "Object.h"
#include "Physics.h"
#include "XML.h"
#include "Interactable.h"
#include <SFGUI/Label.hpp>
#include <SFGUI/Window.hpp>
#include "SSG_PlanetSurface.h"

#define SSG_PLANET_DEFAULT_RADIUS 1000.f
#define SSG_PLANET_DEFAULT_MASS 4.f/3.f*PI*10e10

class SSG_SolarSystem;



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
    
    SFG::Pointer<SSG_PlanetSurface> m_planet_surface;
	
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

