#include "stdafx.h"
#include "SSG_Game.h"
#include <XML.h>
#include "Game.h"


SSG_Game::SSG_Game()
{
    this->name = "main_gamestate";
    this->m_cam.setSize(1000.f, 1000.f);
	this->m_mouse_mode = MouseMode::Select;
	
	this->m_build_overlay.m_enabled.onValueChange = [&](const bool& b){
		if(b) this->m_mouse_mode = MouseMode::BuildOverlayPassThrough;
		else this->m_mouse_mode = MouseMode::Select;
	};
	
	this->m_cam_index = 0;
	
	if(!sfg::Context::Get().GetEngine().SetProperty<std::string>("*", "FontName", "/usr/share/fonts/truetype/dejavu/DejaVuSansMono.ttf"))
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to set font");
	
	this->m_game_speed = 1.f;
	
	Game::cmdTranslator.setVar<float>("game_speed", &m_game_speed);
}

SSG_Game::~SSG_Game()
{
    this->m_physicsEngine.removeAllElements();
}

int SSG_Game::update(double dt)
{
	//TESTING
	m_cam_counter++;
	/*if(m_cam_counter % 20 == 0)
			printf("%zu\n", m_cam_counter);*/
	m_cam.update(dt);
	if(m_cam_counter >= 250)
	{
		m_cam_counter = 0;
		//printf("Switching target=(%zu->", m_cam_index);
		m_cam_index = (m_cam_index + 1) % m_next_planets.size();
		//printf("%zu)================================.\n", m_cam_index);
		this->m_lock_on.reset(m_next_planets[m_cam_index].cast<PE::PhysicObject>());
		//m_cam.setSize(m_next_planets[m_cam_index]->getShape().getGlobalBounds().width*10.f,
		//			  m_next_planets[m_cam_index]->getShape().getGlobalBounds().height*10.f);
		/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
			"Lock_on is @ (%f|%f) %f:%f", m_next_planets[m_cam_index]->getLogicBoundingRect().left, m_next_planets[m_cam_index]->getLogicBoundingRect().top,
							m_next_planets[m_cam_index]->getLogicBoundingRect().width, m_next_planets[m_cam_index]->getLogicBoundingRect().height
		);*/
	}
	//!TESTING

    this->m_physicsEngine.applyMutualForces();
	
	this->m_universe.update(dt*float(m_game_speed));

	if(this->m_build_overlay.isEnabled()) {
		if(m_cam.rdl != SFG::Camera::Low)
			this->m_build_overlay.update(dt);
	}
	//Update gui
    m_uiman.update(dt);
    //Clear any abandoned tasks
    m_universe.clearDelayedActionTasks();
    
    return 0;
}

