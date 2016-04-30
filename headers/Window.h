#pragma once
#include "stdafx.h"
#include "UI.h"
#include "Message.h"

namespace SFG
{

///<summary>
///Displays a Splashscreen
///</summary>
class SplashScreenItem
{
public:
    sf::Sprite m_tex;
    int32_t m_display_time;
};

class SplashScreen : public MsgObject
{
public:
    enum SplashScreenMessages
    {
        LoadingDone = 0
    };
    SplashScreen()
    {

    }

    void close()
    {
        m_window.close();
    }

    int notify(int msg)
    {
        if (msg == SplashScreenMessages::LoadingDone)
        {
            this->close();
        }
        return 0;
    }

    int exec(bool* sync = nullptr);

    void addSplashScreenItem(const SplashScreenItem& ssi)
    {
        this->m_ssi.push_back(ssi);
    }

private:
    sf::Time m_last_time;
    sf::RenderWindow m_window;
    std::vector<SplashScreenItem> m_ssi;
};


class Window
{
public:
    Window();
    ~Window();

    int create(int width, int height, const sf::String& windowTitle);

    int processEvents(std::vector<sf::Event>& events);

    inline float getElapsedTime()
    {
        return this->m_rdt_drawing.getElapsedTime().asMicroseconds() / 1000.f;
    }

    void startDraw();

    void endDraw();

    inline float scale()
    {
        return m_pixelscale;
    }
    inline void setScale(float s)
    {
        m_pixelscale = s;
    }

    inline void draw(const sf::Drawable& drawable, const sf::RenderStates& rs = sf::RenderStates::Default)
    {
        m_window.draw(drawable, rs);
    }

    inline operator sf::RenderTarget&()
    {
        return this->m_window;
    }

    inline operator sf::RenderTarget*()
    {
        return &this->m_window;
    }

    inline operator sf::RenderWindow*()
    {
        return &this->m_window;
    }

    inline operator sf::RenderWindow&()
    {
        return this->m_window;
    }

    inline sf::RenderWindow& getSFMLWindow()
    {
        return this->m_window;
    }

    std::mutex drawMutex;

private:
    sf::RenderWindow m_window;
    float m_pixelscale;
    sf::Clock m_rdt_drawing;
    float m_last_draw_time;
};

};
