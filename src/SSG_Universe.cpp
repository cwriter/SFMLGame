#include "stdafx.h"
#include "SSG_Universe.h"



SSG_Universe::SSG_Universe()
{
}


SSG_Universe::~SSG_Universe()
{
}

void SSG_Universe::addShip(SFG::Pointer<SSG_Ship>& ptr)
{
	assert(ptr.isValid());
	m_ships[ptr.getElement()] = ptr;
	m_physics_engine.addObject(ptr);

	//ptr.cast<GObjectBase>()->addListener(this,
	ptr->addListener(this,
		MsgObjectAction(std::bind(&SSG_Universe::object_message_handler_ships,
			this, std::placeholders::_1, std::placeholders::_2)));
}

void SSG_Universe::addGalaxy(SFG::Pointer<SSG_Galaxy>& ptr)
{
	assert(ptr.isValid());
	m_galaxies[ptr.getElement()] = ptr;
	m_physics_engine.addObject(ptr);

	//ptr.cast<GObjectBase>()->addListener(this,
	ptr->addListener(this,
		MsgObjectAction(std::bind(&SSG_Universe::object_message_handler_galaxies,
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
		this->m_galaxies.erase(sender); //#TODO: Check if this actually calls the destructor
	}
	else
	{
		return -1;
	}
	return 0;
}