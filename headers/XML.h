#pragma once
#include "stdafx.h"

class XMLReader;
class XMLWriter;

class XMLPair
{
public:
    XMLPair(const sf::String& s1, const sf::String& s2)
        : str1(s1), str2(s2)
    {

    }

    XMLPair()
    {

    }

    sf::String str1;
    sf::String str2;

};

class XMLGroup
{
public:
    XMLGroup() {
        cursor = 0;

    }

    ///<summary>
    ///Returns a pointer to the group that's named as in str from *begin to *current_index.
    ///</summary>
    ///<param name="str">A reference to the path to the requested resource.</param>
    ///<param name="begin">A pointer to the index where the name of the named group begins.</param>
    ///<param name="current_index">A pointer to the index where the name of the named group ends.</param>
    const XMLGroup* namedGroup(const sf::String& str, size_t* begin, size_t* current_index) const;

    ///<summary>
    ///Returns a pointer to the group that's numbered as in str from *begin to *current_index.
    ///</summary>
    ///<param name="str">A reference to the path to the requested resource.</param>
    ///<param name="begin">A pointer to the index where the number of the numbered group begins.</param>
    ///<param name="current_index">A pointer to the index where the number of the numbered group ends.</param>
    const XMLGroup* numberedGroup(const sf::String& str, size_t* begin, size_t* current_index) const;

    ///<summary>
    ///Returns a pointer to the group that's typed as in str from *begin to *current_index.
    ///</summary>
    ///<param name="str">A reference to the path to the requested resource.</param>
    ///<param name="begin">A pointer to the index where the type of the typed group begins.</param>
    ///<param name="current_index">A pointer to the index where the type of the typed group ends.</param>
    const XMLGroup* defaultGroup(const sf::String& str, size_t* begin, size_t* current_index) const;

    ///<summary>
    ///Returns a pointer to the attribute string in the path that's named as in str from *begin to *current_index.
    ///</summary>
    ///<param name="str">A reference to the path to the requested resource.</param>
    ///<param name="begin">A pointer to the index where name of the named attribute begins.</param>
    ///<param name="current_index">A pointer to the index where name of the named attribute ends.</param>
    sf::String attribute(const sf::String& str, size_t* begin, size_t* current_index) const;

    int parse();

	
	///<summary>
	///Dumps the contents of this group to the standard output
	///</summary>
	void dump() const;

    size_t cursor;
    sf::String name;
    std::vector<XMLPair> attributes;
    sf::String content;
    std::vector<XMLGroup> lowerElements;
private:
};

class XMLReader
{
public:
    XMLReader();
    XMLReader(const XMLGroup& group, bool parse = true);
    virtual ~XMLReader();

    void setSource(const sf::String& text) {
        this->m_base.content = text;
    }

    int parse() {
        return this->m_base.parse();
    }

    ///<summary>
    ///Gets a value from the xml file.
    ///</summary>
    ///<param name="path">The path to the value. / means Group, '.' means attribute -> [group]/[attribute]. <br/>
    ///To access named objects, use '[' and ']' after the identifier- So: group[name]/attribute. Note that the this works using the 'name'-Attribute only! <br/>
    ///To access objects by index, use #[num]. For example "group#5/" for the 6th element.
    ///When not using [], the function will return the first appearance of the group type
    ///</param>
    ///<param name="type">Unused, set to 0</param>
    sf::String getValue(const sf::String& path, int type = 0) const;

    double asDouble(const sf::String& path, bool& real) const;

    int asInt(const sf::String& path, bool& real) const;

    sf::FloatRect asFloatRect(const sf::String& path, bool& real) const;

    ///<summary>
    ///Gets an XMLGroup from the xml file. Please note that this code is nearly equivalent to getValue and should be treated as such
    ///</summary>
    ///<param name="path">The path to the value. / means Group -> [group]/ \
    ///To access named objects, use '[' and ']' after the identifier- So: group[name]/ Note that the this works using the 'name'-Attribute only! \
    ///When not using [], the function will return the first appearance of the group type
    ///</param>
    ///<param name="type">Unused, set to 0</param>
    const XMLGroup* getXMLGroupHandle(const sf::String& path, int type = 0) const;


    ///<summary>
    ///Calls the specified function on every numbered entry of the given path
    ///</summary>
    size_t for_all(const sf::String& path, std::function<void(const XMLGroup*)> func) const
    {
        size_t i = 0;
        while (true)
        {
            sf::String exactpath = path + "#" + std::to_string(i) + "/";
            auto handle = this->getXMLGroupHandle(exactpath);
            if (handle == nullptr)
            {
                //Nothing found, return from this function
                break;
            }
            //Otherwise, call the specified function
            func(handle);
            i++;
        }
        return i;
    }

protected:
    XMLGroup m_base;

};

class XMLWriter : public XMLReader
{
public:
    //XMLWriter();
    //~XMLWriter();

    void setBaseGroup(const XMLGroup& base)
    {
        this->m_base = base;
    }

    int setValue(const sf::String& path, const sf::String& value, int type = 0);

    int addGroup(const sf::String& path, XMLGroup& group, int type = 0);
private:

};
