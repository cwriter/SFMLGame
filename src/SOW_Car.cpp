#include "stdafx.h"
#include "SOW_Car.h"



SOW_Car::SOW_Car()
{
}


SOW_Car::~SOW_Car()
{
}

int Module_SOW_Car::load(const XMLReader& xmldata)
{
    //See comment below
    if (ModuleG2D::load(xmldata) != 0) {
        //Some error happened
        return -1;
    }
    //Load from xml
    sf::String path;
    size_t i = 0;
    while (true) {
        path = sf::String(L"object#") + (std::to_wstring(i)).append(L"/");
        sf::String result = xmldata.getValue(path);
        if (result == L"__xml_failure") {
            //Data not found (meaning index out of range)
            break;
        }
        //Otherwise, use data
        //sf::String namepath = path + L"name.";
        //sf::String name = xmldata.getValue(path);

        //See ModuleG2D::load for examples

        //Load specifc car data
        sf::String blinkerpath = path + L"blinker";
        const XMLGroup* data = xmldata.getXMLGroupHandle(blinkerpath);
        if (data == NULL) {
            break;
        }
        XMLReader blinkerReader(*data);
        blinkerReader.parse();
        size_t b_index = 0;
        while (true)
        {
            const XMLGroup* blinkerLight = blinkerReader.getXMLGroupHandle(L"Light#" + std::to_wstring(b_index) + L"/");
            if (blinkerLight == NULL) break;
            //Add blinker to car

            //#TODO: INSERT CODE

            //#END TODO
            b_index++;
        }

        //Same for front-lights, rear-light, reverse-light
        //#TODO: INSERT CODE
        i++;
    }

    return 0;
}
