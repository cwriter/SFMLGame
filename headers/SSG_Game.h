#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "Camera.h"
#include "SSG_Universe.h"
#include "SSG_BuildOverlay.h"
#include "UI.h"

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
    int unload() override;

    ///<summary>
    ///Inits the gamestate with the window. This function gets called whenever the game switches to this gamestate.
    ///</summary>
    ///<param name="win">The window of the Game</param>
    int init(SFG::Window& win) override;

private:
	
	enum MouseMode
	{
		Select = 0
	};
	
	MouseMode m_mouse_mode;
	
	SSG_BuildOverlay m_build_overlay;
	
    PE::PhysicsEngine m_physicsEngine;		//The physics engine used
    SFG::Camera m_cam;						//The standard game camera
    SFG::Pointer<PE::PhysicObject> m_lock_on;	//Center of the Camera
    std::vector<SFG::Pointer<SSG_Planet>> m_next_planets;	//TESTING
    size_t m_cam_counter;
	size_t m_cam_index;

    //The universe the game shall handle
    SSG_Universe m_universe;

};

