#include "stdafx.h"
#include "Game.h"



Game::Game()
{
    //Default event queue size is 64.
    m_unhandled_events.reserve(SFG_EVENT_QUEUE_SIZE);
    //Set the current gamestate correctly
    this->current_gamestate = 0;

    //Test the stack call lister
    std::string out;
    SFG::Util::getStackTrace(out);
    printf("%s\n", out.c_str());
}

Game::~Game()
{
}

SFG::Pointer<GameState> Game::getCurrentGameState() const
{
#ifndef _DEBUG
    printf("[Error/Development] Function that was marked for debug only was called in %s:%d\n", __FILE__, __LINE__);
    return SFG::Pointer<GameState>(nullptr); //We do NOT allow this in release mode (guess why...)
#else
    if (current_gamestate < g_gamestates.size())
        return g_gamestates[current_gamestate];
    else
        return SFG::Pointer<GameState>(nullptr);
#endif
}

int Game::load(sf::String path, SFG::SplashScreen* ss)
{
    XMLReader reader;
    sf::String out;
    if (basicLoadFile(path.toWideString(), out) != 0)
    {
        printf("[Error] Failed to load file %s in %s:%d\n", path.toAnsiString().c_str(), __FILE__, __LINE__);
        return -1;
    }


    reader.setSource(out);

    //Start parsing
    if (reader.parse() != 0)
    {
        //Errors happening
    }
    //For now, we only need to load the gamestates up...

    //Get the loading gamestate (with fancy animations and stuff)
    int ret = 0;
    auto sugs = SFG::Pointer<GameState>(new StartupGameState());
    if ((ret = loadGamestate(reader, L"Startup", sugs)) < 0)
    {
        printf("[Error] Failed to load Startup Gamestate in %s:%d from file \"%s\" with Code %d.\n", __FILE__, __LINE__, path.toAnsiString().c_str(), ret);
    }
    //The first gamestate always needs to be inited
    this->g_gamestates[0]->init(this->window);
    //Get the menu gamestate
    SFG::Pointer<GameState> menu(new MenuGameState());
    if ((ret = loadGamestate(reader, L"Menu", menu)) < 0)
    {
        printf("[Error] Failed to load Menu Gamestate in %s:%d from file \"%s\" with Code %d.\n", __FILE__, __LINE__, path.toAnsiString().c_str(), ret);
    }


    //Get all other gamestates
    for (size_t i = 2; ; i++)
    {
        sf::String path = L"xml/gamestate#" + std::to_wstring(i) + L"/";
        sf::String ret = reader.getValue(path);
        if (ret == L"__xml_failure")
        {
            break;
        }
        else
        {
            //#TODO: Add custom gamestate
            //STILL_TO_DO(__FILE__, __LINE__);

            int res = loadGamestate(reader, reader.getValue(path + "name."), SFG::Pointer<GameState>(nullptr));
            if (res != 0)
            {
                std::string tmp = reader.getValue(path + "name.").toAnsiString();
                SFG::Util::printLog(SFG::Util::LogMessageType::Error, __FILE__, __LINE__, "Failed to load custom gamestate named \"%s\"", tmp.c_str());
            }

        }

    }



    //Reading done
    if (ss != nullptr)
    {
        //Tell the SplashScreen to disappear
        ss->notify(SFG::SplashScreen::SplashScreenMessages::LoadingDone);
    }
    return 0;
}

