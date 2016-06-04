#include "stdafx.h"
#include "SSG_Planet.h"



SSG_Planet::SSG_Planet()
{
    //We are a planet, so we should probably be all round and stuff
    this->m_circle.setFillColor(sf::Color(128, 64, 0));
    this->m_circle.setPosition(0.f, 0.f);

    //Set the default radius
    this->m_circle.setRadius(SSG_PLANET_DEFAULT_RADIUS);
    this->setMass(PE::Mass(SSG_PLANET_DEFAULT_MASS));

}


SSG_Planet::~SSG_Planet()
{
}

int SSG_Planet::load(const XMLReader& reader)
{
	SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "Still to do");
	//Load values accordingly	
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
	this->setPosition(r.left, r.top);
	this->setVelocity(PE::Velocity(r.width, r.height));
	return 0;
}

void SSG_Planet::draw(sf::RenderTarget& t)
{
    //We have to override the default as we're not drawing a sprite, but a shape
    t.draw(this->m_circle);
}

int SSG_Planet::update(float dt)
{
    double time = dt / 1000.f;
    this->finishPhysicsCycle(dt);
    assert(!isnan(x()));
    assert(!isnan(y()));

    float test1 = float(this->x() + this->getVelocity().getVector().x * time);
    float test2 = float(this->y() + this->getVelocity().getVector().y * time);

    //We can lose precision, so if the precision is too small, just ignore everything
    if (!(isnan(test1) || std::isinf(test1)) && !(isnan(test2) || std::isinf(test2)))
    {
        this->setPosition(test1, test2);
        assert(!isnan(x()));
        assert(!isnan(y()));
    }
    //std::cout << "Vel (" << this->getVelocity().getVector().x << "|" << this->getVelocity().getVector().y << ")" << std::endl;
    //std::cout << "Loc (" << this->x() << "|" << this->y() << ")" << std::endl;

    return 0;
}
