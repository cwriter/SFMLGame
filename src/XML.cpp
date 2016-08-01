#include "stdafx.h"
#include "XML.h"



XMLReader::XMLReader()
{
}


XMLReader::~XMLReader()
{
}

int XMLWriter::setValue(const sf::String& path, const sf::String& value, int type)
{
    XMLGroup* ptr = (XMLGroup*)getXMLGroupHandle(path, type); //Get the group handle first
    if (ptr == NULL) {
        printf("[Warning] Group at %s not found in %s:%d\n", path.toAnsiString().c_str(), __FILE__, __LINE__);
        return -1;
    }
    ptr->content = value;
    return 0;
}

int XMLWriter::addGroup(const sf::String& path, XMLGroup& group, int type)
{
    XMLGroup* ptr = (XMLGroup*)getXMLGroupHandle(path, type); //Get the group handle first
    if(ptr == NULL) {
        printf("[Warning] Group at %s not found in %s:%d\n", path.toAnsiString().c_str(), __FILE__, __LINE__);
        return -1;
    }
    //add group element
    ptr->lowerElements.push_back(group);
    return 0;
}

void XMLGroup::dump() const
{
	printf("================dump================\n");
	printf("name: %s\n", name.toAnsiString().c_str());
	//First, dump attributes
	for(size_t i = 0; i < attributes.size(); i++)
	{
		printf("/%s.\t: %s\n", 
			   attributes[i].str1.toAnsiString().c_str(), attributes[i].str2.toAnsiString().c_str());
	}
	//Then, dump lower elements by recursion
	for(size_t i = 0; i < lowerElements.size(); i++)
	{
		lowerElements[i].dump();
	}
	printf("=============end of dump============\n");
}

const XMLGroup* XMLGroup::namedGroup(const sf::String& str, size_t* begin, size_t* current_index) const
{
    assert(current_index != NULL);
    assert(begin != NULL);
    sf::String match;
    match = str.substring(*begin, *current_index - *begin);
    //sf::String match(str.begin() + *begin, str.begin() + *current_index);
    *begin = *current_index + 1;
    for (; *current_index < str.getSize(); (*current_index)++)
    {
        if (str[*current_index] == L']')
        {
            //sf::String namematch(str.begin() + *begin, str.begin() + *current_index);
            sf::String namematch = str.substring(*begin, *current_index - *begin);
            *begin = *current_index;
            for (size_t g = 0; g < this->lowerElements.size(); g++)
            {
                if (this->lowerElements[g].name == match)
                {
                    for (auto a : this->lowerElements[g].attributes)
                    {
                        if (a.str1 == L"name")
                        {
                            if (a.str2 == namematch)
                            {
                                return &this->lowerElements[g];
                            }
                        }
                    }
                }
            }
        }
    }
    //printf("__xml_failure in %s:%d\n", __FILE__, __LINE__);
    return this;
}

const XMLGroup* XMLGroup::numberedGroup(const sf::String& str, size_t* begin, size_t* current_index) const
{
    //sf::String match(str.begin() + *begin, str.begin() + *current_index);
    sf::String match = str.substring(*begin, *current_index - *begin);
    int counting_left = 0;
    size_t num_start = ++(*current_index);
    for (; *current_index < str.getSize(); (*current_index)++) {
        if (str[*current_index] == L'/') {
            //Extract the number
            sf::String tmp = str.substring(num_start, *current_index - num_start);
            counting_left = std::stoi(tmp.toAnsiString());

            break;
        }
    }
    *begin = *current_index;
    if (*current_index == str.getSize()) {
        printf("__xml_failure in %s:%d (unexpected end of data input)\n", __FILE__, __LINE__);
        return this;
    }

    for (size_t g = 0; g < this->lowerElements.size(); g++) {
        if (this->lowerElements[g].name == match) {
            if (counting_left == 0) {
                return &this->lowerElements[g];
            }
            else {
                counting_left--;
            }
        }
    }

    //printf("__xml_failure in %s:%d\n", __FILE__, __LINE__);
    return this;
}

const XMLGroup* XMLGroup::defaultGroup(const sf::String& str, size_t* begin, size_t* current_index) const
{
    //sf::String match(str.begin() + *begin, str.begin() + *current_index);
    sf::String match = str.substring(*begin, *current_index - *begin);
    *begin = *current_index;
    for (size_t g = 0; g < this->lowerElements.size(); g++) {
        if (this->lowerElements[g].name == match)
        {
            return &this->lowerElements[g];
        }
    }
    return this;
}

