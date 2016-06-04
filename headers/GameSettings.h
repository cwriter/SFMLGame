#pragma once
#include "stdafx.h"
#include "Message.h"

///<summary>
///Holds game settings. By default these are:
///*Resolution
///[More to come]
///</summary>
class GameSettings
    : public MsgObject
{
public:
    enum GameSettingsMessages
    {
        ApplySettings = 0
    };

    GameSettings();
    ~GameSettings();

    ///<summary>
    ///Applies settings by informing listeners that the settings held by "this" should be applied.
    ///</summary>
    int applySettings()
    {
        //Notify the listeners (preferably an instance of the "Game"-class)
        this->dynNotifyAll(
            GameSettingsMessages::ApplySettings,
            SFG::Pointer<MsgPackage>(nullptr));
        return 0;
    }
private:
    SFG::Vector2<int> m_set_eng_res;//Holds the resolution
    int m_set_eng_win_mode;			//Holds the window mode (0=Window, 1=Borderless, 2=Fullscreen)

    float m_set_eng_ui_scale;		//Holds the UI scale (default: 1.f)
};

///<summary>
///Provides an interface to the gamestates to get the game settings
///</summary>
class GameSettingsInterface
{
public:



    ~GameSettingsInterface()
    {

    }

    void setGameSettings(GameSettings* gs)
    {
        std::lock_guard<std::mutex> lck(m_lock);
        this->m_gs = gs;
    }

    inline GameSettings* getGameSettings()
    {
        std::lock_guard<std::mutex> lck(m_lock);
        return this->m_gs;
    }


private:
    GameSettings* m_gs;
    std::mutex m_lock;
};
