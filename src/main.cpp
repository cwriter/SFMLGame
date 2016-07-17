#include "stdafx.h"
#include "GameState.h"
#include "Game.h"
#include "UI.h"
//#include "DynMusicEngine.h"
#include "ExtensionInterface.h"
#include "DebugWindow.h"
#include "SSG_Game.h"
#include "SOW_Game.h"
#include <gmpxx.h>


#ifndef _WIN32
#include <X11/Xlib.h>
#endif

#define RUNNING_SOW 1
//#define RUNNING_SSG 1

int testingStuff(int argc, char* argv[])
{
    auto dbgthread = std::async([&]()
    {
        printf("SFG Startup: Debug Information:\n");
        printf("Path of exe: \"%s\"\n", argv[0]);
        printf("Maximum texture size: %d\n", sf::Texture::getMaximumSize());
        printf("Hardware concurrency potential: %d\n", std::thread::hardware_concurrency());
    });
    /*DynMusicEngine dme;
    dme.create("NUL");
    dme.start();
    //dme.play("ASDIh");
    dme.end();
    */

    /*dynVar t1(std::string("Das ist ein "));
    dynVar test(std::string("Test"));
    //dynVar test(int(20));
    try {
    t1.append(&test);
    }
    catch (dynVarException& e) {
    printf(e.what());
    }
    std::string t = t1.STRFunc_toString();
    */
    ScriptEngine se;
    se.load(L"Script/script - test.cpp");
    std::vector<dynVar*> args;
    args.push_back(new dynVar(4));
    args.push_back(new dynVar("ora et lab"));
    dynVar* out = se.Invoke(L"repeatString", args);
    if (out == NULL) {
        printf("ERROR.");
        for (auto a : args) {
            delete a;
        }
    }
    else {
        printf("Result is \"%s\"\n", out->toString().c_str());
    }

    /*
    ExtensionInterface ei;
    ei.load(L"");
    */

    //system("PAUSE");
    //return 0;



    //Init loading UI
    XMLReader UIReader;

    SFG::UIManager uim;
    /*UIWindow* win = new UIWindow();
    if (win == NULL) {
    perror("ALLOCATION ERROR");
    exit(-1);
    }
    sf::String data;
    basicLoadFile(L"Data/example - window.xml", data);
    XMLReader winRead;
    winRead.setSource(data);
    winRead.parse();
    win->load(winRead, sf::String(L"Test-Window"), StringManager(L"Data/example - strings.xml"));
    uim.addWindow(win);*/

    uim.load("Data/example - uimanager.xml", StringManager(L"Data/example - strings.xml"));

    //========================NEEDS TO BE COPIED TO THE MAIN FUNC TO TEST THE UIMAN==============
    //uim.setTarget(game.window);

    ModuleLoader ml;
    ModuleG2D mg2d;
    //ml.load(L"Data/example - g2d.xml", mg2d, L"xml/module[G2D]/");


    //system("PAUSE");
    //return 0;

    //--------------------------------
    dbgthread.wait();
    return 0;
}

int main(int argc, char* argv[])
{
#ifdef _DEBUG
#ifdef _WIN32
    _CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif // _WIN32
#endif
#ifndef _WIN32
    XInitThreads();
#endif // _WIN32
	//Set the precision we need (we'll stick with 128 bits for the moment)
	mpf_set_default_prec(128);
	{
		//Scope of the actual game
		testingStuff(argc, argv);

		Game game;

		game.parseArgs(argc, argv);

		int ret = game.load();
#ifdef RUNNING_SSG
		
		//Add the SSG gamestate
		SFG::Pointer<GameState> SSG_gs(new SSG_Game);
		//Load the game (give a folder path)
		int SSG_load_ret = SSG_gs->load("Data/SSG/");
		if(SSG_load_ret != 0)
		{
			//Loading failed
			SFG::Util::printLog(SFG::Util::Development, __FILE__, __LINE__, 
								"Failed to load SSG with code %d", SSG_load_ret);
		}
		game.addGamestate(SSG_gs);
#elif RUNNING_SOW
		SFG::Pointer<GameState> SOW_gs(new SOW_Game);
		game.addGamestate(SOW_gs);
		
#endif
		//game.window.create(1024, 720, "Game");
		game.window.create(1920, 1080, "Game");
		game.window.setScale(1.f);

		game.window.getSFMLWindow().setFramerateLimit(60);


		//Create debug window and overwatch the game
	#ifdef _DEBUG
		SFG::DebugWindow dbgwnd;
		dbgwnd.create(&game);
	#endif


		//Set the timer to 0 again
		printf("Loading the engine core took %f seconds.\n", game.elapsedTime.restart().asMilliseconds() / 1000.f);

		//Loop while the game doesn't require any breakage
		while (game.gameLoop() == 0);

	#ifdef _DEBUG
		//dbgwnd.close();
		system("PAUSE");
	#endif
	}

	printf("Exit succeeded. Have nice day :)");
    return 0;
}
