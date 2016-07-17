#include "stdafx.h"
#include "SOW_Car.h"



SOW_Car::SOW_Car()
{
	this->m_gear_ratios.push_back(1.f);
	this->m_current_gear = 0;
	this->m_differential_ratio = 1.f;
	this->m_mtr_rpm = 0.f;
	
	this->m_wheel_front_left.setMass(250.f);
	this->m_wheel_front_left.setWheelRadius(0.25f);
	
	m_wheel_rear_left = m_wheel_rear_right = m_wheel_front_right = m_wheel_front_left;
}


SOW_Car::~SOW_Car()
{
}

int SOW_Car::load(const XMLReader& data)
{
	data.for_all("blinker", [=](const XMLGroup* g){
		XMLReader(*g).for_all("Light",[=](const XMLGroup* g2){
			//Add lights and stuff
			
			
		});
	});
	return 0;
}

int Module_SOW_Car::load(const XMLReader& xmldata)
{
    //See comment below
    if (ModuleG2D::load(xmldata) != 0) {
        //Some error happened
        return -1;
    }
    //Load from xml
    xmldata.for_all("object", [=](const XMLGroup* g) {
		
		XMLReader r(*g);
		SFG::Pointer<SOW_Car> c(new SOW_Car(this));
		c.cast<Loadable>()->Loadable::load(r);
		
		c.cast<GObjectBase>()->GObjectBase::load(XMLReader(*r.getXMLGroupHandle("GObjectBase")));
		
		c->load(XMLReader(*r.getXMLGroupHandle("Car")));
		
		this->addObject(c);
		
		
	});
    return 0;
}
