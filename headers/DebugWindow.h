#pragma once
#include "stdafx.h"
#include "Game.h"

namespace SFG
{

///<summary>
///Class providing debug information of everything that happens within the Game Engine
///</summary>
class DebugWindow
{
public:
    DebugWindow();
    ~DebugWindow();

    void create(Game* toMonitor)
    {
        this->m_game = toMonitor;
        assert(toMonitor != nullptr);



        m_running = true;
        //std::async(std::launch::async, &DebugWindow::run, this);
        std::thread { &DebugWindow::run, this } .detach();
    }

    void close()
    {
        while (m_running)
        {
            printf("[Info] DebugWindow waiting for destruction...\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(300));
        }
    }

    int run();
private:
    std::atomic_bool m_running;
    float m_updating_speed;
    Game* m_game;
    sf::RenderWindow m_window;
    sf::Text m_display_text;

};

};
