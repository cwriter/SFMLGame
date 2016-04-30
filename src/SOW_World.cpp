#include "stdafx.h"
#include "SOW_World.h"



SOW_World::SOW_World()
{
	this->m_restricted_to_active_scenes = true;
}

SOW_World::~SOW_World()
{
	for (auto s : m_active_scenes)
	{
		//s->removeObjects([](GObjectBase* g) {return true; });
	}
	for (auto o : m_objects)
	{
		delete o; //Destroy everything
	}
}

int SOW_World::optimize(sf::Int64 max_time, sf::FloatRect& rect, int flags)
{
	sf::Clock clock;
	int ret = optimize_sub_loop(flags, clock, rect, max_time);
	if (ret != 0) {
		if (ret == -1)
			return 0;
		else
			return ret;
	}

	ret = optimize_sub_loop(flags ^ flags, clock, rect, max_time);

	if (ret != 0) {
		if (ret == -1)
			return 0;
		else
			return ret;
	}
	return 0;
}

int SOW_World::optimize_rects(sf::Int64 max_time, std::vector<sf::FloatRect*>& rects, int flags)
{
	sf::Clock clock;
	for (auto r : rects)
	{
		if (optimize(max_time, *r, flags) == 1)
			return 1;
		max_time -= clock.restart().asMicroseconds();
	}
	return 0;
}

int SOW_World::optimize_sub_loop(int flags, sf::Clock& clock, sf::FloatRect& rect, sf::Int64 max_time)
{
	if (last_optimize_index >= m_objects.size())
		last_optimize_index = 0; //Reset the index
	for (; last_optimize_index < this->m_objects.size(); last_optimize_index++) {

		if (m_objects[last_optimize_index]->getSprite().getGlobalBounds().intersects(rect))
		{
			if (flags & AllocateFirstDeleteLast) {
				if (!m_objects[last_optimize_index]->loaded) {
					//Need to load
					m_objects[last_optimize_index]->preload();
				}
			}
		}
		else
		{
			if (flags & DeleteFirstAllocateLast) {
				if (m_objects[last_optimize_index]->loaded) {
					//Need to load
					m_objects[last_optimize_index]->unload();
				}
			}
		}

		//Break if time requires
		if (clock.getElapsedTime().asMicroseconds() >= max_time) {
			return 1;
		}
	}
	//If Balanced or NoDelete is set, tell the caller that we have finished already
	if (flags & Balanced || flags & NoDelete) {
		return -1;
	}
	return 0;
}

int SOW_World::draw(sf::RenderTarget& target, const sf::FloatRect& trect, const sf::FloatRect& srect)
{
	/*//Create the scene
	Scene2D scene;
	scene.selectObjects(&this->m_objects, [=](GObjectBase* obj) {
		return (srect.intersects(obj->getBoundingRect())); //Add object if it need to be drawn (i.e. when it intersects with the source rect)
	});
	scene.optimize(); //Sort by depth
	scene.draw(target, trect);*/
	if (addScene(srect) != 0)
	{
		//Error

		return -1;
	}
	m_active_scenes.back()->draw(target, trect);

	return 0;
}

int SOW_World::draw(sf::RenderTarget& target, const sf::FloatRect& srect)
{
	for (auto s : this->m_active_scenes)
	{
		if (s->rect().intersects(srect))
		{
			s->draw(target, srect); //This will draw globally
		}
	}
	return 0;
}

int SOW_World::addScene(const sf::FloatRect& rect)
{
	SFG::Pointer<Scene2D> p((Scene2D*)new Scene2D);
	if (!p.isValid()) {
		printf("[Error] Allocation error in %s:%d\n", __FILE__, __LINE__);
		return -1;
	}

	//Get the objects in the required rect
	p->selectObjects(&this->m_objects, [=](const GObjectBase* obj) {
		return (rect.intersects(obj->getBoundingRect())); //Add object if it need to be drawn (i.e. when it intersects with the source rect)
	});

	//Sort by depth
	p->optimize();

	//Add to scene vector
	m_active_scenes.push_back(p);

	return 0;
}

