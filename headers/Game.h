#pragma once
#include "stdafx.h"
#include "GameState.h"
#include "Window.h"
#include "Network.h"
#include "KeyMapper.h"
#include "GameSettings.h"

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
    int addGamestate(SFG::Pointer<GameState>& gsptr)
    {
        assert(gsptr.isValid());
        gsptr->getGameSettingsInterface().setGameSettings(&this->m_game_settings);
        this->g_gamestates.push_back(gsptr);
        return 0;
    }

    ///<summary>
    ///Processes all game events.
    ///This is different from update() as it just goes through
    ///every event in the event queue, applying each of its effects,
    ///but does not calculate game logic.
    ///</summary>
    void processEvents();

    void draw(sf::RenderTarget* t = nullptr);

    ///<summary>
    ///The default game loop (should be used if nothing exceptionally custom should happen)
    ///</summary>
    virtual int gameLoop();

    int update();


    ///<summary>
    ///Returns the current gamestate. There are multiple issues with this function:<br/>
    ///The gamestate might change quickly, the vector itself is NOT threadsafe.<br/>
    ///Therefore, this function only has effects when run in Debug Mode.
    ///</summary>
    ///<returns>nullptr if the gamestate could not be determined,<br/>
    ///a pointer to the current gamestate otherwise</returns>
    SFG::Pointer<GameState> getCurrentGameState() const;


    SFG::Window window;

    sf::Clock elapsedTime;

    GameSettings m_game_settings;

private:
    SFG::System m_system;
    SFG::Network m_network;

    SFG::GameLoopTiming m_timing;

    std::vector<sf::Event> m_unhandled_events;
    std::map<sf::Keyboard::Key, sf::Keyboard::Key> m_keyMap;
    std::vector<SFG::Pointer<GameState>> g_gamestates;
    size_t current_gamestate;
};
