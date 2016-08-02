#include "SSG_BuildOverlay.h"
#include "Game.h"


SSG_BuildOverlay::SSG_BuildOverlay()
{
	m_enabled = false;
}

SSG_BuildOverlay::~SSG_BuildOverlay()
{

}

int SSG_BuildOverlay::processEvents(std::vector< sf::Event >& events)
{
	//Process UI first
	m_ui.processEvents(events);
	for(size_t i = 0; i < events.size(); i++)
	{
		if(events[i].type == sf::Event::EventType::Resized)
		{
			printf("SSG_BuildOverlay processEvents called\n");
			if(!m_tex.create(events[i].size.width, events[i].size.height))
				printf("FAILED TO CREATE RENDER TEXUTRE!\n");
			//m_ui.refresh();
			/*m_tex.clear(sf::Color(255,0,0,255));
			m_tex.resetGLStates();
			sf::View view = m_tex.getView();
			float ratio = (float)events[i].size.width / (float)events[i].size.height;
			view.setSize(events[i].size.width * ratio, events[i].size.width);
			m_tex.setView(view);*/
			//events[i].type = sf::Event::EventType::Count;
		}
	}
	
	return 0;
}

void SSG_BuildOverlay::draw(sf::RenderTarget& target)
{
	//#TODO: Draw everything on the texture
	
	
	//Draw to intermediate texture
	//m_tex.create(target.getSize().x, target.getSize().y);
	m_tex.resetGLStates();
	m_tex.clear(sf::Color(0,0,0,0));
	m_tex.setView(m_tex.getDefaultView());
	m_ui.draw(m_tex);
	//finalize
	m_tex.display();
	
	sf::Sprite tmp(m_tex.getTexture());
	sf::View tmpv = target.getView();
	
	target.setView(sf::View(sf::FloatRect(0,0, m_tex.getSize().x, m_tex.getSize().y)));
	target.draw(tmp);
	target.setView(tmpv);
	/*SFG::Util::printLog(SFG::Util::Information, __FILE__, __LINE__,
		"Drawing SSG_BuildOverlay with %d windows", m_ui.getWindowCount()
	);*/
	//m_ui.draw(target);
}