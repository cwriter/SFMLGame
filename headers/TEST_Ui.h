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
		this->UI()->load("Data/example - uimanager.xml", StringManager(L"Data/example - strings.xml"));
		
		
		return 0;
	}
	
private:	
};