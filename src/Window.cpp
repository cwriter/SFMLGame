#include "stdafx.h"
#include "Window.h"


namespace SFG
{
Window::Window()
{
    this->m_pixelscale = 1.f;
}


Window::~Window()
{
}

int Window::create(int width, int height, const sf::String& windowTitle)
{
    m_window.create(sf::VideoMode(width, height), windowTitle);
    m_window.setActive(false);
    return 0;
}

int SplashScreen::exec(bool* sync)
{
    m_window.create(sf::VideoMode(400, 300), "Splash", sf::Style::None);
    sf::Clock mainClock;


    while (m_window.isOpen())
    {
        m_window.setFramerateLimit(30);
        sf::Event e;
        while (m_window.pollEvent(e))
        {

        }

        int32_t t = mainClock.getElapsedTime().asMilliseconds();
        int32_t targett = 0;
        for (size_t i = 0; i < this->m_ssi.size(); i++)
        {
            if (targett < t && targett + m_ssi[i].m_display_time > t)
            {
                //Display correct thing
                m_window.draw(m_ssi[i].m_tex);
                break;
            }
            targett += m_ssi[i].m_display_time;
        }
        if (sync != nullptr)
        {
            if (*sync)
            {
                //Loading process finished, break
                break;
            }
        }
    }
    return 0;
}

int Window::processEvents(std::vector<sf::Event>& events)
{
    sf::Event mainEvent;

    while (m_window.pollEvent(mainEvent))
    {
        events.push_back(mainEvent);
        if (mainEvent.type == sf::Event::Closed)
        {
            m_window.close();
        }
        else if (mainEvent.type == sf::Event::Resized)
        {
            //m_window.getDefaultView().setSize((float)mainEvent.size.width, (float)mainEvent.size.height);
            m_window.setView(sf::View(sf::FloatRect(0.f, 0.f, (float)mainEvent.size.width, (float)mainEvent.size.height)));
        }
        else if (mainEvent.type == sf::Event::KeyPressed)
        {

        }
        else if (mainEvent.type == sf::Event::KeyReleased)
        {

        }
        else if (mainEvent.type == sf::Event::MouseButtonPressed)
        {

        }
        else if (mainEvent.type == sf::Event::MouseButtonReleased)
        {

        }

    }

    return 0;
}

void Window::startDraw()
{
    drawMutex.lock();
    m_window.setActive(true);
    m_window.clear();
    m_rdt_drawing.restart();
}

void Window::endDraw()
{
    m_window.display();
    m_last_draw_time = m_rdt_drawing.getElapsedTime().asMicroseconds() / 1000.f;
    m_window.setActive(false);
    drawMutex.unlock();
}
};
