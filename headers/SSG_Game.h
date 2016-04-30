#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "Camera.h"
#include "SSG_Universe.h"

class SSG_Game :
	public GameState
{
public:
	SSG_Game();
	virtual ~SSG_Game();

	///<summary>
	///Updates the game logic
	///<param name="dt>
	///Time that has elapsed since the last update in milliseconds with microsecond resolution (expected values are between 1 and 100)
	///</param>
	///</summary>
	int update(double dt) override;

	///<summary>
	///New list of inputs to process. The Inputlag of this operation is the render time
	///</summary>
	int processEvents(SFG::Window& window, std::vector<sf::Event>& events) override;

	///<summary>
	///Draw the current scene
	///</summary>
	void draw(sf::RenderTarget* t) override;

	///<summary>
	///Loads the references to the modules. This does/should not
	///load all data, but will load it on demand. Use preload() to load everything instantly.
	///</summary>
	int load(const sf::String& path) override;

	///<summary>
	///Action to be executed when the player requires a pause or wants to continue
	///</summary>
	///<returns>True if the game should switch to the default pause gamestate, false if you want to handle it by yourself (or ignore pauses)
	bool pause(bool p) override;

	///<summary>
	///Preloads the content needed for this specific gamestate
	///</summary>
	int preload() override {
		return 0;
	}

	///<summary>
	///Unloads all data from this gamestate while begin able to instantly reload (=keeping filepaths for example)
	///</summary>
	int unload() override {
		for (auto m : this->m_modules)
		{
			m->unload();
		}
		return 0;
	}

	///<summary>
	///Inits the gamestate with the window. This function gets called whenever the game switches to this gamestate.
	///</summary>
	///<param name="win">The window of the Game</param>
	int init(SFG::Window& win) override
	{
		this->m_cam.setSize(float(win.getSFMLWindow().getSize().x), float(win.getSFMLWindow().getSize().y));
		return 0;
	}

private:
	PE::PhysicsEngine m_physicsEngine;		//The physics engine used
	SFG::Camera m_cam;						//The standard game camera
	

	//Testing/Loading
	SFG::Pointer<SSG_Planet> sun;
	SFG::Pointer<SSG_Planet> planet;
	SFG::Pointer<SSG_Planet> moon;
};