int SOW_World::load(const sf::String& path)
{
	sf::String out;
	int ret = basicLoadFile(path, out);
	if (ret != 0)
	{
		printf("[Error] Failed to load file in %s:%d with code %d.\n", __FILE__, __LINE__, ret);
		return ret;
	}

	XMLReader rdr;
	rdr.setSource(out);
	ret = rdr.parse();
	if (ret != 0)
	{
		printf("[Error] Failed to parse xml data in %s:%d with code %d.\n", __FILE__, __LINE__, ret);
		return ret;
	}

	//Get the data group handle
	auto handle = rdr.getXMLGroupHandle(L"xml/World/");
	if (handle == nullptr)
	{
		printf("[Error] Failed to locate World group in xml file in %s:%d.\n", __FILE__, __LINE__);
		return -1;
	}

	//Get Buildings
	rdr.for_all("xml/World/Building", std::bind(&SOW_World::building_loader, this, std::placeholders::_1));

	return 0;
}

void SOW_World::building_loader(const XMLGroup* group)
{
	SOW_Building bld;
	XMLReader reader(*group);
	int ret = bld.load(reader);
	if (ret != 0)
	{
		printf("[Error] Failed to load building in %s:%d\n", __FILE__, __LINE__);
	}
}

SFG::Pointer<ContextOptionList> SOW_World::getContextOptions(const sf::Vector3f& pos, const float max_dist)
{
	SFG::Pointer<ContextOptionList> ret(new ContextOptionList());
	for (auto s : m_active_scenes)
	{
		if (s->rect().contains(pos.x, pos.y))
		{
			//2D positioning checked, we're good (meaning we don't have to check the entire world for elements)
			for (auto o : *s->objects())
			{
				//Add elements to list that are within bounds
				//CODE CHANGES HERE HAVE TO BE APPLIED TO THE CODE BELOW AS WELL!
				if (pow(SFG::FloatRect(o->getBoundingRect()).distanceTo(pos.x, pos.y), 2.0f) + pow(pos.z - o->getDepth(), 2.f) <= pow(max_dist, 2.f))
				{
					//Add now
				}
			}
		}
	}
	//Still nothing found. That's bad, mainly because we have to check every single object now

	if (!m_restricted_to_active_scenes)
	{
		//Only if allowed: Check EVERYTHING (usually: DON'T)
		for (auto o : this->m_objects)
		{
			//CODE CHANGES HERE HAVE TO BE APPLIED TO THE CODE ABOVE AS WELL!
			if (pow(SFG::FloatRect(o->getBoundingRect()).distanceTo(pos.x, pos.y), 2.0f) + pow(pos.z - o->getDepth(), 2.f) <= pow(max_dist,2.f))
			{
				//Add now
			}
		}
	}

	return ret;
}


//NOTIFIES HERE (might have to move them to .h in future):

template <> int SOW_World::notify(int msg, GObjectBase* sender)
{
	if (msg == GObjectBase::GObjectMessages::PositionChanged ||
		msg == GObjectBase::GObjectMessages::ScaleChanged)
	{
		//If position changed, try to set this to an existing scene if necessary (otherwise ignore)
		for (auto v : this->m_active_scenes)
		{
			if (sender->getBoundingRect().intersects(v->rect()))
			{
				//add to this scene
				v->addObject(sender);
			}
		}
	}
	else if (msg == GObjectBase::Destroyed)
	{
		//This should not happen.
		assert(false);
	}
	return 0;
}

template <> int SOW_World::notify(int msg, Scene2D* sender)
{
	if (msg == Scene2D::Scene2DMessages::RequireReEvaluation)
	{
		//Scene needs to be reset.
		sender->selectObjects(&this->m_objects, [=](const GObjectBase* obj) {
			return (sender->rect().intersects(obj->getBoundingRect())); //Add object if it intersects the source rect
		});
		//Done
		return 0;
	}

	return 0;
}

template <typename T, typename S> int SOW_World::notify(int msg, T* sender, S* object)
{
	printf("[Error] int notify was overriden, but does not provide solutions for this type of sender in %s:%d\n", __FILE__, __LINE__);
	return -1;
}
int SOW_World::dynamicNotify(int msg, const SFG::Pointer<MsgPackage>& pkg)
{

    if (msg == Scene2D::Scene2DMessages::ObjectAbandoned)
	{
		//Scene has abandoned object, take care of it again

		Scene2D* sender = (Scene2D*)pkg->getValue(0); //The first parameter is the sender
		GObjectBase* object = (GObjectBase*)pkg->getValue(1); //The second parameter is the object

		object->addListener(this);
	}
    return 0;
}

