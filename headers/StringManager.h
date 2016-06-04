#pragma once
#include "stdafx.h"
#include "XML.h"

class lang {
public:
    lang() {

    }
    lang(const std::string& s) {
        main = std::string(s.begin(), s.begin() + 2);
        sub = std::string(s.begin()+3, s.end());
    }

    lang(const sf::String& main, const sf::String& sub) {
        this->main = main;
        this->sub = sub;
    }

    std::string asString() const {
        return main + "-" + sub;
    }

    bool operator<(const lang& l) const {
        return this->asString() < l.asString();
    }

    bool operator==(const lang& l) const {
        return (this->asString() == l.asString());
    }

    bool operator==(const lang l) const {
        return (this->asString() == l.asString());
    }

    bool operator==(const std::string& s) const {
        return (this->asString() == s);
    }


    std::string main;
    std::string sub;
};

class MLString {
public:

    bool operator==(const sf::String& s) {
        return (name == s);
    }

    bool operator==(const sf::String& s) const {
        return (name == s);
    }


    sf::String name;
    std::map<lang, sf::String> m_map;
};

class StringManager
{
public:
    StringManager();
    StringManager(const sf::String& str);
    ~StringManager();

    int load(const XMLReader& reader);

    sf::String getString(const sf::String name, const lang& l) const;

private:
    lang m_default;
    std::vector<MLString> m_strings;
    std::map<std::string, lang> m_fallback;	//Langstring to use if the desired language is not available
};

