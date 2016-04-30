#include "stdafx.h"
#include "DebugWindow.h"

namespace SFG
{

	DebugWindow::DebugWindow()
	{
		m_updating_speed = 300.f;
	}


	DebugWindow::~DebugWindow()
	{
		close();
	}

	int DebugWindow::run()
	{
		//Create window here to minimize threading issues
		m_window.create(sf::VideoMode(800, 600), "Debug Information");
		m_window.setActive(false);
		m_window.setFramerateLimit(10);
		sf::Font fnt;
		if (!fnt.loadFromFile("Fonts/arial.ttf"))
		{
			printf("[Error] Failed to load font in %s:%d\n", __FILE__, __LINE__);
		}
		m_display_text.setFont(fnt);
		m_display_text.setCharacterSize(12);



		//Keep updating at a given speed
		sf::Clock sf_clck;
		while (m_running && m_window.isOpen() && m_game->window.getSFMLWindow().isOpen())
		{
			//printf("Updating after %f ms\n", sf_clck.getElapsedTime().asMicroseconds() / 1000.f);
			sf::String display_string;

			//Reset clock
			sf_clck.restart();

			sf::Event sev;
			while (m_window.pollEvent(sev))
			{
				//Handle events
				if (sev.type == sf::Event::Closed) {
					m_window.close();
					m_running = false;
					return 1;
				}
			}


			//Iterate through all modules in the current gamestate
			auto g = m_game->getCurrentGameState();
			if (g.getElement() != nullptr)
			{
				auto view = m_game->window.getSFMLWindow().getView();
				display_string += "Viewport: (" + std::to_string(view.getViewport().left) + "|"
					+ std::to_string(view.getViewport().top) + ":"
					+ std::to_string(view.getViewport().width) + "|"
					+ std::to_string(view.getViewport().height) + ")\n";
				display_string += "Current Gamestate: \"" + g->getName() + "\":\n";
				auto modules = g->getModules();
				if (modules != nullptr)
				{
					for (auto m : *modules)
					{
						display_string += "In Module: ";
						if (m->getType() == "moduleG2D")
						{
							display_string += "G2D\n";
							//Get the Graphics Information
							for (auto o : *m->getData())
							{
								//Analyze each object
								auto go = o.cast<GObjectBase>();
								auto name = go->m_name;

								display_string += "\tIn object named \"" + sf::String(name) + "\":\n";
								//display_string += "\t\tPosition/Size: \t(%f|%f:%f|%f)\n";
								display_string += "\t\tPosition/Size: \t(" + std::to_string(go->getBoundingRect().left) + "|" + std::to_string(go->getBoundingRect().top) + ":" +
									std::to_string(go->getBoundingRect().width) + "|" + std::to_string(go->getBoundingRect().height) + ")\n";
								display_string += "\t\tSize: \t(" + std::to_string(go->getSprite().getLocalBounds().width)
									+ "|" + std::to_string(go->getSprite().getLocalBounds().height) + ")\n";
								display_string += "\t\tAnimation:\n\t\t\tIndex: " + std::to_string(go->getCurrentAnimationIndex()) + "\n";


								if (go->getAnimationVector() != nullptr &&
									go->getAnimationVector()->size() > go->getCurrentAnimationIndex())
								{

									auto anim = (*go->getAnimationVector())[go->getCurrentAnimationIndex()];
									display_string += "\t\t\tAnim.-Time:\t" + std::to_string(anim->m_anim_state) +
										"/" + std::to_string(anim->m_totaltime) +
										"\n\t\t\tFrame:\t" + std::to_string(anim->m_current_frame) + "\n";

								}
							}

						}
					}
				}
			}
			else
            {
                display_string += "Error: Current gamestate could not be determined.\n";
            }
			this->m_display_text.setString(display_string);

            //printf("DbgWnd-String===========================\n%s", display_string.toAnsiString().c_str());

			m_game->window.drawMutex.lock();
			if (m_window.setActive(true))
			{
				m_window.clear();
				m_window.draw(m_display_text);
				m_window.display();
			}
			if (!m_window.setActive(false))
			{
				printf("[Error] Failed to deactivate context in %s:%d\n", __FILE__, __LINE__);
			}
			m_game->window.drawMutex.unlock();

			//Wait (or yield) until the proposed time has been reached (if not already)
			while ((sf_clck.getElapsedTime().asMicroseconds() / 1000.) < m_updating_speed)
			{
				//std::this_thread::yield();
				std::this_thread::sleep_for(std::chrono::milliseconds(abs(int((sf_clck.getElapsedTime().asMicroseconds() / 1000.) - m_updating_speed))));
			}
		}
		m_running = false;
		return 0;
	}

};
