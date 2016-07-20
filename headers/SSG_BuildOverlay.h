#include "stdafx.h"


class SSG_BuildOverlay
{
public:
	SSG_BuildOverlay();
	~SSG_BuildOverlay();
	
	
	int processEvents(std::vector<>& events)
	{
		return 0;
	}
	
	void draw(sf::RenderTarget& target)
	{
		target.draw(m_tex);
	}

	
	
protected:
	sf::RenderTexture m_tex;	//Provides a texture with transparent filling to then draw OVER the current game
};