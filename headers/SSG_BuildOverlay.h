#include "stdafx.h"
#include "UI.h"
#include "GameConsole.h"
#include "SFGUI/Window.hpp"
#include "SFGUI/Widgets.hpp"
#include "SFGUI/RadioButtonGroup.hpp"
#include "Interactable.h"

///<summary>
///Provides the menu to build a new ship design or modify an old one
///</summary>
class SSG_BuildOverlay
	: public SFG::Interactable
{
public:
	SSG_BuildOverlay();
	~SSG_BuildOverlay();
	
	
	int processEvents(std::vector<sf::Event>& events);
	
	int load(const sf::String& path)
	{
		/*int ret = this->m_ui.load(path, StringManager(path));
		if(ret != 0)
		{
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
				"Failed to load SSG_BuildOverlay (Code %d)", ret);
			return ret;
		}
		return ret;*/
		
		auto button = sfg::Button::Create("Physicalize");
		
		auto rbGroup = sfg::RadioButtonGroup::Create();
		auto mat1 = sfg::RadioButton::Create("Steel");
		auto mat2 = sfg::RadioButton::Create("Composite");
		
		mat1->SetGroup(rbGroup);
		mat2->SetGroup(rbGroup);
		
		auto window = sfg::Window::Create(sfg::Window::Style::BACKGROUND | sfg::Window::Style::TITLEBAR);
		window->GetSignal(window->OnCloseButton).Connect([&](){
			window->Show(false);
			m_ui.desktop().Remove(window);
		});
		
		auto grid = sfg::Box::Create(sfg::Box::Orientation::VERTICAL);
		grid->Pack(button);
		grid->Pack(mat1, false, false);
		grid->Pack(mat2, false, false);
		window->SetTitle( "Material" );
		
		window->Add(grid);
		
		m_ui.desktop().Add(window);
		
		return 0;
	}
	
	int update(float dt)
	{
		if(!isEnabled()) return -1;
		m_ui.update(dt);
		return 0;
	}
	
	bool isEnabled() const {
		return m_enabled;
	}
	
	int enable(sf::RenderTarget& target)
	{
		if(!m_tex.create(target.getSize().x, target.getSize().y))
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to create render texture");
		
		
		
		m_enabled = true;
		return 0;
	}
	
	int disable()
	{
		
		
		m_enabled = false;
		return 0;
	}
	
	void draw(sf::RenderTarget& target);

	void setTarget(SFG::Window& t)
	{
		this->m_ui.setTarget(&t.getSFMLWindow());
	}
	
	SFG::Var<bool> m_enabled;
	
protected:
	
	sf::RenderTexture m_tex;	//Provides a texture with transparent filling to then draw OVER the current game
	
	SFG::UIManager m_ui;
};