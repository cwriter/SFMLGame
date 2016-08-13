#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "Window.h"
#include "Network.h"
#include "KeyMapper.h"
#include "GameSettings.h"
#include "GameConsole.h"

//The first gamestate (will load the game)
#define GAMESTATE_LOAD 0
//The second gamestate (will display something (while loading if needed))
#define GAMESTATE_PRE 1
//The third gamestate (Main Menu)
#define GAMESTATE_MENU 2
//The pause gamestate
#define GAMESTATE_PAUSED 3
//Others are userdefined


namespace SFG
{

///<summary>
///Contains the time in ms spent in the game loop
///</summary>
class GameLoopTiming
{
public:


    float processingTime;	//The time spent processing events
    float updatingTime;		//The time spent updating the game Logic
    float drawingTime;		//The time spent drawing
private:

};

};

///<summary>
///The general game
///</summary>
class Game
    : public MsgObject, public SFG::KeyMapper
{
public:
    Game();
    virtual ~Game();

    ///<summary>
    ///Parses .exe start arguments to enable different behavior
    ///</summary>
    ///<param name="argc">The standard argc from int main()</param>
    ///<param name="argv">The standard argv from int main()</param>
    int parseArgs(int argc, char* argv[]);


    ///<summary>
    ///Displays a splashscreen before starting to load. Usable for big projects
    ///</summary>
    virtual int SplashScreenLoad(sf::String path = L"Gamestates/Game.xml");

    ///<summary>
    ///Loads a game (XML/DLL) into memory. Default path is Gamestates/Game.xml
    ///</summary>
    int load(sf::String path = L"Gamestates/Game.xml", SFG::SplashScreen* ss = nullptr);

    ///<summary>
    ///Loads a gamestate with name "GSname"
    ///</summary>
    int loadGamestate(const XMLReader& reader, const sf::String& GSname, const SFG::Pointer<GameState>& included);

    ///<summary>
    ///Adds an existing gamestate to the game.
    ///</summary>
    ///<param name="gsptr">An SFG Pointer pointing to the Gamestate that should be added</param>
    int addGamestate(SFG::Pointer<GameState>& gsptr);

    ///<summary>
    ///Processes all game events.
    ///This is different from update() as it just goes through
    ///every event in the event queue, applying each of its effects,
    ///but does not calculate game logic.
    ///</summary>
    void processEvents();

	///<summary>
	///Draws the game to a target or to the default window of the game
	///</summary>
	///<param name="t">
	///A target to draw on or nullptr if the default window shall be used
	///</param>
    void draw(sf::RenderTarget* t = nullptr);

    ///<summary>
    ///The default game loop (should be used if nothing exceptionally custom should happen)
    ///</summary>
    virtual int gameLoop();

	///<summary>
	///Update part of the game. Calls gamestate's update functions with the correct dt.
	///</summary>
    int update();


    ///<summary>
    ///Returns the current gamestate. There are multiple issues with this function:
    ///The gamestate might change quickly, the vector itself is NOT threadsafe.
    ///Therefore, this function only has effects when run in Debug Mode.
    ///</summary>
    ///<returns>nullptr if the gamestate could not be determined,
    ///a pointer to the current gamestate otherwise</returns>
    SFG::Pointer<GameState> getCurrentGameState() const;

	///<summary>
	///Shows or hides the timing informations
	///</summary>
	inline void setShowTimings(bool show = true)
	{
		m_timing_show = show;
	}
	
    static SFG::Window window;
	
	static SFG::CommandTranslator cmdTranslator;

    sf::Clock elapsedTime;

    GameSettings m_game_settings;
		
private:
    SFG::System m_system;
    SFG::Network m_network;

    SFG::GameLoopTiming m_timing;

	//bool m_timing_show;
	SFG::Var<bool> m_timing_show;
	
	sf::Text m_timing_display;
	sf::Font m_timing_font;
	
	SFG::GameConsole m_game_console;
	
    std::vector<sf::Event> m_unhandled_events;
    std::map<sf::Keyboard::Key, sf::Keyboard::Key> m_keyMap;
    std::vector<SFG::Pointer<GameState>> g_gamestates;
    size_t current_gamestate;
};
