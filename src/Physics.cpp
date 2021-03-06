#include "stdafx.h"
#include "Physics.h"
#include "Polygon.h"

namespace PE
{
void PhysicObject::finishPhysicsCycle(double time)
{
    //Get the acceleration
    setAcceleration(m_force / getMass());

    //Derive the velocity
    setVelocity(getVelocity() + Velocity(getAcceleration(), Time(time / 1000.)));

    //Reset the force
    this->m_force.setVector(PE::Vector2df { 0.,0. });
}

SFG::Pointer<SFG::Polygon> PhysicObject::transrotatedPhysicsMesh(int lod, double rot, const PE::Vector2df& trans)
{
	assert(lod >= 0 && lod < 3);
	return m_physics_mesh[lod].transrotatedPolygon(rot, trans);
}

SFG::Pointer<CollisionDetail> PhysicObject::collide(const PE::PhysicObject* inc) const
{
    //Safety first
    assert(inc != nullptr);

    //Get quick links
    const auto& vel = inc->getVelocity();
    const auto& mass = inc->getMass();
    const PE::Vector2df pos( {
        inc->x(), inc->y()
    });

    //Get local quick links
    const auto& localvel = this->getVelocity();
    const auto& localmass = this->getMass();
    const PE::Vector2df localpos( {
        this->x(), this->y()
    });

    //Now we have a nice little equation to solve:
    //localpos + localvel*t = pos + vel*t
    //-> localpos - pos = vel*t - localvel*t
    //-> (localpos - pos) / (vel - localvel) = t
    mpf_class tmp = (localpos.x - pos.x) / (vel.getVector().x - localvel.getVector().x);
    double eta = tmp.get_d();
    //Get a (really) rough estimate whether the objects will collide
    bool b_collides = (abs((localpos.y - pos.y) / (vel.getVector().y- localvel.getVector().y) - eta) < 5);



    //Also, get the (elastic) impulse:
    //v_1' = 2*(m_1*v_1 + m_2*v_2)/(m_1 + m_2) - v_1
    //(dito for v_2')
    PE::Vector2df reusable = 	(vel.getVector() * mass.getScalar()
                                 + localvel.getVector() * localmass.getScalar())
                                / (mass.getScalar() + localmass.getScalar());
    PE::Vector2df newvel = reusable*2.f - vel.getVector();
    PE::Vector2df newlocalvel = reusable*2.f - localvel.getVector();


    //Create the collision object
    SFG::Pointer<CollisionDetail> ptr(new CollisionDetail(b_collides, eta,
                                      *this, *inc,
                                      PE::Velocity(newlocalvel),  PE::Velocity(newvel)));

    //Return the created object
    return ptr;
}

PhysicsEngine::PhysicsEngine()
{
}


PhysicsEngine::~PhysicsEngine()
{
    printf("In Destructor...\n");
    removeAllElements();
}

Acceleration::Acceleration(const Force& f, const Mass& m)
{
    assert(m.getScalar() != 0.0);
    this->m_vec.x = f.getVector().x / m.getScalar();
    this->m_vec.y = f.getVector().y / m.getScalar();
}


void PhysicsEngine::addObject(const SFG::Pointer<PhysicObject>& ptr)
{
    //We need to listen to the destruction of this object
    ptr->addListener(	this,
                        MsgObjectAction(std::bind(					//Bind the message function
                                            &PhysicsEngine::object_message_handler, //To the message handler
                                            this,									//Of this instance
                                            std::placeholders::_1,					//With open arguments
                                            std::placeholders::_2)));

    //Add the pointer to the map
    this->m_objects[ptr.getElement()] = ptr;
}

void PhysicsEngine::removeObject(const SFG::Pointer<PhysicObject>& ptr)
{
    this->removeListener(ptr.getElement());

}

int PhysicsEngine::object_message_handler(int msg, const SFG::Pointer<MsgPackage>& pkg)
{
    if (msg == PE::PhysicObject::PhysicObjectMessages::Destroyed)
    {
        //First argument is the sender
        PhysicObject* sender = (PhysicObject*)pkg->getValue(0);
        //Remove this guy from the object list
        this->m_objects.erase(sender); //#TODO: Check if this actually calls the destructor
    }
    else
    {
        return -1;
    }
    return 0;
}

void PhysicsEngine::removeAllElements() {
#ifdef _DEBUG
    for (auto e : m_objects)
    {
        e.second->removeListener(this);
        e.second.release();
    }
#endif
    m_objects.clear();
}

void PhysicsEngine::applyMutualForces(const Mass& threshhold)
{
    for (auto obj1 = m_objects.begin(); obj1 != m_objects.end(); ++obj1)
    {
        if (obj1->first->isActive())
        {
            auto cpy = obj1;
            ++cpy;
            for (auto& obj2 = cpy; obj2 != m_objects.end(); ++obj2)
            {
                //Continue condition
				if(	obj1->first == obj2->first ||
					!obj2->first->isActive() ||
					obj1->first->getMass().getScalar() * obj2->first->getMass().getScalar() < threshhold.getScalar())
                {
                    continue;
                }
                //Calculate force
                PE::Vector2df dist( { obj2->first->x() - obj1->first->x(), obj2->first->y() - obj1->first->y() });
                Force f = dist*(GRAVITY_CONSTANT * obj1->first->getMass().getScalar() * obj2->first->getMass().getScalar()) / pow(dist.absLength().get_d(), 3.0);
                //Add the force
                obj1->first->addForceToCycle(f);

                //Add the negated force to the 2nd object
                obj2->first->addForceToCycle(f*(-1.));

                //printf("Frc (%f|%f)\n", f.getVector().x, f.getVector().y);
            }
        }
    }
}
};
