#include "stdafx.h"

/*
sf::String s2ws(const std::string& str)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	sf::String_convert<convert_typeX, wchar_t> converterX;

	return converterX.from_bytes(str);
}

std::string ws2s(const sf::String& wstr)
{
	typedef std::codecvt_utf8<wchar_t> convert_typeX;
	sf::String_convert<convert_typeX, wchar_t> converterX;

	return converterX.to_bytes(wstr);
}
*/
int split(std::vector<sf::String>& out, sf::String& str) {
    std::wistringstream f(str);
    std::wstring s;
    int i = 0;
    //while (std::getline(f, s, L';')) {
    while (std::getline(f, s, L';')) {
        out.push_back(sf::String(s));
        i++;
    }
    return i;
}

int basicLoadFile(const sf::String& xmlpath, sf::String& out)
{
    //File is generally xml
    sf::FileInputStream fis;
    if (!fis.open(xmlpath))
    {
        perror("[Error] Failed to read file");
        return -1;
    }
    sf::Int64 filesize = fis.getSize();
    fis.seek(0);
    char* filedata = (char*)calloc(size_t(filesize), 1);
    if (filedata == NULL)
    {
        perror("ALLOCATION ERROR");
        return -2;
    }

    size_t ret = fis.read(filedata, filesize);
    assert(ret == filesize);

    std::string tmpstr(filedata, filesize);

    free(filedata);

    out = sf::String::fromUtf8(tmpstr.begin(), tmpstr.end());

    return 0;
}
