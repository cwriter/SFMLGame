#pragma once
#include "stdafx.h"
#include "XML.h"
#include "StringManager.h"
#include "Window.h"
#include "Pointer.h"
#include <functional>
#ifdef SFG_USE_CUSTOM_UI
#include "custom_UI.h"
#else
#include <SFGUI/SFGUI.hpp>
#include <SFGUI/Desktop.hpp>
#include <SFGUI/Button.hpp>
#include <SFGUI/Window.hpp>
#endif

namespace SFG
{

#ifndef SFG_USE_CUSTOM_UI

class UIManager
{
public:
	void update(float dt)
	{
		this->m_sfg_desktop.Update(dt);
	}
	
	void draw(sf::RenderTarget& target)
	{
		//WARNING: Dangerous cast
		SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__, "Dangerous cast");
		//sfgui.Display(*(sf::RenderTexture*)(&target));
	}
	
	void draw(sf::RenderTexture& tex);
	
    void draw(SFG::Window& target)
	{
		target.getSFMLWindow().resetGLStates();
		sfgui.Display(target.getSFMLWindow());
	}
	
	int processEvents(std::vector<sf::Event>& events);

	
	int load(const sf::String& path, const StringManager& strman)
	{
		auto button = sfg::Button::Create( "Hello" );
		auto window = sfg::Window::Create();
		window->SetTitle( "Hello World example" );
		window->Add( button );
		m_sfg_desktop.Add(window);
		m_windows.push_back(window);
		
		return -1;
	}
	
	void setTarget(sf::RenderWindow* t){
		this->m_target = t;
	}
	void refresh(){
		this->m_sfg_desktop.Refresh();
	}
	
	inline sfg::Desktop& desktop() {
		return m_sfg_desktop;
	}
	
#ifndef SFG_USE_CUSTOM_UI
	static sfg::SFGUI sfgui;
#endif
	
private:
	std::vector<std::shared_ptr<sfg::Window>> m_windows;
	sfg::Desktop m_sfg_desktop;
	sf::RenderWindow* m_target;
};

#else
class UIManager
{
public:
    UIManager() {
        m_target = nullptr;
		m_manager_scale = 1.f;
		
		m_context_window.setWindowFlags(
			UIWindow::WindowFlags::NoTitlebar | 
			UIWindow::WindowFlags::NoResize
		);
		
    }
    ~UIManager() {
        for (auto p : this->m_tmp_textures)
        {
            if (p != nullptr) {
                delete p;
            }
        }
        for (auto w : this->m_windows) {
            if (w != nullptr) {
                delete w;
            }
        }
    }

    void setScale(float s){
		m_manager_scale = s;
	}
	
	float getScale() const {
		return m_manager_scale;
	}
    
    void setTarget(sf::RenderWindow* w) {
        m_target = w;
    }

    sf::RenderWindow* getTarget() {
        return m_target;
    }

    ///<summary>
    ///Loads up all windows / things that should be open already
    ///</summary>
    ///<param name="path">The path to the file containing the UI Manager data</param>
    int load(const sf::String& path, const StringManager& strman);
	
	
    size_t selectElementByMousePos(sf::Vector2f* out);

    int processEvents(std::vector<sf::Event>& events);

    void draw(sf::RenderTarget& target);
    void draw(SFG::Window& target);

    void update(float dt) {
		assert(m_target != nullptr);
        for (auto w : m_windows) {
            sf::Vector2f mpos;
            mpos = m_target->mapPixelToCoords(sf::Mouse::getPosition(*m_target), m_target->getDefaultView());
			//mpos = sf::Vector2f(sf::Mouse::getPosition(*m_target).x, sf::Mouse::getPosition(*m_target).y);
            w->update(mpos, dt);
        }
    }

    inline void addWindow(UIWindow* w) {
        this->m_windows.push_back(w);
        this->m_tmp_textures.push_back(nullptr);
    }

    sf::FloatRect m_positions;
	
	size_t getWindowCount() const {
		return m_windows.size();
	}

private:

    void drawWindowToTexture(sf::RenderTarget& target, size_t i, float scale);
	
	void priv_draw(sf::RenderTarget& target, float scale);
	
	float m_manager_scale;
    sf::RenderWindow* m_target;
	
	//The global context menu
	SFG::UIWindow m_context_window;
	
    std::vector<UIWindow*> m_windows;
    std::vector<sf::RenderTexture*> m_tmp_textures;
};
#endif
};
