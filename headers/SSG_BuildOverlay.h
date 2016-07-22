#include "stdafx.h"

///<summary>
///Provides the menu to build a new ship or modify an old one
///</summary>
class SSG_BuildOverlay
{
public:
	SSG_BuildOverlay();
	~SSG_BuildOverlay();
	
	
	int processEvents(std::vector<sf::Event>& events)
	{
		return 0;
	}
	
	void draw(sf::RenderTarget& target)
	{
		sf::Sprite tmp(m_tex.getTexture());
		target.draw(tmp);
	}

	
	
protected:
	sf::RenderTexture m_tex;	//Provides a texture with transparent filling to then draw OVER the current game
};