sf::String XMLGroup::attribute(const sf::String& str, size_t* begin, size_t* current_index) const
{
    //sf::String match(str.begin() + *begin, str.begin() + *current_index);
    sf::String match = str.substring(*begin, *current_index - *begin);
    for (size_t a = 0; a < this->attributes.size(); a++) {

        if (this->attributes[a].str1 == match) {
            return this->attributes[a].str2;

        }
    }
    *begin = *current_index;
    return sf::String(L"__xml_failure");
}

sf::String XMLReader::getValue(const sf::String& path, int type) const {
    const XMLGroup* group = &m_base;
    size_t lasti = 0;

    //bool groupAlreadySet = false;
    //path means: attributes are "<attribute>.", groups are "<group>/"
    for (size_t i = 0; i < path.getSize(); i++) {
        //bool found = false;

        if (path[i] == L'[') {
            //Group
            auto tmpgroup = group->namedGroup(path, &lasti, &i);

            if (tmpgroup == group) return sf::String(L"__xml_failure");
            else group = tmpgroup;
            lasti = i + 1; //#Not sure
            continue;
        }
        else if (path[i] == L'#') {
            //Numbered group
            auto tmpgroup = group->numberedGroup(path, &lasti, &i);

            if (tmpgroup == group) return sf::String(L"__xml_failure");
            else group = tmpgroup;
            lasti = i + 1;
            continue;
        }
        else if (path[i] == L'/') {
            //Group
            auto tmpgroup = group->defaultGroup(path, &lasti, &i);

            if (tmpgroup == group) return sf::String(L"__xml_failure");
            else group = tmpgroup;
            lasti = i + 1;
            continue;			//#Not sure
        }
        else if (path[i] == L'.') {
            //Attribute (return directly)
            /*sf::String match(path.begin() + lasti, path.begin() + i);
            for (size_t a = 0; a < group->attributes.size(); a++) {

            	if (group->attributes[a].str1 == match) {
            		found = true;
            		return group->attributes[a].str2;

            	}
            }*/
            return group->attribute(path, &lasti, &i);
            /*lasti = i + 1;
            if (!found) return sf::String(L"__xml_failure");*/
        }
    }
    //still nothing returned, meaning that the content of the object is requested
    return group->content;
}

int XMLGroup::parse()
{
    //Cause otherwise bugs seem to happen //#TODO: Check
    //this->cursor = 0;

    int count = 0;
    size_t i = cursor;

    while (i < content.getSize())
    {

        for (i = 0; content[i] != L'<'; i++) {
            if (i == content.getSize()) break;
        }
        if (i == content.getSize())
            return count;	//Nothing found

        size_t copy_i = i;	//Start point for space search

        size_t delbegin = i;
        size_t begin = i + 1;	//Otherwise, we have the beginning

        for (; content[i] != L'>'; i++) {
            if (i == content.getSize()) break;
        }

        if (i == content.getSize())
            return count;	//Nothing found

        size_t name_end = i;

        //Create new
        this->lowerElements.push_back(XMLGroup());
        XMLGroup& sub = this->lowerElements.back();

        while (copy_i < i)
        {
            //Attributes
            for (; content[copy_i] != L' '; copy_i++) {
                if (copy_i == content.getSize()) break;
            }
            if (copy_i > i) break;

            if (name_end == i)
                name_end = copy_i;

            //Get attribute

            size_t begin = ++copy_i;

            for (; content[copy_i] != L'='; copy_i++) {
                if (copy_i == content.getSize()) break;
            }
            if (!(copy_i < i)) break;

            sub.attributes.push_back(XMLPair());
            XMLPair& pair = sub.attributes.back();

            pair.str1 = content.substring(begin, copy_i++ - begin);
            //pair.str1 = sf::String(content.begin() + begin, content.begin() + copy_i++);
            begin = ++copy_i;

            for (; content[copy_i] != L'\"'; copy_i++) {
                if (copy_i == content.getSize()) break;
            }
            //pair.str2 = sf::String(content.begin() + begin, content.begin() + copy_i++);
            pair.str2 = content.substring(begin, copy_i++ - begin);
        }




        //sub.name = sf::String(content.begin() + begin, content.begin() + name_end);
        sub.name = content.substring(begin, name_end - begin);

        sf::String match = sf::String(L"</") + sub.name + sf::String(L">");
        sf::String same = L"<" + sub.name + L">";
        //Now we want to get the match (aka closing tag)
        begin = i + 1;	//Set a new beginning

        size_t next = begin;
        size_t end = begin;

        //printf("Content:\n%s\n", content.toAnsiString().c_str());

        while (next <= end) //While there are other elements of the same name inside, skip to the correct end
        {
            //printf("same: \"%s\"\nmatch: \"%s\"\n", same.toAnsiString().c_str(), match.toAnsiString().c_str());
            next = this->content.find(same, next);
            end = this->content.find(match, end);
            if (end == this->content.InvalidPos) {
                printf("Error parsing xml file: Wrong format\n");
                return -1;
            }
        }


        //Now cut to next element
        //sub.content = sf::String(content.begin() + begin, content.begin() + end);
        sub.content = content.substring(begin, end - begin);
        this->content.erase(delbegin, end + match.getSize() - delbegin);

        //recursive action now
        int res = sub.parse();
        count += res;

        //cursor = end;
    }
    return count;
}

