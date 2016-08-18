#include "stdafx.h"
#include "SSG_Planet.h"
#include <SSG_SolarSystem.h>
#include <functional>
#include <SFGUI/Widgets.hpp>
#include <sstream>


SSG_Planet::SSG_Planet()
{
    //We are a planet, so we should probably be all round and stuff
    this->m_circle.setFillColor(sf::Color(128, 64, 0));
    this->m_circle.setPosition(0.f, 0.f);
	m_x = 0.;
	m_y = 0.;

    //Set the default radius
    this->m_circle.setRadius(SSG_PLANET_DEFAULT_RADIUS);
    this->setMass(PE::Mass(SSG_PLANET_DEFAULT_MASS));

	//bind the functions
	setClick(std::bind(&SSG_Planet::internal_onclick, this, std::placeholders::_1, std::placeholders::_2));
	
	m_info_show = false;
}


SSG_Planet::~SSG_Planet()
{
}



int SSG_Planet::load(const XMLReader& reader)
{
	//SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, "Still to do");
	//Load values accordingly	
	this->m_name = reader.getValue("name.");
	bool real = false;
	this->setMass(PE::Mass(reader.asDouble("mass/", real)));
	if(!real)
	{
		//Failed to load mass
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load Mass");
	}
	sf::FloatRect r = reader.asFloatRect("PosAndVel/", real);
	if(!real)
	{
		//Failed to load pos and Velocity
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to load PosAndVel");
	}
	//this->setPosition(r.left, r.top);
	this->setPosition(mpf_class(r.left), mpf_class(r.top));
	this->setVelocity(PE::Velocity(r.width, r.height));
	
	auto radius = reader.asDouble("radius/", real);
	if(!real)
	{
		SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
							"Failed to get radius for planet %s", this->m_name.toAnsiString().c_str());
	}
	this->getShape().setRadius(radius);
	
	SFG::FloatRect rect(this->getShape().getLocalBounds());
	this->getShape().setOrigin(rect.center());
	
	m_planet_surface.reset(new SSG_PlanetSurface(radius));
	m_planet_surface->load();
	
	//Get Moons
	reader.for_all("Moon", [=](const XMLGroup* g){
		SFG::Pointer<SSG_Planet> ptr(new SSG_Planet());
		ptr->m_parentSys = this->m_parentSys;
		ptr->setGuiDesktop(this->desktop());
		int ret = ptr->load(XMLReader(*g));
		if(ret != 0)
		{
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
								"Failed to load moon");
			return;
		}
		printf("Position: %f + %f | %f + %f\n", getShape().getPosition().x , ptr->getShape().getPosition().x,
			    getShape().getPosition().y, ptr->getShape().getPosition().y);
		//Correct relative values
		//ptr->setPosition(getShape().getPosition().x + ptr->getShape().getPosition().x, 
		//				 getShape().getPosition().y + ptr->getShape().getPosition().y);
		ptr->setPosition(x() + ptr->x(), 
						 y() + ptr->y());
		ptr->setVelocity(getVelocity() + ptr->getVelocity());
		
		
		//Add to system
		this->m_parentSys->addSpecificToSystem(ptr);
		
		SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,	"Moon \"%s\" has been added", ptr->getName().toAnsiString().c_str());
		
	});
	
	//TESTING
	this->getShape().setFillColor(sf::Color(255, 255, 255));
	//!TESTING
	return 0;
}

void SSG_Planet::draw(sf::RenderTarget& t)
{
    //We have to override the default as we're not drawing a sprite, but a shape
    t.draw(this->m_circle);
	drawPhysicsComponents(&t, 1000.f);
	draw_info(t);
}

void SSG_Planet::drawPhysicsComponents(sf::RenderTarget* t, float vecscale) const
{
	sf::VertexArray va(sf::Lines);
	
	va.append(sf::Vertex(sf::Vector2f(x().get_d(), y().get_d()), sf::Color::White));
	va.append(sf::Vertex(
		sf::Vector2f(x().get_d() + getVelocity().getVector().x.get_d() * vecscale * 5.f, 
					 y().get_d() + getVelocity().getVector().y.get_d() * vecscale * 5.f)
		, sf::Color::White));
	
	t->draw(va);
	
	va.clear();
	
	
	va.append(sf::Vertex(sf::Vector2f(x().get_d(), y().get_d()), sf::Color::Red));
	va.append(sf::Vertex(
		sf::Vector2f(x().get_d() + getAcceleration().getVector().x.get_d() * vecscale * 100000.f, 
					 y().get_d() + getAcceleration().getVector().y.get_d() * vecscale * 100000.f)
		, sf::Color::Red));
	
	t->draw(va);
}

int SSG_Planet::update(float dt)
{
    double time = dt / 1000.f;
    this->finishPhysicsCycle(dt);
    assert(!std::isnan(x().get_d()));
    assert(!std::isnan(y().get_d()));
	
	//printf("Velocity is %f|%f\n", this->getVelocity().getVector().x.get_d(), this->getVelocity().getVector().y.get_d());

	mpf_class ex1 = this->x().get_d() + this->getVelocity().getVector().x.get_d() * time;
	mpf_class ex2 = this->y().get_d() + this->getVelocity().getVector().y.get_d() * time;
	
    float test1 = float(ex1.get_d());
    float test2 = float(ex2.get_d());

    //We can lose precision, so if the precision is too small, just ignore everything
    if (!(std::isnan(test1) || std::isinf(test1)) && !(std::isnan(test2) || std::isinf(test2)))
    {
        this->setPosition(ex1, ex2);
        assert(!std::isnan(x().get_d()));
        assert(!std::isnan(y().get_d()));
    }
    //std::cout << "Vel (" << this->getVelocity().getVector().x << "|" << this->getVelocity().getVector().y << ")" << std::endl;
    //std::cout << "Loc (" << this->x() << "|" << this->y() << ")" << std::endl;

    update_info(dt);
	
    return 0;
}

