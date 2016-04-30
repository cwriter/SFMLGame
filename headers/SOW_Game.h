#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "SOW_World.h"

class SOW_Game :
	public GameState
{
public:
	SOW_Game();
	virtual ~SOW_Game();


	///<summary>
	///Update your logic.
	///<param name="dt>
	///Time that has elapsed since the last update in milliseconds with microsecond resolution (expected values are between 1 and 100)
	///</param>
	///</summary>
	virtual int update(double dt)
	{
		return 0;
	}

	///<summary>
	///New list of inputs to process. The Inputlag of this operation is the render time
	///</summary>
	virtual int processEvents(SFG::Window& window, std::vector<sf::Event>& events)
	{

		return 0;
	}

	///<summary>
	///Draw the current scene
	///</summary>
	virtual void draw(sf::RenderTarget* t)
	{
		//Draw items
		SFG::FloatRect rect(SFG::Vector2f(t->getView().getCenter()) - SFG::Vector2f(t->getView().getSize()) / 2.f, t->getView().getSize());
		m_world.draw(*t, rect);

		//#TODO: Draw UI etc...
	}

	///<summary>
	///Loads the references to the modules. This does/should <i>not</i><br/>
	///load all data, but will load it on demand. Use <c>preload()</c> to load everything instantly.
	///</summary>
	virtual int load(const sf::String& path)
	{
		//Load the world
		m_world.load("saves/latest.xml"); //#TODO: Has to be adjusted

		//Load all image / animation files from path (#TODO)
		STILL_TO_DO(__FILE__, __LINE__);

		return 0;
	}

	///<summary>
	///Action to be executed when the player requires a pause or wants to continue
	///</summary>
	///<returns>True if the game should switch to the default pause gamestate, false if you want to handle it by yourself (or ignore pauses)
	virtual bool pause(bool p)
	{
		//The code here are just recommendations.
		//For example, if you want to be able to load data during pause, you should not stop the system
		if (p)
		{
			this->m_system.halt_thread(); //Care: Can block (Estimated time < 60 ms)
		}

		if (!p)
		{
			this->m_system.continue_thread(); //Will resume the system task execution
		}
		return false;
	}

	///<summary>
	///Preloads the content needed for this specific gamestate
	///</summary>
	virtual int preload()
	{
		return 0;
	}

	///<summary>
	///Unloads all data from this gamestate while begin able to instantly reload (=keeping filepaths for example)
	///</summary>
	virtual int unload()
	{
		for (auto m : this->m_modules)
		{
			m->unload();
		}
		return 0;
	}

	///<summary>
	///Loads a list of assets
	///</summary>
	///<param name="reader">The reader providing the data</param>
	virtual int loadAssets(const XMLReader& reader)
	{
		auto list = reader.getXMLGroupHandle(L"");
		if (list == nullptr) return -1;
		for (auto a : list->lowerElements)
		{

			if (a.name == L"module")
			{
				//get the name
				sf::String name;
				for (auto attr : a.attributes)
				{
					if (attr.str1 == L"name")
					{
						name = attr.str2;
						break;
					}
				}
				//Get the corresponding module
				for (auto m : m_modules)
				{
					if (m->getType() == name)
					{
						//Load into this module
						m->load(XMLReader(a));
					}
				}
			}
		}
		return 0;
	}
private:

	SOW_World m_world;	//The actual world
};

