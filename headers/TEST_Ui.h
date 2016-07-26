#include "stdafx.h"
#include "GameState.h"
#include "UI.h"


class TEST_UI : public GameState
{
public:
	TEST_UI()
	{
		this->name = "main_gamestate";
	}
	
    virtual int processEvents(SFG::Window& window, std::vector<sf::Event>& events) override
    {
		this->UI()->processEvents(events);
		return 0;
	}
	
    virtual int update(double dt) override
    {
		this->UI()->update(dt);
		return 0;
	}
	
    virtual void draw(sf::RenderTarget* t) override
    {
	
		this->UI()->draw(*t);
	}
	
    virtual int load(const sf::String& path) override
    {
		SFG::UIWindow* win = new SFG::UIWindow();
		if (win == NULL) {
			perror("ALLOCATION ERROR");
			exit(-1);
		}
		sf::String data;
		basicLoadFile(L"Data/example - window.xml", data);
		XMLReader winRead;
		winRead.setSource(data);
		winRead.parse();
		
		win->load(XMLReader(*winRead.getXMLGroupHandle("xml/module/")), sf::String(L"Test-Window"), StringManager(L"Data/example - strings.xml"));
		UI()->addWindow(win);
		
		this->UI()->load("Data/example - uimanager.xml", StringManager(L"Data/example - strings.xml"));
		
		
		return 0;
	}
	
private:	
};