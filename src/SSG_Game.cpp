#include "stdafx.h"
#include "SSG_Game.h"



SSG_Game::SSG_Game()
{
	this->name = "main_gamestate";
	this->m_cam.setSize(10000.f, 10000.f);

	moon.reset(new SSG_Planet);
	planet.reset(new SSG_Planet);
	sun.reset(new SSG_Planet);

	sun->setMass(10e18);
	sun->getShape().setFillColor(sf::Color::Yellow);
	sun->setPosition(-200.f, -10000.f);

	this->planet->setMass(10e18);
	this->planet->setVelocity(PE::Velocity(400., 0.));

	this->moon->setMass(SSG_PLANET_DEFAULT_MASS / 1000.f);
	this->moon->getShape().setFillColor(sf::Color(255, 255, 255));
	this->moon->setPosition(0.f, 2000.f);
	this->moon->setVelocity(PE::Velocity(1000., 0.));
	this->moon->getShape().setRadius(SSG_PLANET_DEFAULT_RADIUS / 100.f);

	SFG::FloatRect rect(this->moon->getShape().getLocalBounds());
	this->moon->getShape().setOrigin(rect.center());
	SFG::FloatRect rect2(this->planet->getShape().getLocalBounds());
	this->planet->getShape().setOrigin(rect2.center());
	SFG::FloatRect rect3(this->sun->getShape().getLocalBounds());
	this->sun->getShape().setOrigin(rect3.center());

	this->m_physicsEngine.addObject(SFG::Pointer<PE::PhysicObject>(moon));
	this->m_physicsEngine.addObject(SFG::Pointer<PE::PhysicObject>(planet));
	this->m_physicsEngine.addObject(SFG::Pointer<PE::PhysicObject>(sun));
}

SSG_Game::~SSG_Game()
{
	moon.release();
	this->m_physicsEngine.removeAllElements();
}

int SSG_Game::update(double dt)
{

	this->m_physicsEngine.applyMutualForces();

	this->moon->update(float(dt));

	this->planet->update(float(dt));

	this->sun->update(float(dt));
	//this->planet->update(float(dt));
	return 0;
}

int SSG_Game::processEvents(SFG::Window& window, std::vector<sf::Event>& events)
{
	for (size_t i = 0; i < events.size(); i++)
	{
		//#TODO: Consider remapping with this->keyMapper()
		if (events[i].type == sf::Event::EventType::MouseWheelMoved)
		{
			float ratio = m_cam.getView().getSize().x / m_cam.getView().getSize().y;
			this->m_cam.setSize(
				m_cam.getView().getSize().x + ratio*events[i].mouseWheel.delta*50.f,
				m_cam.getView().getSize().y + 1.f*events[i].mouseWheel.delta*50.f);
			//printf("Scrolled %d\n", events[i].mouseWheel.delta);
			window.getSFMLWindow().setView(m_cam);
		}
		else if (events[i].type == sf::Event::EventType::Resized)
		{
			float oldw = m_cam.getView().getSize().x;
			float oldh = m_cam.getView().getSize().y;
			this->m_cam.setSize(float(events[i].size.width), float(events[i].size.height));
			this->m_cam.setCenter(m_cam.getView().getCenter().x + (events[i].size.width - oldw) / 2.f,
				m_cam.getView().getCenter().y + (events[i].size.height - oldh) / 2.f);

			window.getSFMLWindow().setView(m_cam);
		}
	}
	window.getSFMLWindow().setView(m_cam);
	return 0;
}

int SSG_Game::load(const sf::String& path)
{
	//We got our path, please start loading
	sf::String out;
	basicLoadFile(path, out);
	XMLReader reader;
	//Set the source
	reader.setSource(out);
	//Parse
	if (reader.parse() != 0)
	{
		std::string st;
		SFG::Util::getStackTrace(st);
		printf("[Error] Failed to parse XML data in %s:%d\n.%s\n", __FILE__, __LINE__, st.c_str());
		return -1;
	}
	//Load actual data


	return 0;
}

void SSG_Game::draw(sf::RenderTarget* t)
{
	assert(t != nullptr);

	//Set the camera
	this->m_cam.setCenter(moon->getSprite().getPosition().x, moon->getSprite().getPosition().y);
	t->setView(m_cam);

	//Draw items
	this->sun->draw(*t);
	this->planet->draw(*t);
	this->moon->draw(*t);


}

bool SSG_Game::pause(bool p)
{
	//The code here are just recommendations.
	//For example, if you want to be able to load data during pause, you should not stop the system
	if (p)
	{
		this->m_system.halt_thread(); //Care: Can block (Estimated time < 60 ms)
	}

	if (!p)
	{
		this->m_system.continue_thread(); //Will resume the system task execution
	}
	return false;
}