int SSG_Game::processEvents(SFG::Window& window, std::vector<sf::Event>& events)
{
	if(this->m_build_overlay.isEnabled()) {
		this->m_build_overlay.processEvents(events);
	}
	
    for (size_t i = 0; i < events.size(); i++)
    {
        //#TODO: Consider remapping with this->keyMapper()
        if (events[i].type == sf::Event::EventType::MouseWheelMoved)
        {
			float scale = (10.f - 1.f*events[i].mouseWheel.delta) / 10.f;
			
			if(scale < 0.f) printf("ERROR: NEGATIVE SCALE!");
			
			if(m_cam.rdl == SFG::Camera::Low)
				this->m_cam.setSize(
					m_cam.getView().getSize().x * scale,
					m_cam.getView().getSize().y * scale);
			else if(m_cam.rdl == SFG::Camera::Medium)
			{
				this->m_cam.setSize(
					m_cam.getView().getSize().x * scale,
					m_cam.getView().getSize().y * scale);
			}
			else if(m_cam.rdl == SFG::Camera::Detailed)
			{
				this->m_cam.setSize(
					m_cam.getView().getSize().x * scale,
					m_cam.getView().getSize().y * scale);
			}
			
            //printf("Scrolled %d\n", events[i].mouseWheel.delta);
            window.getSFMLWindow().setView(m_cam);
			
			//Invalidate
			events[i].type = sf::Event::EventType::Count;
        }
        else if (events[i].type == sf::Event::EventType::Resized)
        {
            float oldw = window.getSFMLWindow().getSize().x;
            float oldh = window.getSFMLWindow().getSize().y;
			
			float rescalex = events[i].size.width / oldw;

			float ratio = double(events[i].size.width) / double(events[i].size.height);
            this->m_cam.setSize(m_cam.getView().getSize().x * ratio * rescalex, m_cam.getView().getSize().x * rescalex);
            this->m_cam.setCenter(m_cam.getView().getCenter().x + (/*events[i].size.width - */oldw) / 2.f,
                                  m_cam.getView().getCenter().y + (/*events[i].size.height - */oldh) / 2.f);

            window.getSFMLWindow().setView(m_cam);
			
			//Invalidate
			events[i].type = sf::Event::EventType::Count;
        }
        else if (events[i].type == sf::Event::EventType::MouseButtonPressed)
		{
			//We _could_ handle that shit for MouseMode::Select, but naaaaah
			if(this->m_mouse_mode == MouseMode::BuildOverlayPassThrough)
			{
				if(events[i].mouseButton.button == sf::Mouse::Button::Left)
				{
					//Start construction of beam (mouse button release = stop construction of beam)
					//TODO
					
				}
			}
		}
		else if (events[i].type == sf::Event::EventType::MouseButtonReleased)
		{
			//We rather use that one
			if (this->m_mouse_mode == MouseMode::Select)
			{
				if (events[i].mouseButton.button == sf::Mouse::Button::Left)
				{
					sf::Vector2f pos = window.getSFMLWindow().mapPixelToCoords(
						sf::Vector2i(events[i].mouseButton.x, events[i].mouseButton.y),
						m_cam.getView());
					mouseRequest* mreq = new mouseRequest; //The task will later take care of deleting this object
					
					mreq->pos = pos;
					mreq->button = events[i].mouseButton.button;
					
					/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
										"Mouse @ (%f|%f)", mreq->pos.x, mreq->pos.y);*/
					
					SFG::Pointer<delayedActionTask> ptr(new delayedActionTask);
					ptr->m_request = mreq;
					ptr->m_task_type = delayedActionTask::TaskType::mouseRequest;
					
					if(m_cam.rdl == SFG::Camera::Low)
						m_universe.addDelayedActionTask(ptr);
				}
			}
		}
    }
    window.getSFMLWindow().setView(m_cam);
    return 0;
}

int SSG_Game::load(const sf::String& path)
{
	sf::String baseoverlaypath(path + "Buildoverlay.xml");
	int boret = m_build_overlay.load(baseoverlaypath);
	if(boret != 0)
	{
		SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__,
			"Failed to load overlay with Code %d", boret);
	}
	//m_build_overlay.enable(*UI()->getTarget());
	
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
    //Set the desktop
    m_universe.setGuiDesktop(&this->UI()->desktop());
	
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
	
	
	/*this->m_next_planets.push_back(m_universe.findPlanet("Sol"));
	this->m_next_planets.push_back(m_universe.findPlanet("Mercury"));
	this->m_next_planets.push_back(m_universe.findPlanet("Venus"));*/
	this->m_next_planets.push_back(m_universe.findPlanet("Terra"));
	this->m_next_planets.push_back(m_universe.findPlanet("Luna"));
	/*this->m_next_planets.push_back(m_universe.findPlanet("Mars"));
	this->m_next_planets.push_back(m_universe.findPlanet("Jupiter"));
	this->m_next_planets.push_back(m_universe.findPlanet("Saturn"));
	this->m_next_planets.push_back(m_universe.findPlanet("Uranus"));
	this->m_next_planets.push_back(m_universe.findPlanet("Neptune"));
	this->m_next_planets.push_back(m_universe.findPlanet("Pluto"));*/
	
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
		printf("Planet \"%s\" is at %f|%f\n", p->getName().toAnsiString().c_str(), p->x().get_d(), p->y().get_d());
		index++;
	}
	
	//Load a ship
	int sret = 0;
	SFG::Pointer<SSG_Ship> p(new SSG_Ship());
	sf::String shippath = path + "example - ship.xml";
	XMLReader shipreader;
	sret = basicLoadFile(shippath, out);
	assert(sret == 0);
	shipreader.setSource(out);
	sret = shipreader.parse();
	assert(sret == 0);
	XMLReader sread(*shipreader.getXMLGroupHandle("xml/ship/"));
	sret = p->load(sread);
	assert(sret == 0);
	this->m_universe.addShip(p);
	
	//!TESTING
	
    return 0;
}

