#include <SSG_CelestialObjectContainer.h>


template <typename T>
SSG_CelestialObjectContainer<T>::SSG_CelestialObjectContainer()
{
    m_x = 0.;
    m_y = 0.;
	this->setClick([&](const sf::Vector2f& mpos, const sf::Mouse::Button& but){
		return -1;
	});
	this->setRightClick([&](const sf::Vector2f& mpos, const sf::Mouse::Button& but){
		return -1;
	});
	this->setHover([&](const sf::Vector2f& mpos){
		return -1;
	});
}

template<typename T>
sf::FloatRect SSG_CelestialObjectContainer<T>::getLogicBoundingRect() const
{
	float l, r, t, b;
	l = std::numeric_limits<float>::max();
	r = std::numeric_limits<float>::min();
	t = std::numeric_limits<float>::max();
	b = std::numeric_limits<float>::min();
	
	//for(const auto& o : m_CelestialObjects)
	for(size_t i = 0; i < m_CelestialObjects.size(); i++)
	{
		auto& o = m_CelestialObjects[i];
		sf::FloatRect rect = o->getLogicBoundingRect();
		l = std::min(l, rect.left);
		r = std::max(r, rect.left + rect.width);
		t = std::min(t, rect.top);
		b = std::max(b, rect.top + rect.height);
	}
	
	return sf::FloatRect(l, r-l, t, b-t);
}

template<typename T>
int SSG_CelestialObjectContainer<T>::update(float dt)
{
	this->m_physicsEngine.applyMutualForces();
	this->finishPhysicsCycle(dt);
	/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
		"Updating \"%s\" with %d objects", m_name.toAnsiString().c_str(),
						m_CelestialObjects.size()
	);*/
	#pragma omp parallel for
	for(size_t i = 0; i < m_CelestialObjects.size(); i++)
	{
		auto& g = m_CelestialObjects[i];
		//Check for mouse actions
		//for(auto& action : this->m_requests)
		for(size_t ii = 0; ii < m_requests.size(); ii++)
		{
			auto& action = m_requests[ii];
			if(action->isActive())
			{
				if(action->m_task_type == delayedActionTask::TaskType::mouseRequest)
				{
					auto req = static_cast<mouseRequest*>(action->m_request);
					//if(!this->getLogicBoundingRect().contains(req->pos)) continue;
					/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
						"%s: (%f|%f) %f:%f", g.first->m_name.toAnsiString().c_str(), g.first->getLogicBoundingRect().left, g.first->getLogicBoundingRect().top,
										g.first->getLogicBoundingRect().width, g.first->getLogicBoundingRect().height
					);*/
					if(g->getLogicBoundingRect().contains(req->pos))
					{
						if(g->onClick(req->pos, req->button) == 0)
						{
							//We're done
							/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
								"Event has been consumed"
							);*/
							#pragma omp critical
							{
								action->deactivate();
							}
						}
						else
						{
							//Handle more...
							/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
							"Relaying event to \"%s\"", g.first->m_name.toAnsiString().c_str()
						);*/
						//g.first->addDelayedActionTask(action);
						}
					}
					#pragma omp critical
					{
						g->addDelayedActionTask(action);
					}
				}
			}
		}
		g->update(dt);
	}
	return 0;
}

template<typename T>
void SSG_CelestialObjectContainer<T>::removeObjectFromSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
{
    m_physicsEngine.removeObject(ptr);
    //Remove from the mass center
    m_x -= ptr->getMass().getScalar() * ptr->x();
    m_y -= ptr->getMass().getScalar() * ptr->y();

    this->setMass(getMass() - ptr->getMass());
}

template<typename T>
void SSG_CelestialObjectContainer<T>::addObjectToSystem(const SFG::Pointer<PE::PhysicObject>& ptr)
{
    this->setMass(getMass() + ptr->getMass());
    m_physicsEngine.addObject(ptr);
    //We now need to add the object's mass to totalmass and
    //add the specific weights to the balanced position
    m_x += ptr->getMass().getScalar() * ptr->x();
    m_y += ptr->getMass().getScalar() * ptr->y();
}