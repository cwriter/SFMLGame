#include "stdafx.h"
#include "StringManager.h"



StringManager::StringManager()
{
	m_default.main = "en";
	m_default.sub = "us";
}

StringManager::StringManager(const sf::String& str) {
	m_default.main = "en";
	m_default.sub = "us";
	XMLReader r;
	sf::String data;
	try {
		if (basicLoadFile(str, data) != 0) {
			//Error
			throw -1;
		}
	}
	catch (int e) {
		printf("[Error] Failed to load file in StringManager(const sf::String&). (%d)\n", e);
		return;
	}
	r.setSource(data);
	r.parse();
	if (load(r) != 0) {
		throw "Could not load data in StringManager constructor.\n";
	}
}

int StringManager::load(const XMLReader& reader) {
	//Load all strings and/or tmpload them
	sf::String path;
	size_t i = 0;
	while (true) {
		path = L"xml/string#" + std::to_wstring(i) + L"/";
		sf::String res = reader.getValue(path);
		if (res == L"__xml_failure")
			break;

		MLString mlstr;
		mlstr.name = reader.getValue(path + L"name.");

		size_t ii = 0;
		while (true) {
			sf::String langp = path + L"lang#" + std::to_wstring(ii) + L"/";
			sf::String res = reader.getValue(langp);
			if (res == L"__xml_failure")
				break;

			sf::String langname = reader.getValue(langp + L"name.");

			size_t iii = 0;
			while (true) {
				sf::String sub = langp + L"sub#" + std::to_wstring(iii) + L"/";
				sf::String res = reader.getValue(sub);
				if (res == L"__xml_failure")
					break;

				sf::String subname = reader.getValue(sub + L"name.");

				lang l(langname, subname);
				mlstr.m_map[l] = res;


				//Add
				iii++;
			}
			ii++;
		}
		i++;
		this->m_strings.push_back(mlstr);
	}
	return 0;
}

sf::String StringManager::getString(const sf::String name, const lang& l) const
{
	std::vector<MLString>::const_iterator v_it = std::find(m_strings.begin(), m_strings.end(), name);
	if (v_it == m_strings.end()) {
		return name;	//Not found
	}
	lang* ptr = NULL;
	if (l == std::string("**-**")) {
		ptr = (lang*)&m_default;
	}
	else {
		ptr = &((lang&)l);
	}
	std::map<lang, sf::String>::const_iterator it = v_it->m_map.find(*ptr);
	if (it == v_it->m_map.end()) {
		//Not found, try fallbacks. It will use any language of the main group if possible
		std::map<std::string, lang>::const_iterator fb_it = m_fallback.find(ptr->main);
		if (fb_it == m_fallback.end()) {
			//No fallback found
			return name;
		}
		else {
			//Recursive search
			return getString(name, fb_it->second);
		}

	}
	else {
		return it->second;
	}
}

StringManager::~StringManager()
{
}