int Game::loadGamestate(const XMLReader& reader, const sf::String& GSname, const SFG::Pointer<GameState>& included)
{
    SFG::Pointer<GameState> GS_ptr;

    sf::String GSxmlpath = L"xml/gamestate[" + GSname + L"]";
    auto str = reader.getValue(GSxmlpath, 0);

    if (str == L"__xml_failure")
    {
        printf("[Error] Unspecified startup gamestate in %s:%d.\n", __FILE__, __LINE__);
        return -2;
    }
    else
    {

        if (str == L"#included")
        {
            //Use default
            GS_ptr.reset(included);
            if (!GS_ptr.isValid() && included.getElement() != nullptr)
            {
                printf("[Critical] Failed to allocate memory for \"%s\" in %s:%d\n",GSname.toAnsiString().c_str(), __FILE__, __LINE__);
                return -1;
            }

        }
        else
        {
            //Use specified
            //#TODO
            STILL_TO_DO(__FILE__, __LINE__);
            //We first need an instance of a DLLLoadedGameState
            GS_ptr.reset(new DLLLoadedGameState());
            //Set the name
            GS_ptr->setName(GSname);
            if (GS_ptr->getName() == "__xml_failure")
            {
                SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__, "Failed to get Gamestate name, aborting.");
                return -1;
            }
            //Then, load the specified file
            if (GS_ptr.cast<DLLLoadedGameState>()->loadFromDll(str.toAnsiString().c_str()) != 0)
            {
                SFG::Util::printLog(SFG::Util::Error, __FILE__, __LINE__,
									"Failed to load gamestate \"%s\" from DLL", str.toAnsiString().c_str());
                return -2;
            }

            //We actually should be done


        }

        //Get all modules
        sf::String uses = reader.getValue(GSxmlpath + L"uses.");
        if (uses != L"__xml_failure")
        {
            //We have specifiers
            size_t begin = 0;
            size_t last = 0;
            //Begin needs to be checked as the last module wouldn't be counted otherwise
            while ((last = uses.find(L',', last)) != sf::String::InvalidPos || begin != sf::String::InvalidPos)
            {
                sf::String string;
                if (last == sf::String::InvalidPos)
                {
                    //If the end would otherwise exceed the string length, set it to the maximum (sf::String::end()).
                    //string = sf::String(uses.begin() + begin, uses.end());
                    string = uses.substring(begin, uses.getSize() - begin);
                }
                else
                {
                    //If a ',' was found, use its position
                    //string = sf::String(uses.begin() + begin, uses.begin() + last);
                    string = uses.substring(begin, last - begin);
                }
                //Check for module names
                if (string == L"G2D")
                {
                    SFG::Pointer<ModuleG2D> ptr(new ModuleG2D());
                    if (!ptr.isValid())
                    {
                        char buf[512];
#ifdef _WIN32
                        strerror_s(buf, errno);
#else
                        strerror_r(errno, buf, 512);
#endif // _WIN32
                        printf("[Error] Failed to create Module G2D in %s:%d: %s\n", __FILE__, __LINE__, buf);
                        return -3;
                    }
                    GS_ptr->addModule(ptr);
                }
                else
                {
                    //#TODO: Add more modules (sound etc.)
                    STILL_TO_DO(__FILE__, __LINE__);
                }
                if(last != sf::String::InvalidPos) begin = last + 1; //if a ',' was found, go search on for the next one
                else begin = last; //Meaning begin is sf::String::npos
            }
        }

        //Check for external asset list
        auto ext_assets = reader.getValue(GSxmlpath + "assets.");
        if (ext_assets != "__xml_failure")
        {
            //We found external assets, load them
        }
        auto assets = reader.getXMLGroupHandle(L"xml/assetlist[" + GSname + L"]");
        if (assets == nullptr)
        {
            printf("[Warning] No assets for Gamestate \"%s\" found. This is not encouraged and might be an error. Please check the game.xml file. In: %s:%d\n", GSname.toAnsiString().c_str(), __FILE__, __LINE__);
            //return 1;
        }
        else
        {
            auto ret = GS_ptr->loadAssets(XMLReader(*assets));
            if (ret != 0)
            {
                printf("[Error] Failed to load assets for Gamestate \"%s\" in %s:%d.\n", GSname.toAnsiString().c_str(), __FILE__, __LINE__);
            }
        }

        //Empty path to tell the gamestate to just update what it has (or do nothing if nothing has to be done)
        GS_ptr->load(L"");

        //this->g_gamestates.push_back(GS_ptr);
        this->addGamestate(GS_ptr);
    }
    return 0;
}

void Game::processEvents()
{
    //Clear before any new loop
    m_unhandled_events.clear();
    window.processEvents(m_unhandled_events);
    //printf("Event vector has %zd elements.\n", m_unhandled_events.size());
    for (auto e : m_unhandled_events)
    {
        if (e.type == sf::Event::KeyPressed || e.type == sf::Event::KeyReleased)
        {
            //We shall remap everything - however, make sure to actually process these as otherwise, they could loop forever
            e.key.code = this->keyMapper(e.key.code, false);
        }
    }
    //Let the UI pick what it wants and set picked values to sf::event::count to make it unusable for further use
    if (current_gamestate >= g_gamestates.size())
        return;
    this->g_gamestates[this->current_gamestate]->UI()->processEvents(m_unhandled_events); //Handle for UI first (as it is more likely to be on top
    this->g_gamestates[this->current_gamestate]->processEvents(window, m_unhandled_events);		 //Then handle for the gamestate itself
}

