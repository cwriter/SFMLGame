#include "stdafx.h"
#include "SSG_Galaxy.h"



SSG_Galaxy::SSG_Galaxy()
{
    this->m_x = 0.f;
    this->m_y = 0.f;
}


SSG_Galaxy::~SSG_Galaxy()
{
}

int SSG_Galaxy::load(const XMLReader& reader)
{
	this->m_galaxy_name = reader.getValue("name.");
    size_t count = reader.for_all("SolarSystem", [=](const XMLGroup* g) {
        SFG::Pointer<SSG_SolarSystem> ptr(new SSG_SolarSystem());

        int ret = ptr->load(XMLReader(*g));

        if(ret == 0)
            this->m_systems[ptr.getElement()] = ptr;
        else
            SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load solar systems.");

    });
    SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__, "Loaded %d solar systems in galaxy \"%s\"",
                        count, m_galaxy_name.toAnsiString().c_str());

    return 0;
}

void SSG_Galaxy::addObjectToGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr)
{
    m_totalmass += ptr->getMass();
    m_physicsEngine.addObject(ptr);
    //We now need to add the object's mass to totalmass and
    //add the specific weights to the balanced position
    m_x += ptr->getMass().getScalar() * ptr->x();
    m_y += ptr->getMass().getScalar() * ptr->y();
}

void SSG_Galaxy::addSolarSystem(SFG::Pointer<SSG_SolarSystem> slrsys)
{
    this->addObjectToGalaxy(slrsys);
    this->m_systems[slrsys.getElement()] = slrsys;
}

void SSG_Galaxy::removeObjectFromGalaxy(const SFG::Pointer<PE::PhysicObject>& ptr)
{
    m_physicsEngine.removeObject(ptr);
    //Remove from the mass center
    m_x -= ptr->getMass().getScalar() * ptr->x();
    m_y -= ptr->getMass().getScalar() * ptr->y();

    m_totalmass -= ptr->getMass();
}

void SSG_Galaxy::addShipToGalaxy(const SFG::Pointer<SSG_Ship>& ptr)
{
    this->m_ships[ptr.getElement()] = ptr;
    this->addObjectToGalaxy(ptr);
}
