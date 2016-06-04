#pragma once
#include "stdafx.h"

namespace SFG
{
class KeyMapper
{
public:
    KeyMapper();
    ~KeyMapper();

    ///<summary>
    ///Maps key inputs to logical actions. This just converts the saved numbers to other numbers.
    ///</summary>
    ///<param name="code">The code to translate</param>
    ///<param name="ignoreUnknown">If set, keys that could not be resolved will be returned as Unknown and thus will have no effect (default). If set to false,
    ///the input code will be returned if it could not be resolved</param>
    ///<returns>The converted value if a conversion is available, sf::Keyboard::Key::Unknown if 'ignoreUnknown' is true (default),
    ///the input code otherwise
    ///</returns>
    sf::Keyboard::Key keyMapper(sf::Keyboard::Key code, bool ignoreUnknown = true) const;

private:
    std::map<sf::Keyboard::Key, sf::Keyboard::Key> m_keyMap; //Provides a keymap
};

};
