#include "stdafx.h"
#include "SSG_Universe.h"



SSG_Universe::SSG_Universe()
{
}


SSG_Universe::~SSG_Universe()
{
}

int SSG_Universe::load(const XMLReader& reader)
{
    //Dump first time
    size_t count = reader.for_all("Galaxy", [=](const XMLGroup* g) {
        SFG::Pointer<SSG_Galaxy> ptr(new SSG_Galaxy());
		ptr->setGuiDesktop(this->desktop());
        int ret = ptr->load(XMLReader(*g));
        if(ret == 0)
            this->addSpecificToSystem(ptr);
        else
            SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load galaxy. Code %d", ret);
    });

    SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__, "Loaded %d galaxies", count);

    return 0;
}

void SSG_Universe::addShip(SFG::Pointer<SSG_Ship>& ptr)
{
    assert(ptr.isValid());
    m_ships[ptr.getElement()] = ptr;
    m_physicsEngine.addObject(ptr);


    m_x += ptr->getMass().getScalar() * ptr->x();
    m_y += ptr->getMass().getScalar() * ptr->y();

    setMass(getMass() + ptr->getMass());

    //ptr.cast<GObjectBase>()->addListener(this,
    ptr->addListener(this,
                     MsgObjectAction(std::bind(&SSG_Universe::object_message_handler_ships,
                                     this, std::placeholders::_1, std::placeholders::_2)));
}


int SSG_Universe::object_message_handler_ships(int msg, const SFG::Pointer<MsgPackage>& pkg)
{
    if (msg == PE::PhysicObject::PhysicObjectMessages::Destroyed)
    {
        //First argument is the sender
        SSG_Ship* sender = (SSG_Ship*)pkg->getValue(0);
        //Remove this guy from the object list
        this->m_ships.erase(sender); //#TODO: Check if this actually calls the destructor

        this->m_x -= sender->getMass().getScalar() * sender->x();
        this->m_y -= sender->getMass().getScalar() * sender->y();

        setMass(getMass() - sender->getMass());
    }
    else
    {
        return -1;
    }
    return 0;
}

int SSG_Universe::object_message_handler_galaxies(int msg, const SFG::Pointer<MsgPackage>& pkg)
{
    if (msg == PE::PhysicObject::PhysicObjectMessages::Destroyed)
    {
        //First argument is the sender
        SSG_Galaxy* sender = (SSG_Galaxy*)pkg->getValue(0);
        //Remove this guy from the object list
        //this->m_CelestialObjects.erase(sender); //#TODO: Check if this actually calls the destructor
		//Find the object
		auto it = std::find_if(m_CelestialObjects.begin(), m_CelestialObjects.end(), [sender](const SFG::Pointer<SSG_Galaxy>& ptr){
			return (sender == ptr.getElement());
		});
		if(it == m_CelestialObjects.end()) return -2;

        this->m_x -= sender->getMass().getScalar() * sender->x();
        this->m_y -= sender->getMass().getScalar() * sender->y();

        setMass(getMass() - sender->getMass());
		
		//Now erase it
		m_CelestialObjects.erase(it);
    }
    else
    {
        return -1;
    }
    return 0;
}
