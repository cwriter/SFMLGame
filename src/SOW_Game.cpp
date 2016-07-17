#include "stdafx.h"
#include "SOW_Game.h"
#include <SOW_Car.h>



SOW_Game::SOW_Game()
{
    this->name = "main_gamestate";
	Module_SOW_Car md;
}


SOW_Game::~SOW_Game()
{
}

int SOW_Game::processEvents(SFG::Window& window, std::vector<sf::Event>& events)
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
        else if(events[i].type == sf::Event::EventType::KeyPressed)
		{
			if(events[i].key.code == 'W')
			{
				testcar.accel();
			}
			else if(events[i].key.code == 'S')
			{
				
			}
			else if(events[i].key.code == 'A')
			{
				
			}
			else if(events[i].key.code == 'D')
			{
				
			}
			
		}
		else if(events[i].type == sf::Event::EventType::KeyReleased)
		{
			if(events[i].key.code == 'W')
			{
				testcar.stopAccel();
			}
			else if(events[i].key.code == 'S')
			{
				
			}
			else if(events[i].key.code == 'A')
			{
				
			}
			else if(events[i].key.code == 'D')
			{
				
			}
			
		}
    }
    window.getSFMLWindow().setView(m_cam);
    return 0;
}