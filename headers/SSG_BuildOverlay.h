#include "stdafx.h"
#include "UI.h"
#include "GameConsole.h"

///<summary>
///Provides the menu to build a new ship design or modify an old one
///</summary>
class SSG_BuildOverlay
{
public:
	SSG_BuildOverlay();
	~SSG_BuildOverlay();
	
	
	int processEvents(std::vector<sf::Event>& events);
	
	int load(const sf::String& path)
	{
		int ret = this->m_ui.load(path, StringManager(path));
		if(ret != 0)
		{
			SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
				"Failed to load SSG_BuildOverlay (Code %d)", ret);
			return ret;
		}
		
		return ret;
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
	
protected:
	SFG::Var<bool> m_enabled;
	
	sf::RenderTexture m_tex;	//Provides a texture with transparent filling to then draw OVER the current game
	
	SFG::UIManager m_ui;
};