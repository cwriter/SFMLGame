#pragma once
#include "stdafx.h"
#include "SOW_Road.h"
#include "XML.h"

class SOW_main
{
public:
    SOW_main();
    ~SOW_main();

    int load() {
        sf::String str;
        basicLoadFile("Gamestates/game.xml", str);
        m_reader.setSource(str);
        m_reader.parse(); //Parse. Consider moving it to a separate thread



        return 0;
    }
private:
    XMLReader m_reader;
};