const XMLGroup* XMLReader::getXMLGroupHandle(const sf::String& path, int type) const
{
    if (path == L"") return &m_base; //No interaction requested
    const XMLGroup* group = &m_base;
    size_t lasti = 0;

    //path means: attributes are "<attribute>.", groups are "<group>/"
    for (size_t i = 0; i < path.getSize(); i++)
    {
        if (path[i] == L'[')
        {
            //Group
            auto tmpgroup = group->namedGroup(path, &lasti, &i);

            if (tmpgroup == group) return NULL;
            else group = tmpgroup;

            lasti = i + 1 + 1;
            i++;
            continue;
        }
        else if (path[i] == L'#')
        {
            //Numbered group
            auto tmpgroup = group->numberedGroup(path, &lasti, &i);

            if (tmpgroup == group) return NULL;
            else group = tmpgroup;

            lasti = i + 1;
            continue;
        }
        else if (path[i] == L'/')
        {
            //Group
            auto tmpgroup = group->defaultGroup(path, &lasti, &i);

            if (tmpgroup == group) return NULL;
            else group = tmpgroup;

            lasti = i + 1;
            continue;
        }
    }
    //still nothing returned, meaning that the content of the object is requested
    return group;
}

double XMLReader::asDouble(const sf::String& path, bool& real) const
{
    sf::String val = getValue(path);
    if (val != L"__xml_failed")
        real = true;
    else
	{
        real = false;
		return 0.0;
	}
    return std::stod(val.toAnsiString());
}

int XMLReader::asInt(const sf::String& path, bool& real) const
{
    int ret = 0;
    sf::String val = getValue(path);
    if (val != L"__xml_failed")
        real = true;
    else
        real = false;
    try {
        ret = std::stoi(val.toAnsiString());
    }
    catch (std::exception& e) {
#ifdef _DEBUG
        //DebugBreak();
#endif
        puts(e.what());

    }
    return ret;
}

sf::FloatRect XMLReader::asFloatRect(const sf::String& path, bool& real) const
{
    sf::FloatRect fr;
    sf::String str = this->getValue(path);
    if (str == L"__xml_failure") {
        real = false;
        return fr;
    }
    std::istringstream f(str.toAnsiString());
    std::string part;
    size_t i = 0;
    while (std::getline(f, part, ';')) {
        switch (i) {
        case 0:
            fr.left = std::stof(part);
            break;
        case 1:
            fr.top = std::stof(part);
            break;
        case 2:
            fr.width = std::stof(part);
            break;
        case 3:
            fr.height = std::stof(part);
            break;
        default:
            printf("Error: \"%s\" is not a correctly formatted rect string\n", str.toAnsiString().c_str());
            real = false;
            return fr;
        }
        i++;
    }
    real = true;
    return fr;
}

XMLReader::XMLReader(const XMLGroup& group, bool parse)
{
    this->m_base = group;
    if (parse)
    {
        if (this->parse() != 0)
        {
            printf("[Warning] Parsing in constructor failed in %s:%d\n", __FILE__, __LINE__);
        }
    }
}
