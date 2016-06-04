#include "stdafx.h"
#include "SSG_Ship.h"



SSG_Ship::SSG_Ship()
{
}


SSG_Ship::~SSG_Ship()
{
}

int SSG_Ship::load(XMLReader& reader)
{
    //Load ship
    reader.for_all("xml/ship", [&](const XMLGroup* group) {
        XMLReader r(*group);
        r.parse();
        r.for_all("part", [&](const XMLGroup* group) {
            SFG::Pointer<SSG_Part> part(new SSG_Part());
            this->m_parts.push_back(part);
        });
    });
    return 0;
}