int SSG_Game::unload()
{
    for (auto m : this->m_modules)
    {
        m->unload();
    }
    return 0;
}

int SSG_Game::init(SFG::Window& win)
{
	this->UI()->setTarget(&win.getSFMLWindow());
	m_build_overlay.setTarget(win);
	//m_build_overlay.enable(win.getSFMLWindow());
	float ratio = double(win.getSFMLWindow().getSize().x) / double(win.getSFMLWindow().getSize().y);
    this->m_cam.setSize(float(win.getSFMLWindow().getSize().x), float(win.getSFMLWindow().getSize().y*ratio));
	//TESTING
	this->m_cam.setSize(1.0e10 * ratio, 1.0e10);
	//!TESTING
    return 0;
}


void SSG_Game::draw(sf::RenderTarget* t)
{
    assert(t != nullptr);
	
	//this->m_cam.setCenter((float)m_lock_on->x().get_d(), (float)m_lock_on->y().get_d());
	if(m_cam_index < m_next_planets.size() && m_next_planets[m_cam_index].isValid() && m_next_planets[m_cam_index]->m_planet_surface.isValid())
	{
		const auto& pl = m_next_planets[m_cam_index];
		if(m_cam.rdl == SFG::Camera::Low)
			this->m_cam.setCenter(
				pl->x().get_d() + cos(pl->m_planet_surface->getMainPosition().x.get_d()) * pl->m_planet_surface->getMainPosition().y.get_d(),
				pl->y().get_d() + sin(pl->m_planet_surface->getMainPosition().x.get_d()) * pl->m_planet_surface->getMainPosition().y.get_d()
			);
		else if(m_cam.rdl == SFG::Camera::Medium)
		{
			this->m_cam.setCenter(
				cos(pl->m_planet_surface->getMainPosition().x.get_d()) * pl->m_planet_surface->getMainPosition().y.get_d(),
				sin(pl->m_planet_surface->getMainPosition().x.get_d()) * pl->m_planet_surface->getMainPosition().y.get_d()
			);
		}
	}
	
	if(m_cam.rdl == SFG::Camera::Low)
	{
		//Universe view (cartesian)
		if(m_cam.rdl_changed)
		{
			//We'll have to reset the correct positioning...
		}
		t->setView(m_cam);
		this->m_universe.draw(*t);
	}
	else if(m_cam.rdl == SFG::Camera::Medium)
	{
		//Planet view (polar)
		//We'll have to convert from cartesian in any case
		//Also, we'll have to know which planet is being focussed
		//(e.g. get the planet that's closest to the cam's center)
		//And we'll also have to keep a lock-on for the camera.
		if(m_cam.rdl_changed)
		{
			//We'll have to reset the correct positioning...
			printf("Planet view engaged\n");
			//Get the closest planet (this is a pretty expensive
			//operation, but we're fine with it for now)
			auto plptr = m_universe.findClosestPlanet(
			PE::Vector2f({m_cam.getView().getCenter().x,
				m_cam.getView().getCenter().y}));
			
		}
		
		const auto& pl = m_next_planets[m_cam_index];
		this->m_cam.setCenter(
			cos(pl->m_planet_surface->getMainPosition().x.get_d()) * pl->m_planet_surface->getMainPosition().y.get_d(),
			sin(pl->m_planet_surface->getMainPosition().x.get_d()) * pl->m_planet_surface->getMainPosition().y.get_d()
		);
			
		t->setView(m_cam);
		//Now, get the appropriate surface
		//plptr->m_planet_surface->draw(*t);
		pl->m_planet_surface->draw(*t);
	}
	else if(m_cam.rdl == SFG::Camera::Detailed)
	{
		//Planet or even higher details (cartesian)
		
		
		if(m_cam.rdl_changed)
		{
			//We'll have to reset the correct positioning...
			printf("Planet view (ultra detail) engaged\n");
		}
	}
	if(m_cam.rdl_changed)
		m_cam.rdl_changed = false;
	
	
	t->setView(m_cam);
	

	if(this->m_build_overlay.isEnabled()) {
		if(m_cam.rdl != SFG::Camera::Low)
			this->m_build_overlay.draw(*t);
	}
	this->UI()->draw(Game::window);
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

