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
            this->addSpecificToSystem(ptr);
        else
            SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load solar systems.");

    });
    SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__, "Loaded %d solar systems in galaxy \"%s\"",
                        count, m_galaxy_name.toAnsiString().c_str());

    return 0;
}

void SSG_Galaxy::addShipToGalaxy(const SFG::Pointer<SSG_Ship>& ptr)
{
    this->m_ships[ptr.getElement()] = ptr;
    this->addObjectToSystem(ptr);
}
