#include "stdafx.h"
#include "UI.h"


///<summary>
///Provides the menu to build a new ship design or modify an old one
///</summary>
class SSG_BuildOverlay
{
public:
	SSG_BuildOverlay();
	~SSG_BuildOverlay();
	
	
	int processEvents(std::vector<sf::Event>& events)
	{
		//Process UI first
		m_ui.processEvents(events);
		
		
		return 0;
	}
	
	int load(const sf::String& path)
	{
		
		return 0;
	}
	
	int update(float dt)
	{
		m_ui.update(dt);
		return 0;
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

	
	
protected:
	bool m_enabled;
	
	sf::RenderTexture m_tex;	//Provides a texture with transparent filling to then draw OVER the current game
	
	SFG::UIManager m_ui;
};