void SSG_Planet::setPosition(float x, float y)
{
	SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__, 
			   "Function %s with low precision was called. Please call the function with\n"
			   "the higher precision instead", __FUNCTION__);
	std::string str;
	SFG::Util::getStackTrace(str);
	printf(str.c_str());
	m_x = x; 
	m_y = y;
   this->m_circle.setPosition(x, y);
   GObjectBase::setPosition(x, y);
}

int SSG_Planet::internal_onclick(const sf::Vector2f& mpos, const sf::Mouse::Button& but)
{
	if(m_info_show) return 0;
	
	m_info_window = sfg::Window::Create(
		sfg::Window::Style::TOPLEVEL | sfg::Window::Style::CLOSE);
	m_info_window->GetSignal(sfg::Window::OnCloseButton).Connect([this](){
		m_info_window->Show(false);
		this->m_info_show = false;
		desktop()->Remove(m_info_window);
	});
	m_info_window->SetTitle("Properties of planet \"" + m_name + "\"");
	
	auto table = sfg::Table::Create();
	
	
	auto masslabel = sfg::Label::Create("Mass:");
	
	std::ostringstream strm;
	strm << std::scientific << this->getMass().getScalar().get_d();
	
	m_info_masstext = sfg::Label::Create(strm.str() + "kg");
	m_info_masstext->SetAlignment(sf::Vector2f(1.f,0.f));
	
	auto poslabel = sfg::Label::Create("Position:");
	
	strm.str(std::string());
	
	strm << std::scientific << "(" << this->x().get_d() << "|" << this->y().get_d() << ")m";
	m_info_postext = sfg::Label::Create(strm.str());
	m_info_postext->SetAlignment(sf::Vector2f(1.f,0.f));
	
	
	table->Attach(masslabel, sf::Rect<sf::Uint32>(0,0,1,1), sfg::Table::EXPAND, sfg::Table::EXPAND);
	table->Attach(m_info_masstext, sf::Rect<sf::Uint32>(1,0,1,1), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND);
	
	table->Attach(poslabel, sf::Rect<sf::Uint32>(0,1,1,1), sfg::Table::EXPAND, sfg::Table::EXPAND);
	table->Attach(m_info_postext, sf::Rect<sf::Uint32>(1,1,1,1), sfg::Table::EXPAND | sfg::Table::FILL, sfg::Table::EXPAND);
	
	m_info_window->Add(table);
	desktop()->Add(m_info_window);
	
	
	m_info_show = true;
	
	return 0;
}

int SSG_Planet::update_info(float dt)
{
	if(!m_info_show)
		return 0;
	std::ostringstream strm;
	strm << std::scientific << this->getMass().getScalar().get_d() << "kg";
	m_info_masstext->SetText(strm.str());
	
	strm.str(std::string());
	
	strm << std::scientific << "(" << this->x().get_d() << "|" << this->y().get_d() << ")m";
	m_info_postext->SetText(strm.str());
	
	return 0;
}

void SSG_Planet::draw_info(sf::RenderTarget& t)
{
	if(!m_info_show) return;
	this->m_info_window->GetAllocation();
	
	sf::VertexArray va(sf::Lines);
	
	va.append(sf::Vertex(sf::Vector2f(getLogicBoundingRect().left, getLogicBoundingRect().top), sf::Color::White));
	
	va.append(sf::Vertex(t.mapPixelToCoords(
		sf::Vector2i(m_info_window->GetAllocation().left, m_info_window->GetAllocation().top), t.getView()
	), sf::Color::White));
	
	t.draw(va);
	
	va.clear();
	
	
	
	va.append(sf::Vertex(sf::Vector2f(getLogicBoundingRect().left + getLogicBoundingRect().width, getLogicBoundingRect().top), sf::Color::White));
	
	va.append(sf::Vertex(t.mapPixelToCoords(
		sf::Vector2i(m_info_window->GetAllocation().left + m_info_window->GetAllocation().width, m_info_window->GetAllocation().top), t.getView()
	), sf::Color::White));
	
	t.draw(va);
	
	va.clear();
	
	
	
	va.append(sf::Vertex(sf::Vector2f(getLogicBoundingRect().left, getLogicBoundingRect().top + getLogicBoundingRect().height), sf::Color::White));
	
	va.append(sf::Vertex(t.mapPixelToCoords(
		sf::Vector2i(m_info_window->GetAllocation().left, m_info_window->GetAllocation().top + m_info_window->GetAllocation().height), t.getView()
	), sf::Color::White));
	
	t.draw(va);
	
	va.clear();
	
	
	
	va.append(sf::Vertex(sf::Vector2f(getLogicBoundingRect().left + getLogicBoundingRect().width, getLogicBoundingRect().top + getLogicBoundingRect().height), sf::Color::White));
	
	va.append(sf::Vertex(t.mapPixelToCoords(
		sf::Vector2i(m_info_window->GetAllocation().left + m_info_window->GetAllocation().width, m_info_window->GetAllocation().top+ m_info_window->GetAllocation().height), t.getView()
	), sf::Color::White));
	
	t.draw(va);
	
	va.clear();
}

