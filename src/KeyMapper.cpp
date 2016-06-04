#include "stdafx.h"
#include "KeyMapper.h"


namespace SFG
{
KeyMapper::KeyMapper()
{
}


KeyMapper::~KeyMapper()
{
}

sf::Keyboard::Key KeyMapper::keyMapper(sf::Keyboard::Key code, bool ignoreUnknown) const
{
    auto it = m_keyMap.find(code);
    if (it != m_keyMap.end())
    {
        return it->second;
    }
    else if (ignoreUnknown)
    {
        return sf::Keyboard::Key::Unknown;
    }
    else
    {
        return code;
    }
}
};