void Game::draw(sf::RenderTarget* t)
{
    if (g_gamestates.size() <= current_gamestate) {
        return;	//Don't attempt to draw a gamestate if it isn't present
    }
    if (t == nullptr) {
        //draw on window
        g_gamestates[current_gamestate]->draw(window);
    }
    else {
        //draw on specified target
        g_gamestates[current_gamestate]->draw(t);
    }
}

int Game::parseArgs(int argc, char* argv[])
{
    //check argument count:
    if (argc == 1) return 0; //Nothing to do

    if (argv[1] == "-help")
    {
        //Print the help information

        printf(
            "SFMLGameEngine CLI options:\n"
            "\t-help:\t\tPrint this information\n"
            "\t-stream:\t\tEnables streaming server. See -stream -help for more information\n"
        );
    }
    else if (argv[1] == "-stream")
    {
        if (argv[2] == "-help")
        {
            printf(
                "Help for -stream:\n"
                "-stream starts a server that can stream content to another computer to enable remote playing.\n"
                "Note that starting the server might take some time.\n");
        }
    }
    return 0;
}

int Game::gameLoop()
{
    //With 30 fps, we are limited to 33.3ms that this function can take.
    //As 60 fps should be targetted, we are aiming for 16.6ms.

    sf::Clock clk;

    //Processing events should not take more than 2 ms
    this->processEvents();

    this->m_timing.processingTime = clk.restart().asMicroseconds() / 1000.f;

    //Updating the game logic should not take more than 10ms
    this->update();

    this->m_timing.updatingTime = clk.restart().asMicroseconds() / 1000.f;

    //This leaves us with 4ms for drawing
    this->window.startDraw();

    this->draw();

    this->window.endDraw();

    this->m_timing.drawingTime = clk.restart().asMicroseconds() / 1000.f;


    /*SFG::Util::printLog(SFG::Util::LogMessageType::Development, __FILE__, __LINE__, "(%f|%f|%f)",
          m_timing.processingTime,
          m_timing.updatingTime,
          m_timing.drawingTime
    );*/
	
#ifdef _SFGPROFILE
	if(m_timing.processingTime > 2.f)
		SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__,
							"Exceeding expected time for processing (%f > 2.f)", m_timing.processingTime);
	if(m_timing.updatingTime > 10.f)
		SFG::Util::printLog(SFG::Util::Warning, __FILE__, __LINE__,
							"Exceeding expected time for updating (%f > 10.f)", m_timing.updatingTime);
#endif
    if (!this->window.getSFMLWindow().isOpen()) {
        //Window has been closed, generally, we should save everything (if not yet done)
        //And then end the program
        return 1;
    }

    return 0;
}

int Game::update()
{
    if (g_gamestates.size() <= current_gamestate) {
        return -1;	//Don't attemt to update a gamestate if it isn't present
    }
    auto ret = g_gamestates[current_gamestate]->update(elapsedTime.restart().asMicroseconds() / 1000.f);
    if (g_gamestates[current_gamestate]->switch_to != "")
    {
        //We need to switch gamestates
        for (size_t i = 0; i < g_gamestates.size(); i++)
        {
            if (g_gamestates[i]->getName() == g_gamestates[current_gamestate]->switch_to)
            {
                //Neutralize switchto command
                g_gamestates[current_gamestate]->switch_to = "";
                current_gamestate = i;
                //Init the gamestate (mostly for view(-port) reasons
                g_gamestates[current_gamestate]->init(this->window);
                return ret; //Exit loop; no need to continue counting at this point
            }
        }
        //Check if game requests exit
        if (g_gamestates[current_gamestate]->switch_to == "exit_gamestate")
        {
            //Need to exit
            this->window.getSFMLWindow().close();
        }
    }
    return ret;
}

int Game::SplashScreenLoad(sf::String path)
{
    SFG::SplashScreen ss;

    SFG::SplashScreenItem ssi1;
    ssi1.m_display_time = 5000;
    sf::Texture tex;
    tex.loadFromFile("Launch/Splash.png");
    ssi1.m_tex.setTexture(tex);

    std::future<int> fut = std::async(&Game::load, this, path, &ss);

    ss.exec();

    return fut.get();
}
