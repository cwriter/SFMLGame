#include "stdafx.h"
#include "SOW_Car.h"
#include "Object.h"
#include "Pointer.h"

SOW_Car::SOW_Car()
{
	init_car();
}

void SOW_Car::init_car()
{
	this->m_gear_ratios.push_back(1.f);
	this->m_current_gear = 0;
	this->m_differential_ratio = 1.f;
	this->m_mtr_rpm = 0.f;
	
	this->m_wheel_front_left.setMass(250.f);
	this->m_wheel_front_left.setWheelRadius(0.25f);
	
	this->m_steering = 0.f;
	
	m_wheel_rear_left = m_wheel_rear_right = m_wheel_front_right = m_wheel_front_left;
	
	this->setMass(1000.f);
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
/*    //See comment below
    if (ModuleG2D::load(xmldata) != 0) {
        //Some error happened
        return -1;
    }*/
	//xmldata.getXMLGroupHandle("")->dump();
    //Load from xml
    xmldata.for_all("object", [=](const XMLGroup* g) {
		
		XMLReader r(*g);
		SFG::Pointer<SOW_Car> c(new SOW_Car(this));
		int loadableret = c.cast<Loadable>()->Loadable::load(r);
		
		int gobjret = c.cast<GObjectBase>()->GObjectBase::load(XMLReader(*r.getXMLGroupHandle("GObjectBase/")));
		
		int carret = c->load(XMLReader(*r.getXMLGroupHandle("Car")));
		
		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
							"Loading car module has return values %d - %d -%d", loadableret, gobjret, carret);
		
		this->addObject(c);
		
		
	});
    return 0;
}
