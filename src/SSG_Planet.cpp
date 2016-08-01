#include "stdafx.h"
#include "SSG_Planet.h"
#include <SSG_SolarSystem.h>



SSG_Planet::SSG_Planet()
{
    //We are a planet, so we should probably be all round and stuff
    this->m_circle.setFillColor(sf::Color(128, 64, 0));
    this->m_circle.setPosition(0.f, 0.f);
	m_x = 0.;
	m_y = 0.;

    //Set the default radius
    this->m_circle.setRadius(SSG_PLANET_DEFAULT_RADIUS);
    this->setMass(PE::Mass(SSG_PLANET_DEFAULT_MASS));

}


SSG_Planet::~SSG_Planet()
{
}

int SSG_Planet::load(const XMLReader& reader)
{
	//SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "Still to do");
	//Load values accordingly	
	this->m_name = reader.getValue("name.");
	bool real = false;
	this->setMass(PE::Mass(reader.asDouble("mass/", real)));
	if(!real)
	{
		//Failed to load mass
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load Mass");
	}
	sf::FloatRect r = reader.asFloatRect("PosAndVel/", real);
	if(!real)
	{
		//Failed to load pos and Velocity
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load PosAndVel");
	}
	//this->setPosition(r.left, r.top);
	this->setPosition(mpf_class(r.left), mpf_class(r.top));
	this->setVelocity(PE::Velocity(r.width, r.height));
	
	this->getShape().setRadius(reader.asDouble("radius/", real));
	//#TODO: REMOVE TO HAVE CORRECT SIZES!!!! #important!
	//this->getShape().setRadius(2.5559e9 + reader.asDouble("radius/", real));
	if(!real)
	{
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
							"Failed to get radius for planet %s", this->m_name.toAnsiString().c_str());
	}
	
	SFG::FloatRect rect(this->getShape().getLocalBounds());
	this->getShape().setOrigin(rect.center());
	
	//Get Moons
	reader.for_all("Moon", [=](const XMLGroup* g){
		SFG::Pointer<SSG_Planet> ptr(new SSG_Planet());
		ptr->m_parentSys = this->m_parentSys;
		int ret = ptr->load(XMLReader(*g));
		if(ret != 0)
		{
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
								"Failed to load moon");
			return;
		}
		printf("Position: %f + %f | %f + %f\n", getShape().getPosition().x , ptr->getShape().getPosition().x,
			    getShape().getPosition().y, ptr->getShape().getPosition().y);
		//Correct relative values
		//ptr->setPosition(getShape().getPosition().x + ptr->getShape().getPosition().x, 
		//				 getShape().getPosition().y + ptr->getShape().getPosition().y);
		ptr->setPosition(x() + ptr->x(), 
						 y() + ptr->y());
		ptr->setVelocity(getVelocity() + ptr->getVelocity());
		
		
		//Add to system
		this->m_parentSys->addSpecificToSystem(ptr);
		
		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,	"Moon \"%s\" has been added", ptr->getName().toAnsiString().c_str());
		
	});
	
	//TESTING
	this->getShape().setFillColor(sf::Color(255, 255, 255));
	//!TESTING
	return 0;
}

void SSG_Planet::draw(sf::RenderTarget& t)
{
    //We have to override the default as we're not drawing a sprite, but a shape
    t.draw(this->m_circle);
	drawPhysicsComponents(&t, 1000.f);
}

void SSG_Planet::drawPhysicsComponents(sf::RenderTarget* t, float vecscale) const
{
	sf::RectangleShape line(sf::Vector2f(
		vecscale * getVelocity().abs(), vecscale * 5.f));
	line.setFillColor(sf::Color::White);
	line.setPosition(x().get_d(), y().get_d());

	line.setRotation(-180.f/PI*std::atan2(
		getVelocity().getVector().y.get_d(), getVelocity().getVector().x.get_d()));
		
	//printf("Velocity of %s is %f|%f\n",m_name.toAnsiString().c_str(), getVelocity().getVector().x.get_d(), getVelocity().getVector().y.get_d());
	t->draw(line);
	
	/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
		"Acceleration is %f", getAcceleration().getVector().absLength().get_d()
	);*/
		
	line.setSize(sf::Vector2f(
		vecscale * getAcceleration().getVector().absLength().get_d() * 100000.f, vecscale * 5.f));
	line.setRotation(-180.f/PI*std::atan2(
		getAcceleration().getVector().y.get_d(), getAcceleration().getVector().x.get_d()));
	
	line.setFillColor(sf::Color::Red);
	line.setOutlineColor(sf::Color::Red);
		
	t->draw(line);
}

int SSG_Planet::update(float dt)
{
    double time = dt / 1000.f;
    this->finishPhysicsCycle(dt);
    assert(!std::isnan(x().get_d()));
    assert(!std::isnan(y().get_d()));
	
	//printf("Velocity is %f|%f\n", this->getVelocity().getVector().x.get_d(), this->getVelocity().getVector().y.get_d());

	mpf_class ex1 = this->x().get_d() + this->getVelocity().getVector().x.get_d() * time;
	mpf_class ex2 = this->y().get_d() + this->getVelocity().getVector().y.get_d() * time;
	
    float test1 = float(ex1.get_d());
    float test2 = float(ex2.get_d());

    //We can lose precision, so if the precision is too small, just ignore everything
    if (!(std::isnan(test1) || std::isinf(test1)) && !(std::isnan(test2) || std::isinf(test2)))
    {
        this->setPosition(ex1, ex2);
        assert(!std::isnan(x().get_d()));
        assert(!std::isnan(y().get_d()));
    }
    //std::cout << "Vel (" << this->getVelocity().getVector().x << "|" << this->getVelocity().getVector().y << ")" << std::endl;
    //std::cout << "Loc (" << this->x() << "|" << this->y() << ")" << std::endl;

    return 0;
}
