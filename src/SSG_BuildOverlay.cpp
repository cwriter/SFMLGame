#include "SSG_BuildOverlay.h"


SSG_BuildOverlay::SSG_BuildOverlay()
{
	m_enabled = false;
}

SSG_BuildOverlay::~SSG_BuildOverlay()
{

}

void SSG_BuildOverlay::draw(sf::RenderTarget& target)
{
	//#TODO: Draw everything on the texture
	
	
	//Draw to intermediate texture
	m_ui.draw(m_tex);
	//finalize
	m_tex.display();
	
	sf::Sprite tmp(m_tex.getTexture());
	target.draw(tmp);
	/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
		"Drawing SSG_BuildOverlay with %d windows", m_ui.getWindowCount()
	);*/
	m_ui.draw(target);
}