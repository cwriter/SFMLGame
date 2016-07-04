#include "stdafx.h"
#include "SSG_Game.h"
#include <XML.h>



SSG_Game::SSG_Game()
{
    this->name = "main_gamestate";
    this->m_cam.setSize(10000.f, 10000.f);
}

SSG_Game::~SSG_Game()
{
    this->m_physicsEngine.removeAllElements();
}

int SSG_Game::update(double dt)
{
	//TESTING
	m_cam_counter++;
	if(m_cam_counter % 20 == 0)
			printf("%d\n", m_cam_counter);
	m_cam.update(dt);
	if(m_cam_counter >= 250)
	{
		m_cam_counter = 0;
		printf("Switching target=(%f->", m_cam_index);
		m_cam_index = ++m_cam_index % m_next_planets.size();
		printf("%f)================================.\n", m_cam_index);
		this->m_lock_on.reset(m_next_planets[m_cam_index].cast<PE::PhysicObject>());
		m_cam.setSize(m_next_planets[m_cam_index]->getShape().getGlobalBounds().width*250.f,
					  m_next_planets[m_cam_index]->getShape().getGlobalBounds().height*250.f);
	}
	//!TESTING

    this->m_physicsEngine.applyMutualForces();
	
	//#TODO: REMOVE THE LINE BELOW THE COMMENT AND UN-COMMENT THE COMMENT FOR REAL VALUES
	//this->m_universe.update(dt);
	this->m_universe.update(dt*100000.f);

    /*this->moon->update(float(dt));

    this->planet->update(float(dt));

    this->sun->update(float(dt));*/
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
                m_cam.getView().getSize().x + ratio*events[i].mouseWheel.delta*1000.f,
                m_cam.getView().getSize().y + 1.f*events[i].mouseWheel.delta*1000.f);
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
	sf::String universepath(path + "example - universe.xml");
	
    if(basicLoadFile(universepath, out) != 0)
	{
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
							"Failed to load file \"%s\"", universepath.toAnsiString().c_str());
	}
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
    XMLReader versereader(*reader.getXMLGroupHandle("xml/universe/"));
    int ret = m_universe.load(versereader);
    if(ret != 0)
    {
        SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
                            "Loading the universe at path \"%s\" failed with code %d",
							universepath.toAnsiString().c_str(), ret);
    }
    else
	{
		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
							"Loading universe from \"%s\" succeeded.", 
							universepath.toAnsiString().c_str());
	}

	
	//TESTING
	//Find planet by name
	const auto ptr = m_universe.findPlanet("Sol");
	if(!ptr.isValid())
	{
		SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__,
							"Failed to get planet position");
	}
	else
		this->m_lock_on.reset(ptr.cast<PE::PhysicObject>());
	
	
	m_cam_counter = 250;
	m_cam_index = 4;
	
	
	this->m_next_planets.push_back(m_universe.findPlanet("Sol"));
	this->m_next_planets.push_back(m_universe.findPlanet("Mercury"));
	this->m_next_planets.push_back(m_universe.findPlanet("Venus"));
	this->m_next_planets.push_back(m_universe.findPlanet("Terra"));
	this->m_next_planets.push_back(m_universe.findPlanet("Luna"));
	this->m_next_planets.push_back(m_universe.findPlanet("Mars"));
	this->m_next_planets.push_back(m_universe.findPlanet("Jupiter"));
	this->m_next_planets.push_back(m_universe.findPlanet("Saturn"));
	this->m_next_planets.push_back(m_universe.findPlanet("Uranus"));
	this->m_next_planets.push_back(m_universe.findPlanet("Neptune"));
	this->m_next_planets.push_back(m_universe.findPlanet("Pluto"));
	
	size_t index = 0;
	for(auto p : m_next_planets)
	{
		switch(index)
		{
			case 0:
				p->getShape().setFillColor(sf::Color::Green);
				break;
			case 1:
				p->getShape().setFillColor(sf::Color::Cyan);
				break;
			case 2:
				p->getShape().setFillColor(sf::Color::Yellow);
				break;
			case 3:
				p->getShape().setFillColor(sf::Color::Blue);
				break;
			case 4:
				p->getShape().setFillColor(sf::Color::White);
				break;
			case 5:
				p->getShape().setFillColor(sf::Color::Magenta);
				break;
			case 6:
				p->getShape().setFillColor(sf::Color::Blue);
				break;
			case 7:
				p->getShape().setFillColor(sf::Color::Blue);
				break;
			case 8:
				p->getShape().setFillColor(sf::Color::Blue);
				break;
			case 9:
				p->getShape().setFillColor(sf::Color::Blue);
				break;
		}
		printf("Planet \"%s\" is at %f|%f\n", p->getName().toAnsiString().c_str(), p->x(), p->y());
		index++;
	}
	//!TESTING
	
    return 0;
}

void SSG_Game::draw(sf::RenderTarget* t)
{
    assert(t != nullptr);

    /*//Set the camera
    this->m_cam.setCenter(moon->getSprite().getPosition().x, moon->getSprite().getPosition().y);
    t->setView(m_cam);

    //Draw items
    this->sun->draw(*t);
    this->planet->draw(*t);
    this->moon->draw(*t);
	*/
	
	//TESTING
	this->m_cam.setCenter((float)m_lock_on->x().get_d(), (float)m_lock_on->y().get_d());
	/*printf("Dist is %f|%f\n", 
		   m_lock_on->x() - m_cam.getView().getCenter().x,
		   m_lock_on->y() - m_cam.getView().getCenter().y);*/
	printf("Cam locking at %f|%f\n", m_lock_on->x().get_d(), m_lock_on->y().get_d());
	//m_cam.animatedPanTo((float)m_lock_on->x(), (float)m_lock_on->y());
	t->setView(m_cam);
	//!TESTING
	this->m_universe.draw(t);

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

