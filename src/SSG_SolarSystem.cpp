#include "stdafx.h"
#include "SSG_SolarSystem.h"



SSG_SolarSystem::SSG_SolarSystem()
{
    m_x = 0.;
    m_y = 0.;
}


SSG_SolarSystem::~SSG_SolarSystem()
{
}

int SSG_SolarSystem::load(const XMLReader& reader)
{

    reader.for_all("Planet", [=](const XMLGroup* g) {
        SFG::Pointer<SSG_Planet> ptr(new SSG_Planet());
        int ret = ptr->load(XMLReader(*g));
        if(ret == 0)
            this->addPlanetToSystem(ptr);
        else
        {
            SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load planet.");
        }
    });
	if(this->getMass().getScalar() == 0.0)
		SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__,
							"The system \"%s\" has 0 mass, which is a bug", "undefined");
    return 0;
}

void SSG_SolarSystem::addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
{
    this->setMass(getMass() + ptr->getMass());
    m_physicsEngine.addObject(ptr);
    //We now need to add the object's mass to totalmass and
    //add the specific weights to the balanced position
    m_x += ptr->getMass().getScalar() * ptr->x();
    m_y += ptr->getMass().getScalar() * ptr->y();
}

void SSG_SolarSystem::removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
{
    m_physicsEngine.removeObject(ptr);
    //Remove from the mass center
    m_x -= ptr->getMass().getScalar() * ptr->x();
    m_y -= ptr->getMass().getScalar() * ptr->y();

    this->setMass(getMass() - ptr->getMass());
}

void SSG_SolarSystem::addPlanetToSystem(SFG::Pointer<SSG_Planet>& ptr)
{
    m_planets[ptr.getElement()] = ptr;
    addObjectToSystem(ptr);
}

SFG::Vector2f SSG_SolarSystem::getBalancePoint()
{
    SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "This function should not be called in Release Mode!");
    SFG::Vector2f center(0.f, 0.f);

    double mass_sum = this->getMass().getScalar();

    //Sum up all specific weights
    for(auto p : m_planets)
    {
        //Add to sum up by weight
        center.x += p.first->x()*p.first->getMass().getScalar();
        center.y += p.first->y()*p.first->getMass().getScalar();
    }

    return center / mass_sum;
}
