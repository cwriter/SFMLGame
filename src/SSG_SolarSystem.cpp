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
	this->m_name = reader.getValue("name.");
    reader.for_all("Planet", [=](const XMLGroup* g) {
        SFG::Pointer<SSG_Planet> ptr(new SSG_Planet());
		ptr->m_parentSys = this;
		ptr->setGuiDesktop(this->desktop());
        int ret = ptr->load(XMLReader(*g));
        if(ret == 0)
            this->addSpecificToSystem(ptr);
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

SFG::Vector2<mpf_class> SSG_SolarSystem::getBalancePoint()
{
    SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "This function should not be called in Release Mode!");
    SFG::Vector2<mpf_class> center(0.f, 0.f);

    mpf_class mass_sum = this->getMass().getScalar();

    //Sum up all specific weights
    for(auto p : m_CelestialObjects)
    {
        //Add to sum up by weight
        center.x += p.first->x()*p.first->getMass().getScalar();
        center.y += p.first->y()*p.first->getMass().getScalar();
    }

    return center / mass_sum;
}
